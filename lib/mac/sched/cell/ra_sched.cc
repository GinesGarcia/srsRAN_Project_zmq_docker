
#include "ra_sched.h"
#include "../../../ran/gnb_format.h"

using namespace srsgnb;

ra_sched::ra_sched(const cell_configuration& cfg_) :
  cfg(cfg_),
  ra_win_nof_slots(cfg.ul_cfg_common.init_ul_bwp.rach_cfg_common.setup().rach_cfg_generic.ra_resp_win.to_number())
{
  srsran_assert(cfg.dl_cfg_common.init_dl_bwp.pdcch_cfg_common.setup().ra_search_space_present,
                "Creating RA scheduler for cell with no RA search space");
  srsran_assert(cfg.ul_cfg_common.init_ul_bwp.rach_cfg_common_present,
                "Creating RA scheduler for cell with no rach-ConfigCommon");
}

bool ra_sched::handle_rach_indication(const rach_indication_message& msg)
{
  const static unsigned prach_duration = 1; // TODO: Take from config

  // RA-RNTI = 1 + s_id + 14 × t_id + 14 × 80 × f_id + 14 × 80 × 8 × ul_carrier_id
  // s_id = index of the first OFDM symbol (0 <= s_id < 14)
  // t_id = index of first slot of the PRACH (0 <= t_id < 80)
  // f_id = index of the PRACH in the freq domain (0 <= f_id < 8) (for FDD, f_id=0)
  // ul_carrier_id = 0 for NUL and 1 for SUL carrier
  uint16_t ra_rnti = 1 + msg.symbol_index + 14 * msg.slot_rx.slot_idx() + 14 * 80 * msg.frequency_index;

  logger.info("SCHED: New PRACH slot={}, preamble={}, ra-rnti=0x{:x}, temp_crnti=0x{:x}, ta_cmd={}",
              msg.slot_rx,
              msg.preamble_id,
              ra_rnti,
              msg.crnti,
              msg.timing_advance);

  // Check if TC-RNTI value to be scheduled is already under use
  if (not pending_msg3s[msg.crnti].msg3_harq.empty()) {
    logger.warning("PRACH ignored, as the allocated TC-RNTI=0x{:x} is already under use", msg.crnti);
    return false;
  }

  // Find pending RAR with same RA-RNTI
  bool rar_found = false;
  for (pending_rar_t& r : pending_rars) {
    if (r.prach_slot_rx == msg.slot_rx and ra_rnti == r.ra_rnti) {
      if (r.tc_rntis.full()) {
        logger.warning("PRACH ignored, as the the maximum number of RAR grants per tti has been reached");
        return false;
      }
      r.tc_rntis.push_back(msg.crnti);
      rar_found = true;
      break;
    }
  }

  if (not rar_found) {
    // Create new pending RAR
    pending_rars.emplace_back();
    auto& rar_req         = pending_rars.back();
    rar_req.ra_rnti       = ra_rnti;
    rar_req.prach_slot_rx = msg.slot_rx;
    // First slot after PRACH with active DL slot represents the start of the RAR window.
    for (unsigned sl_idx = 0; sl_idx < cfg.nof_slots_per_frame; ++sl_idx) {
      slot_point sl_start = rar_req.prach_slot_rx + prach_duration + sl_idx;
      if (cfg.is_dl_enabled(sl_start)) {
        rar_req.rar_window = {sl_start, sl_start + ra_win_nof_slots};
        break;
      }
    }
    rar_req.tc_rntis.push_back(msg.crnti);
  }

  // Store Msg3 to allocate
  pending_msg3s[msg.crnti].ind_msg = msg;

  return true;
}

