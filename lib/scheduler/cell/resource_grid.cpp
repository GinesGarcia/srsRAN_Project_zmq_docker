/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "resource_grid.h"

using namespace srsgnb;

static subcarrier_spacing get_max_scs(const dl_configuration_common& dl_cfg)
{
  // Note: assumes sorted list. TODO: Verify.
  return dl_cfg.freq_info_dl.scs_carrier_list.back().scs;
}

/////////////////////////////////////////////////////////////////////////

carrier_subslot_resource_grid::carrier_subslot_resource_grid(const scs_specific_carrier& carrier_cfg_) :
  carrier_cfg(carrier_cfg_), slot_rbs(NOF_OFDM_SYM_PER_SLOT_NORMAL_CP * carrier_cfg.carrier_bandwidth)
{}

void carrier_subslot_resource_grid::clear()
{
  slot_rbs.reset();
}

void carrier_subslot_resource_grid::fill(ofdm_symbol_range symbols, prb_interval prbs)
{
  srsran_sanity_check(rb_dims().contains(prbs), "PRB interval out-of-bounds");
  srsran_sanity_check(symbols.stop() <= NOF_OFDM_SYM_PER_SLOT_NORMAL_CP, "OFDM symbols out-of-bounds");

  prbs.displace_by(-(int)offset());
  for (unsigned i = symbols.start(); i < symbols.stop(); ++i) {
    slot_rbs.fill(prbs.start() + i * nof_prbs(), prbs.stop() + i * nof_prbs());
  }
}

