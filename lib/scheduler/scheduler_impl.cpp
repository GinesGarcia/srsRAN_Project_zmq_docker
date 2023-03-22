/*
 *
 * Copyright 2021-2023 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "scheduler_impl.h"
#include "common_scheduling/ssb_scheduler.h"
#include "ue_scheduling/ue_scheduler_impl.h"
#include "srsran/scheduler/config/scheduler_cell_config_validator.h"
#include "srsran/scheduler/config/scheduler_ue_config_validator.h"

using namespace srsran;

scheduler_impl::scheduler_impl(const scheduler_config& sched_cfg_) :
  expert_params(sched_cfg_.expert_params),
  config_notifier(sched_cfg_.config_notifier),
  metrics_notifier(sched_cfg_.metrics_notifier),
  logger(srslog::fetch_basic_logger("SCHED")),
  sched_result_logger(expert_params.log_broadcast_messages),
  metrics(expert_params.metrics_report_period, sched_cfg_.metrics_notifier)
{
}

bool scheduler_impl::handle_cell_configuration_request(const sched_cell_configuration_request_message& msg)
{
  srsran_assert(msg.cell_index < MAX_NOF_DU_CELLS, "cell index={} is not valid", msg.cell_index);
  srsran_assert(not cells.contains(msg.cell_index), "cell={} already exists", msg.cell_index);
  srsran_assert(
      not config_validators::validate_sched_cell_configuration_request_message(msg, expert_params).is_error(),
      "Invalid cell configuration request message. Cause: {}",
      config_validators::validate_sched_cell_configuration_request_message(msg, expert_params).error().c_str());

  // Check if it is a new DU Cell Group.
  if (not groups.contains(msg.cell_group_index)) {
    // If it is a new group, create a new instance.
    groups.emplace(msg.cell_group_index,
                   std::make_unique<ue_scheduler_impl>(expert_params.ue, config_notifier, metrics, sched_ev_logger));
  }
  ue_scheduler& ue_sched = *groups[msg.cell_group_index];

  // Create a new cell scheduler instance.
  cells.emplace(msg.cell_index, std::make_unique<scheduler_cell>(expert_params, msg, ue_sched, sched_ev_logger));
  scheduler_cell& cell = *cells[msg.cell_index];

  // Register new cell in the UE scheduler.
  ue_sched.add_cell(
      ue_scheduler_cell_params{msg.cell_index, &cell.pdcch_sch, &cell.pucch_alloc, &cell.uci_alloc, &cell.res_grid});

  logger.info("Cell with cell_index={} was configured.", msg.cell_index);

  return true;
}

void scheduler_impl::handle_ue_creation_request(const sched_ue_creation_request_message& ue_request)
{
  error_type<std::string> result = config_validators::validate_sched_ue_creation_request_message(ue_request);
  if (result.is_error()) {
    report_fatal_error("Invalid ue={} creation request message. Cause: {}", ue_request.crnti, result.error());
  }

  du_cell_index_t pcell_index = ue_request.cfg.cells[0].serv_cell_cfg.cell_index;
  ue_to_cell_group_index.emplace(ue_request.ue_index, cells[pcell_index]->cell_cfg.cell_group_index);

  // Dispatch UE creation to UE scheduler associated to the PCell.
  cells[pcell_index]->ue_sched.get_ue_configurator().handle_ue_creation_request(ue_request);
}

void scheduler_impl::handle_ue_reconfiguration_request(const sched_ue_reconfiguration_message& ue_request)
{
  srsran_assert(ue_to_cell_group_index.contains(ue_request.ue_index), "UE={} not yet created", ue_request.ue_index);
  du_cell_group_index_t group_index = ue_to_cell_group_index[ue_request.ue_index];

  groups[group_index]->get_ue_configurator().handle_ue_reconfiguration_request(ue_request);
}

void scheduler_impl::handle_ue_removal_request(du_ue_index_t ue_index)
{
  srsran_assert(ue_to_cell_group_index.contains(ue_index), "UE={} not yet created", ue_index);
  du_cell_group_index_t group_index = ue_to_cell_group_index[ue_index];

  groups[group_index]->get_ue_configurator().handle_ue_removal_request(ue_index);
}

void scheduler_impl::handle_rach_indication(const rach_indication_message& msg)
{
  srsran_assert(cells.contains(msg.cell_index), "cell={} does not exist", msg.cell_index);
  cells[msg.cell_index]->ra_sch.handle_rach_indication(msg);
}

void scheduler_impl::handle_ul_bsr_indication(const ul_bsr_indication_message& bsr)
{
  if (not ue_to_cell_group_index.contains(bsr.ue_index)) {
    logger.warning("ue={}: Discarding UL BSR. Cause: UE not recognized", bsr.ue_index);
    return;
  }
  du_cell_group_index_t group_index = ue_to_cell_group_index[bsr.ue_index];

  groups[group_index]->get_feedback_handler().handle_ul_bsr_indication(bsr);
}

void scheduler_impl::handle_dl_buffer_state_indication(const dl_buffer_state_indication_message& bs)
{
  if (not ue_to_cell_group_index.contains(bs.ue_index)) {
    logger.warning("ue={}: Discarding DL buffer status update. Cause: UE not recognized", bs.ue_index);
    return;
  }
  du_cell_group_index_t group_index = ue_to_cell_group_index[bs.ue_index];

  groups[group_index]->get_dl_buffer_state_indication_handler().handle_dl_buffer_state_indication(bs);
}

void scheduler_impl::handle_crc_indication(const ul_crc_indication& crc_ind)
{
  bool has_msg3_crcs = std::any_of(
      crc_ind.crcs.begin(), crc_ind.crcs.end(), [](const auto& pdu) { return pdu.ue_index == INVALID_DU_UE_INDEX; });

  if (has_msg3_crcs) {
    ul_crc_indication msg3_crcs{}, ue_crcs{};
    msg3_crcs.sl_rx      = crc_ind.sl_rx;
    msg3_crcs.cell_index = crc_ind.cell_index;
    ue_crcs.sl_rx        = crc_ind.sl_rx;
    ue_crcs.cell_index   = crc_ind.cell_index;
    for (const ul_crc_pdu_indication& crc_pdu : crc_ind.crcs) {
      if (crc_pdu.ue_index == INVALID_DU_UE_INDEX) {
        msg3_crcs.crcs.push_back(crc_pdu);
      } else {
        ue_crcs.crcs.push_back(crc_pdu);
      }
    }
    // Forward CRC to Msg3 HARQs that has no ueId yet associated.
    cells[crc_ind.cell_index]->ra_sch.handle_crc_indication(msg3_crcs);
    // Forward remaining CRCs to UE scheduler.
    cells[crc_ind.cell_index]->ue_sched.get_feedback_handler().handle_crc_indication(ue_crcs);
  } else {
    cells[crc_ind.cell_index]->ue_sched.get_feedback_handler().handle_crc_indication(crc_ind);
  }
}

void scheduler_impl::handle_uci_indication(const uci_indication& uci)
{
  srsran_assert(cells.contains(uci.cell_index), "cell={} does not exist", uci.cell_index);

  cells[uci.cell_index]->ue_sched.get_feedback_handler().handle_uci_indication(uci);
}

void scheduler_impl::handle_dl_mac_ce_indication(const dl_mac_ce_indication& mac_ce)
{
  if (not ue_to_cell_group_index.contains(mac_ce.ue_index)) {
    logger.warning("ue={}: Discarding MAC CE update. Cause: UE not recognized", mac_ce.ue_index);
    return;
  }
  du_cell_group_index_t group_index = ue_to_cell_group_index[mac_ce.ue_index];

  groups[group_index]->get_feedback_handler().handle_dl_mac_ce_indication(mac_ce);
}

const sched_result* scheduler_impl::slot_indication(slot_point sl_tx, du_cell_index_t cell_index)
{
  srsran_assert(cells.contains(cell_index), "cell={} does not exist", cell_index);
  scheduler_cell& cell = *cells[cell_index];

  if (cell_index == to_du_cell_index(0)) {
    // Set scheduler logger context.
    logger.set_context(sl_tx.sfn(), sl_tx.slot_index());
  }

  // > Mark slot start for logging purposes.
  sched_result_logger.on_slot_start();

  // > Run scheduler for the given slot and cell.
  cell.run_slot(sl_tx);

  // > Log processed events.
  sched_ev_logger.log();

  // > Log the scheduler results.
  sched_result_logger.on_scheduler_result(cell.res_grid[0].result);

  // > Push the scheduler results to the metrics handler.
  metrics.push_result(sl_tx, cell.res_grid[0].result);

  // Return result for the slot.
  return &cell.res_grid[0].result;
}

void scheduler_impl::handle_paging_information(const sched_paging_information& pi)
{
  for (const auto cell_id : pi.paging_cells) {
    cells[cell_id]->pg_sch.handle_paging_information(pi);
  }
}