void ra_sched::run_slot(cell_resource_allocator& res_alloc)
{
  const unsigned          msg3_delay    = 6; // TODO: Make it configurable
  slot_resource_allocator rar_slot_res  = res_alloc[0];
  slot_resource_allocator msg3_slot_res = res_alloc[msg3_delay];

  if (not rar_slot_res.is_dl_active() or not msg3_slot_res.is_ul_active()) {
    // Early exit. RAR only allowed if PDCCH and PDSCH are available and respective Msg3 slot is available for UL
    return;
  }

  for (auto it = pending_rars.begin(); it != pending_rars.end();) {
    pending_rar_t& rar_req = *it;

    // In case of RAR outside RAR window:
    // - if window has passed, discard RAR
    // - if window hasn't started, stop loop, as RARs are ordered by TTI
    if (not rar_req.rar_window.contains(rar_slot_res.slot)) {
      if (rar_slot_res.slot >= rar_req.rar_window.stop()) {
        fmt::memory_buffer str_buffer;
        fmt::format_to(str_buffer,
                       "SCHED: Could not transmit RAR within the window={}, prach_slot={}, slot_tx={}",
                       rar_req.rar_window,
                       rar_req.prach_slot_rx,
                       rar_slot_res.slot);
        fmt::print("{}\n", to_c_str(str_buffer));
        logger.warning("{}", to_c_str(str_buffer));
        it = pending_rars.erase(it);
        continue;
      }
      return;
    }

    // Early checks for space
    if (rar_slot_res.dl_res().rars.full() or msg3_slot_res.ul_res().puschs.full()) {
      log_postponed_rar(rar_req, "No space in sched result.");
      break;
    }

    // Try to schedule DCIs + RBGs for RAR Grants
    unsigned nof_allocs = allocate_rar(rar_req, rar_slot_res, msg3_slot_res);

    if (nof_allocs > 0) {
      // If RAR allocation was successful:
      // - in case all Msg3 grants were allocated, remove pending RAR, and continue with following RAR
      // - otherwise, erase only Msg3 grants that were allocated, and stop iteration

      if (nof_allocs == rar_req.tc_rntis.size()) {
        it = pending_rars.erase(it);
      } else {
        // Remove only allocated Msg3 grants
        std::copy(rar_req.tc_rntis.begin() + nof_allocs, rar_req.tc_rntis.end(), rar_req.tc_rntis.begin());
        rar_req.tc_rntis.resize(rar_req.tc_rntis.size() - nof_allocs);
        break;
      }
    } else {
      // If RAR allocation was not successful, try next pending RAR
      ++it;
    }
  }
}

unsigned ra_sched::allocate_rar(const pending_rar_t&     rar,
                                slot_resource_allocator& rar_alloc,
                                slot_resource_allocator& msg3_alloc)
{
  //  static const unsigned rar_aggr_level   = 2;
  static const unsigned nof_prbs_per_rar = 4, nof_prbs_per_msg3 = 3;

  // 1. Check space in DL sched result for RAR
  if (rar_alloc.dl_res().rars.full()) {
    // early exit
    log_postponed_rar(rar, "No PDSCH space for RAR");
    return 0;
  }

  // Start with the higher number of Msg3 grants to allocate, and keep decrementing based on available space
  unsigned max_nof_allocs = rar.tc_rntis.size();

  // 2. Check space in UL sched result for Msg3
  max_nof_allocs =
      std::min(max_nof_allocs, (unsigned)(msg3_alloc.ul_res().puschs.capacity() - rar_alloc.ul_res().puschs.size()));
  if (max_nof_allocs == 0) {
    // early exit
    log_postponed_rar(rar, "No space in PUSCH for Msg3");
    return 0;
  }

  // 3. Check PRBs available in PDSCH for RAR
  prb_interval avail_dl_prbs;
  {
    unsigned   nof_rar_prbs = nof_prbs_per_rar * max_nof_allocs;
    prb_bitmap used_prbs    = rar_alloc.used_dl_prbs();
    avail_dl_prbs           = find_empty_interval_of_length(used_prbs, nof_rar_prbs, 0);
    max_nof_allocs          = std::min(avail_dl_prbs.length() / nof_prbs_per_rar, max_nof_allocs);
    if (max_nof_allocs == 0) {
      // early exit
      log_postponed_rar(rar, "Not enough PRBs for RAR.");
      return 0;
    }
  }

  // 4. Check PRBs available in PUSCH for Msg3
  prb_interval avail_ul_prbs;
  {
    unsigned   nof_msg3_prbs = nof_prbs_per_msg3 * max_nof_allocs;
    prb_bitmap used_ul_prbs  = msg3_alloc.used_ul_prbs();
    avail_ul_prbs            = find_empty_interval_of_length(used_ul_prbs, nof_msg3_prbs, 0);
    max_nof_allocs           = std::min(avail_ul_prbs.length() / nof_prbs_per_msg3, max_nof_allocs);
    if (max_nof_allocs == 0) {
      // early exit
      log_postponed_rar(rar, "No enough PRBs for Msg3.");
      return 0;
    }
  }

  // 6. Find space in PDCCH for RAR
  // TODO

  // Status: RAR allocation was successful
  avail_ul_prbs.resize_by(nof_prbs_per_msg3 * max_nof_allocs);
  avail_dl_prbs.resize_by(nof_prbs_per_rar * max_nof_allocs);

  // 7. Fill RAR and Msg3 PDSCH, PUSCH and DCI
  fill_rar_grant(rar, avail_dl_prbs, avail_ul_prbs, rar_alloc, msg3_alloc, max_nof_allocs);

  return max_nof_allocs;
}