bool carrier_subslot_resource_grid::collides(ofdm_symbol_range symbols, prb_interval prbs) const
{
  prbs.displace_by(-(int)offset());
  for (unsigned i = symbols.start(); i < symbols.stop(); ++i) {
    if (slot_rbs.any(prbs.start() + i * nof_prbs(), prbs.stop() + i * nof_prbs())) {
      return true;
    }
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cell_slot_resource_grid::carrier_resource_grid::carrier_resource_grid(const scs_specific_carrier& carrier_cfg) :
  sch_prbs(carrier_cfg.carrier_bandwidth), subslot_prbs(carrier_cfg)
{}

cell_slot_resource_grid::cell_slot_resource_grid(span<const scs_specific_carrier> scs_carriers)
{
  std::fill(numerology_to_grid_idx.begin(), numerology_to_grid_idx.end(), std::numeric_limits<unsigned>::max());

  // TODO: For UL, use UL carrier list instead.
  carrier_grids.reserve(scs_carriers.size());
  for (auto& scs_carrier : scs_carriers) {
    carrier_grids.emplace_back(scs_carrier);
    numerology_to_grid_idx[to_numerology_value(scs_carrier.scs)] = carrier_grids.size() - 1;
  }
}

void cell_slot_resource_grid::clear()
{
  for (auto& carrier : carrier_grids) {
    carrier.subslot_prbs.clear();
    carrier.sch_prbs.reset();
  }
}

void cell_slot_resource_grid::fill(grant_info grant)
{
  auto& carrier = get_carrier(grant.scs);

  // Fill RB grid.
  carrier.subslot_prbs.fill(grant.symbols, grant.crbs);
  if (grant.ch == grant_info::channel::sch) {
    carrier.sch_prbs.fill(grant.crbs.start(), grant.crbs.stop());
  }
}

bool cell_slot_resource_grid::collides(grant_info grant) const
{
  auto& carrier = get_carrier(grant.scs);
  return carrier.subslot_prbs.collides(grant.symbols, grant.crbs);
}

bool cell_slot_resource_grid::collides(subcarrier_spacing scs, ofdm_symbol_range ofdm_symbols, prb_interval crbs) const
{
  auto& carrier = get_carrier(scs);
  return carrier.subslot_prbs.collides(ofdm_symbols, crbs);
}

prb_bitmap cell_slot_resource_grid::sch_prbs(const bwp_configuration& bwp_cfg) const
{
  // TODO: Need to display PRB bitmap by BWP offset.
  return get_carrier(bwp_cfg.scs).sch_prbs;
}

cell_slot_resource_grid::carrier_resource_grid& cell_slot_resource_grid::get_carrier(subcarrier_spacing scs)
{
  size_t idx = numerology_to_grid_idx[to_numerology_value(scs)];
  srsran_sanity_check(idx < carrier_grids.size(), "Invalid numerology={}", scs);
  return carrier_grids[idx];
}

const cell_slot_resource_grid::carrier_resource_grid& cell_slot_resource_grid::get_carrier(subcarrier_spacing scs) const
{
  size_t idx = numerology_to_grid_idx[to_numerology_value(scs)];
  srsran_sanity_check(idx < carrier_grids.size(), "Invalid numerology={}", scs);
  return carrier_grids[idx];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cell_slot_resource_allocator::cell_slot_resource_allocator(const cell_configuration& cfg_) :
  cfg(cfg_),
  dl_res_grid(cfg.dl_cfg_common.freq_info_dl.scs_carrier_list),
  ul_res_grid(cfg.dl_cfg_common.freq_info_dl.scs_carrier_list)
{}

cell_slot_resource_allocator::cell_slot_resource_allocator(const cell_configuration&  cfg_,
                                                           span<scs_specific_carrier> dl_scs_carriers,
                                                           span<scs_specific_carrier> ul_scs_carriers) :
  cfg(cfg_), dl_res_grid(dl_scs_carriers), ul_res_grid(ul_scs_carriers)
{}

void cell_slot_resource_allocator::slot_indication(slot_point sl)
{
  clear();
  slot = sl;
}

void cell_slot_resource_allocator::clear()
{
  result = {};
  dl_res_grid.clear();
  ul_res_grid.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cell_resource_allocator::cell_resource_allocator(const cell_configuration& cfg_) : cfg(cfg_)
{
  // Create cell_slot_resource_allocator objects.
  std::vector<scs_specific_carrier> slot_scs_carriers;
  subcarrier_spacing                max_scs = cfg.dl_cfg_common.freq_info_dl.scs_carrier_list.back().scs;
  slots.resize(GRID_NOF_SUBFRAMES * get_nof_slots_per_subframe(max_scs));
  unsigned nof_slots_per_subframe = get_nof_slots_per_subframe(max_scs);
  for (unsigned i = 0; i < slots.size(); ++i) {
    for (auto& carrier : cfg.dl_cfg_common.freq_info_dl.scs_carrier_list) {
      unsigned current_slot_dur = nof_slots_per_subframe / get_nof_slots_per_subframe(carrier.scs);
      if (i % current_slot_dur == 0) {
        slot_scs_carriers.emplace_back(carrier);
      }
    }
    slots[i] = std::make_unique<cell_slot_resource_allocator>(cfg, slot_scs_carriers, slot_scs_carriers);
    slot_scs_carriers.clear();
  }
}

void cell_resource_allocator::slot_indication(slot_point sl_tx)
{
  srsran_sanity_check(not last_slot_ind.valid() or last_slot_ind + 1 == sl_tx, "Slot indication was skipped");
  srsran_sanity_check(sl_tx.numerology() == to_numerology_value(get_max_scs(cfg.dl_cfg_common)),
                      "Invalid slot numerology");

  if (srsran_unlikely(not last_slot_ind.valid())) {
    // First call to slot_indication. Set slot of all slot cell resource grids.
    for (unsigned i = 0; i < slots.size(); ++i) {
      slots[(sl_tx + i).to_uint() % slots.size()]->slot_indication(sl_tx + i);
    }
  } else {
    // Reset old slot state and set its new future slot.
    auto& old_slot_res = slots[(sl_tx - 1).to_uint() % slots.size()];
    old_slot_res->slot_indication(old_slot_res->slot + slots.size());
  }
  last_slot_ind = sl_tx;
}