void ra_sched::fill_rar_grant(const pending_rar_t&     rar_request,
                              const prb_interval&      rar_prbs,
                              const prb_interval&      msg3_prbs,
                              slot_resource_allocator& rar_alloc,
                              slot_resource_allocator& msg3_alloc,
                              unsigned                 nof_msg3_grants)
{
  static const unsigned nof_prbs_per_msg3 = 3;
  static const unsigned max_msg3_retxs    = 4;
  static const unsigned msg3_mcs          = 0;

  // Allocate PRBs and space for RAR
  rar_alloc_info& rar = rar_alloc.alloc_rar(rar_prbs);

  // Fill RAR DCI
  // TODO

  // Fill RAR grant info
  rar.rapid = rar_request.ra_rnti;
  rar.ta    = pending_msg3s[rar_request.tc_rntis[0]].ind_msg.timing_advance;
  // TODO

  unsigned last_msg3_start = msg3_prbs.start();
  for (unsigned i = 0; i < nof_msg3_grants; ++i) {
    auto& msg3_req  = pending_msg3s[rar_request.tc_rntis[i]];
    auto& msg3_harq = pending_msg3s[rar_request.tc_rntis[i]].msg3_harq;
    srsran_sanity_check(msg3_harq.empty(), "Pending Msg3 should not have been added if HARQ is busy.");

    // Add Msg3 grant in RAR info
    rar.msg3s.emplace_back();
    msg3_freq_alloc& msg3_grant = rar.msg3s.back();
    msg3_grant.tc_rnti          = msg3_req.ind_msg.crnti;

    // Determine Msg3 PRB interval
    msg3_grant.prbs = {last_msg3_start, last_msg3_start + nof_prbs_per_msg3};
    last_msg3_start = last_msg3_start + nof_prbs_per_msg3;

    // Fill Msg3 DCI in RAR
    dci_ul_t msg3_dci{};
    // TODO

    // Allocate and fill PUSCH for Msg3
    ul_sched_info& pusch = msg3_alloc.alloc_pusch(msg3_grant.prbs);
    pusch.crnti          = msg3_req.ind_msg.crnti;
    // TODO

    // Allocate Msg3 UL HARQ
    bool success = msg3_harq.new_tx(msg3_alloc.slot, msg3_grant.prbs, msg3_mcs, max_msg3_retxs, msg3_dci);
    srsran_sanity_check(success, "Unexpected HARQ allocation return");
  }

  log_rar(rar);
}

void ra_sched::log_postponed_rar(const pending_rar_t& rar, const char* cause_str)
{
  logger.debug("SCHED: RAR allocation for ra-rnti={} was postponed. Cause: {}", rar.ra_rnti, cause_str);
}

void ra_sched::log_rar(const rar_alloc_info& rar)
{
  if (not logger.info.enabled()) {
    return;
  }
  fmt::memory_buffer fmtbuf;
  for (const msg3_freq_alloc& msg3 : rar.msg3s) {
    fmt::format_to(fmtbuf, "{{0x{:x}: prbs={}}}", msg3.tc_rnti, msg3.prbs);
  }
  logger.info("SCHED: RAR ra-rnti=0x{:x}, ta={}, msg3s=[{}]", rar.rapid, rar.ta, to_c_str(fmtbuf));
}
