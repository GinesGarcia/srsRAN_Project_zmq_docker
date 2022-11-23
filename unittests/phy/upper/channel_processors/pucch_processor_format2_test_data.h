/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#pragma once

// This file was generated using the following MATLAB class on 25-Nov-2022:
//   + "srsPUCCHProcessorFormat2Unittest.m"

#include "../../support/resource_grid_test_doubles.h"
#include "srsgnb/phy/upper/channel_processors/pucch_processor.h"
#include "srsgnb/support/file_vector.h"

namespace srsgnb {

struct context_t {
  unsigned                               grid_nof_prb;
  unsigned                               grid_nof_symbols;
  pucch_processor::format2_configuration config;
};

struct test_case_t {
  context_t                                               context;
  file_vector<resource_grid_reader_spy::expected_entry_t> grid;
  file_vector<uint8_t>                                    harq_ack;
  file_vector<uint8_t>                                    sr;
  file_vector<uint8_t>                                    csi_part_1;
  file_vector<uint8_t>                                    csi_part_2;
};

static const std::vector<test_case_t> pucch_processor_format2_test_data = {
    // clang-format off
  {{182, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 54, 128, 13, {}, 3, 0, 1, 26279, 617, 30091, 3, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols0.dat"}, {"test_data/pucch_processor_format2_test_harq0.dat"}, {"test_data/pucch_processor_format2_test_sr0.dat"}, {"test_data/pucch_processor_format2_test_csi10.dat"}, {"test_data/pucch_processor_format2_test_csi20.dat"}},
  {{186, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 39, 147, 27, {}, 2, 0, 1, 10985, 416, 776, 3, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols1.dat"}, {"test_data/pucch_processor_format2_test_harq1.dat"}, {"test_data/pucch_processor_format2_test_sr1.dat"}, {"test_data/pucch_processor_format2_test_csi11.dat"}, {"test_data/pucch_processor_format2_test_csi21.dat"}},
  {{204, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 54, 150, 1, {}, 1, 0, 1, 52050, 231, 30082, 3, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols2.dat"}, {"test_data/pucch_processor_format2_test_harq2.dat"}, {"test_data/pucch_processor_format2_test_sr2.dat"}, {"test_data/pucch_processor_format2_test_csi12.dat"}, {"test_data/pucch_processor_format2_test_csi22.dat"}},
  {{242, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 7, 235, 0, {}, 1, 0, 1, 61442, 911, 15064, 3, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols3.dat"}, {"test_data/pucch_processor_format2_test_harq3.dat"}, {"test_data/pucch_processor_format2_test_sr3.dat"}, {"test_data/pucch_processor_format2_test_csi13.dat"}, {"test_data/pucch_processor_format2_test_csi23.dat"}},
  {{96, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 44, 52, 13, {}, 1, 0, 1, 13334, 659, 13286, 3, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols4.dat"}, {"test_data/pucch_processor_format2_test_harq4.dat"}, {"test_data/pucch_processor_format2_test_sr4.dat"}, {"test_data/pucch_processor_format2_test_csi14.dat"}, {"test_data/pucch_processor_format2_test_csi24.dat"}},
  {{208, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 11, 197, 7, {}, 1, 0, 1, 28449, 925, 4781, 3, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols5.dat"}, {"test_data/pucch_processor_format2_test_harq5.dat"}, {"test_data/pucch_processor_format2_test_sr5.dat"}, {"test_data/pucch_processor_format2_test_csi15.dat"}, {"test_data/pucch_processor_format2_test_csi25.dat"}},
  {{245, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 9, 236, 2, {}, 4, 0, 1, 39458, 993, 34756, 3, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols6.dat"}, {"test_data/pucch_processor_format2_test_harq6.dat"}, {"test_data/pucch_processor_format2_test_sr6.dat"}, {"test_data/pucch_processor_format2_test_csi16.dat"}, {"test_data/pucch_processor_format2_test_csi26.dat"}},
  {{237, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 28, 209, 18, {}, 2, 0, 1, 33813, 143, 44284, 3, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols7.dat"}, {"test_data/pucch_processor_format2_test_harq7.dat"}, {"test_data/pucch_processor_format2_test_sr7.dat"}, {"test_data/pucch_processor_format2_test_csi17.dat"}, {"test_data/pucch_processor_format2_test_csi27.dat"}},
  {{186, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 37, 149, 15, {}, 1, 0, 1, 27684, 24, 21619, 3, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols8.dat"}, {"test_data/pucch_processor_format2_test_harq8.dat"}, {"test_data/pucch_processor_format2_test_sr8.dat"}, {"test_data/pucch_processor_format2_test_csi18.dat"}, {"test_data/pucch_processor_format2_test_csi28.dat"}},
  {{181, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 117, 64, 112, {}, 1, 0, 1, 46157, 225, 41042, 3, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols9.dat"}, {"test_data/pucch_processor_format2_test_harq9.dat"}, {"test_data/pucch_processor_format2_test_sr9.dat"}, {"test_data/pucch_processor_format2_test_csi19.dat"}, {"test_data/pucch_processor_format2_test_csi29.dat"}},
  {{260, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 63, 197, 17, {}, 1, 0, 1, 39301, 845, 30606, 3, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols10.dat"}, {"test_data/pucch_processor_format2_test_harq10.dat"}, {"test_data/pucch_processor_format2_test_sr10.dat"}, {"test_data/pucch_processor_format2_test_csi110.dat"}, {"test_data/pucch_processor_format2_test_csi210.dat"}},
  {{7, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 5, 2, 4, {}, 1, 0, 1, 23402, 519, 26209, 3, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols11.dat"}, {"test_data/pucch_processor_format2_test_harq11.dat"}, {"test_data/pucch_processor_format2_test_sr11.dat"}, {"test_data/pucch_processor_format2_test_csi111.dat"}, {"test_data/pucch_processor_format2_test_csi211.dat"}},
  {{258, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 6, 252, 0, {}, 6, 0, 1, 54894, 849, 45733, 3, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols12.dat"}, {"test_data/pucch_processor_format2_test_harq12.dat"}, {"test_data/pucch_processor_format2_test_sr12.dat"}, {"test_data/pucch_processor_format2_test_csi112.dat"}, {"test_data/pucch_processor_format2_test_csi212.dat"}},
  {{245, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 119, 126, 57, {}, 3, 0, 1, 45361, 224, 45726, 3, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols13.dat"}, {"test_data/pucch_processor_format2_test_harq13.dat"}, {"test_data/pucch_processor_format2_test_sr13.dat"}, {"test_data/pucch_processor_format2_test_csi113.dat"}, {"test_data/pucch_processor_format2_test_csi213.dat"}},
  {{223, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 32, 191, 11, {}, 2, 0, 1, 1399, 211, 25736, 3, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols14.dat"}, {"test_data/pucch_processor_format2_test_harq14.dat"}, {"test_data/pucch_processor_format2_test_sr14.dat"}, {"test_data/pucch_processor_format2_test_csi114.dat"}, {"test_data/pucch_processor_format2_test_csi214.dat"}},
  {{208, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 174, 34, 38, {}, 2, 0, 1, 22251, 949, 53090, 3, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols15.dat"}, {"test_data/pucch_processor_format2_test_harq15.dat"}, {"test_data/pucch_processor_format2_test_sr15.dat"}, {"test_data/pucch_processor_format2_test_csi115.dat"}, {"test_data/pucch_processor_format2_test_csi215.dat"}},
  {{264, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 85, 179, 78, {}, 1, 0, 1, 26833, 644, 38272, 3, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols16.dat"}, {"test_data/pucch_processor_format2_test_harq16.dat"}, {"test_data/pucch_processor_format2_test_sr16.dat"}, {"test_data/pucch_processor_format2_test_csi116.dat"}, {"test_data/pucch_processor_format2_test_csi216.dat"}},
  {{242, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 172, 70, 36, {}, 1, 0, 1, 38191, 678, 10350, 3, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols17.dat"}, {"test_data/pucch_processor_format2_test_harq17.dat"}, {"test_data/pucch_processor_format2_test_sr17.dat"}, {"test_data/pucch_processor_format2_test_csi117.dat"}, {"test_data/pucch_processor_format2_test_csi217.dat"}},
  {{241, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 235, 6, 35, {}, 6, 0, 1, 39198, 941, 44778, 7, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols18.dat"}, {"test_data/pucch_processor_format2_test_harq18.dat"}, {"test_data/pucch_processor_format2_test_sr18.dat"}, {"test_data/pucch_processor_format2_test_csi118.dat"}, {"test_data/pucch_processor_format2_test_csi218.dat"}},
  {{252, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 205, 47, 7, {}, 3, 0, 1, 39583, 772, 54974, 7, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols19.dat"}, {"test_data/pucch_processor_format2_test_harq19.dat"}, {"test_data/pucch_processor_format2_test_sr19.dat"}, {"test_data/pucch_processor_format2_test_csi119.dat"}, {"test_data/pucch_processor_format2_test_csi219.dat"}},
  {{256, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 110, 146, 62, {}, 2, 0, 1, 64181, 307, 20955, 7, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols20.dat"}, {"test_data/pucch_processor_format2_test_harq20.dat"}, {"test_data/pucch_processor_format2_test_sr20.dat"}, {"test_data/pucch_processor_format2_test_csi120.dat"}, {"test_data/pucch_processor_format2_test_csi220.dat"}},
  {{275, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 6, 269, 3, {}, 2, 0, 1, 17389, 367, 12863, 7, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols21.dat"}, {"test_data/pucch_processor_format2_test_harq21.dat"}, {"test_data/pucch_processor_format2_test_sr21.dat"}, {"test_data/pucch_processor_format2_test_csi121.dat"}, {"test_data/pucch_processor_format2_test_csi221.dat"}},
  {{128, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 42, 86, 13, {}, 1, 0, 1, 34097, 803, 42835, 7, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols22.dat"}, {"test_data/pucch_processor_format2_test_harq22.dat"}, {"test_data/pucch_processor_format2_test_sr22.dat"}, {"test_data/pucch_processor_format2_test_csi122.dat"}, {"test_data/pucch_processor_format2_test_csi222.dat"}},
  {{145, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 122, 23, 59, {}, 1, 0, 1, 65381, 1007, 25993, 7, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols23.dat"}, {"test_data/pucch_processor_format2_test_harq23.dat"}, {"test_data/pucch_processor_format2_test_sr23.dat"}, {"test_data/pucch_processor_format2_test_csi123.dat"}, {"test_data/pucch_processor_format2_test_csi223.dat"}},
  {{155, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 21, 134, 7, {}, 7, 0, 1, 25111, 905, 43287, 7, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols24.dat"}, {"test_data/pucch_processor_format2_test_harq24.dat"}, {"test_data/pucch_processor_format2_test_sr24.dat"}, {"test_data/pucch_processor_format2_test_csi124.dat"}, {"test_data/pucch_processor_format2_test_csi224.dat"}},
  {{275, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 104, 171, 45, {}, 4, 0, 1, 19885, 533, 51303, 7, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols25.dat"}, {"test_data/pucch_processor_format2_test_harq25.dat"}, {"test_data/pucch_processor_format2_test_sr25.dat"}, {"test_data/pucch_processor_format2_test_csi125.dat"}, {"test_data/pucch_processor_format2_test_csi225.dat"}},
  {{235, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 90, 145, 31, {}, 2, 0, 1, 50009, 462, 51181, 7, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols26.dat"}, {"test_data/pucch_processor_format2_test_harq26.dat"}, {"test_data/pucch_processor_format2_test_sr26.dat"}, {"test_data/pucch_processor_format2_test_csi126.dat"}, {"test_data/pucch_processor_format2_test_csi226.dat"}},
  {{223, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 31, 192, 10, {}, 2, 0, 1, 26947, 992, 33535, 7, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols27.dat"}, {"test_data/pucch_processor_format2_test_harq27.dat"}, {"test_data/pucch_processor_format2_test_sr27.dat"}, {"test_data/pucch_processor_format2_test_csi127.dat"}, {"test_data/pucch_processor_format2_test_csi227.dat"}},
  {{260, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 2, 258, 0, {}, 2, 0, 1, 31365, 310, 12257, 7, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols28.dat"}, {"test_data/pucch_processor_format2_test_harq28.dat"}, {"test_data/pucch_processor_format2_test_sr28.dat"}, {"test_data/pucch_processor_format2_test_csi128.dat"}, {"test_data/pucch_processor_format2_test_csi228.dat"}},
  {{244, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 27, 217, 21, {}, 1, 0, 1, 21102, 476, 13333, 7, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols29.dat"}, {"test_data/pucch_processor_format2_test_harq29.dat"}, {"test_data/pucch_processor_format2_test_sr29.dat"}, {"test_data/pucch_processor_format2_test_csi129.dat"}, {"test_data/pucch_processor_format2_test_csi229.dat"}},
  {{73, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 48, 25, 2, {}, 9, 0, 1, 29903, 820, 9598, 7, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols30.dat"}, {"test_data/pucch_processor_format2_test_harq30.dat"}, {"test_data/pucch_processor_format2_test_sr30.dat"}, {"test_data/pucch_processor_format2_test_csi130.dat"}, {"test_data/pucch_processor_format2_test_csi230.dat"}},
  {{262, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 11, 251, 6, {}, 5, 0, 1, 27622, 48, 60179, 7, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols31.dat"}, {"test_data/pucch_processor_format2_test_harq31.dat"}, {"test_data/pucch_processor_format2_test_sr31.dat"}, {"test_data/pucch_processor_format2_test_csi131.dat"}, {"test_data/pucch_processor_format2_test_csi231.dat"}},
  {{244, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 200, 44, 31, {}, 3, 0, 1, 52090, 595, 26770, 7, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols32.dat"}, {"test_data/pucch_processor_format2_test_harq32.dat"}, {"test_data/pucch_processor_format2_test_sr32.dat"}, {"test_data/pucch_processor_format2_test_csi132.dat"}, {"test_data/pucch_processor_format2_test_csi232.dat"}},
  {{256, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 43, 213, 27, {}, 2, 0, 1, 37477, 1022, 36434, 7, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols33.dat"}, {"test_data/pucch_processor_format2_test_harq33.dat"}, {"test_data/pucch_processor_format2_test_sr33.dat"}, {"test_data/pucch_processor_format2_test_csi133.dat"}, {"test_data/pucch_processor_format2_test_csi233.dat"}},
  {{127, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 104, 23, 9, {}, 2, 0, 1, 1447, 582, 53722, 7, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols34.dat"}, {"test_data/pucch_processor_format2_test_harq34.dat"}, {"test_data/pucch_processor_format2_test_sr34.dat"}, {"test_data/pucch_processor_format2_test_csi134.dat"}, {"test_data/pucch_processor_format2_test_csi234.dat"}},
  {{266, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 181, 85, 46, {}, 2, 0, 1, 39476, 124, 32409, 7, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols35.dat"}, {"test_data/pucch_processor_format2_test_harq35.dat"}, {"test_data/pucch_processor_format2_test_sr35.dat"}, {"test_data/pucch_processor_format2_test_csi135.dat"}, {"test_data/pucch_processor_format2_test_csi235.dat"}},
  {{168, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 21, 147, 16, {}, 2, 12, 2, 48807, 616, 28599, 3, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols36.dat"}, {"test_data/pucch_processor_format2_test_harq36.dat"}, {"test_data/pucch_processor_format2_test_sr36.dat"}, {"test_data/pucch_processor_format2_test_csi136.dat"}, {"test_data/pucch_processor_format2_test_csi236.dat"}},
  {{166, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 63, 103, 47, {}, 1, 12, 2, 8429, 335, 50416, 3, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols37.dat"}, {"test_data/pucch_processor_format2_test_harq37.dat"}, {"test_data/pucch_processor_format2_test_sr37.dat"}, {"test_data/pucch_processor_format2_test_csi137.dat"}, {"test_data/pucch_processor_format2_test_csi237.dat"}},
  {{245, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 223, 22, 177, {}, 1, 12, 2, 57771, 669, 7420, 3, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols38.dat"}, {"test_data/pucch_processor_format2_test_harq38.dat"}, {"test_data/pucch_processor_format2_test_sr38.dat"}, {"test_data/pucch_processor_format2_test_csi138.dat"}, {"test_data/pucch_processor_format2_test_csi238.dat"}},
  {{272, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 112, 160, 15, {}, 1, 12, 2, 23740, 74, 34217, 3, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols39.dat"}, {"test_data/pucch_processor_format2_test_harq39.dat"}, {"test_data/pucch_processor_format2_test_sr39.dat"}, {"test_data/pucch_processor_format2_test_csi139.dat"}, {"test_data/pucch_processor_format2_test_csi239.dat"}},
  {{64, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 34, 30, 20, {}, 1, 12, 2, 52027, 680, 24094, 3, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols40.dat"}, {"test_data/pucch_processor_format2_test_harq40.dat"}, {"test_data/pucch_processor_format2_test_sr40.dat"}, {"test_data/pucch_processor_format2_test_csi140.dat"}, {"test_data/pucch_processor_format2_test_csi240.dat"}},
  {{274, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 68, 206, 32, {}, 1, 12, 2, 36473, 27, 26286, 3, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols41.dat"}, {"test_data/pucch_processor_format2_test_harq41.dat"}, {"test_data/pucch_processor_format2_test_sr41.dat"}, {"test_data/pucch_processor_format2_test_csi141.dat"}, {"test_data/pucch_processor_format2_test_csi241.dat"}},
  {{259, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 6, 253, 4, {}, 2, 12, 2, 48013, 446, 51752, 3, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols42.dat"}, {"test_data/pucch_processor_format2_test_harq42.dat"}, {"test_data/pucch_processor_format2_test_sr42.dat"}, {"test_data/pucch_processor_format2_test_csi142.dat"}, {"test_data/pucch_processor_format2_test_csi242.dat"}},
  {{183, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 21, 162, 7, {}, 1, 12, 2, 21190, 220, 33722, 3, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols43.dat"}, {"test_data/pucch_processor_format2_test_harq43.dat"}, {"test_data/pucch_processor_format2_test_sr43.dat"}, {"test_data/pucch_processor_format2_test_csi143.dat"}, {"test_data/pucch_processor_format2_test_csi243.dat"}},
  {{238, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 162, 76, 58, {}, 1, 12, 2, 9486, 475, 34504, 3, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols44.dat"}, {"test_data/pucch_processor_format2_test_harq44.dat"}, {"test_data/pucch_processor_format2_test_sr44.dat"}, {"test_data/pucch_processor_format2_test_csi144.dat"}, {"test_data/pucch_processor_format2_test_csi244.dat"}},
  {{250, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 179, 71, 102, {}, 1, 12, 2, 44753, 567, 47950, 3, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols45.dat"}, {"test_data/pucch_processor_format2_test_harq45.dat"}, {"test_data/pucch_processor_format2_test_sr45.dat"}, {"test_data/pucch_processor_format2_test_csi145.dat"}, {"test_data/pucch_processor_format2_test_csi245.dat"}},
  {{190, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 188, 2, 178, {}, 1, 12, 2, 40493, 775, 1045, 3, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols46.dat"}, {"test_data/pucch_processor_format2_test_harq46.dat"}, {"test_data/pucch_processor_format2_test_sr46.dat"}, {"test_data/pucch_processor_format2_test_csi146.dat"}, {"test_data/pucch_processor_format2_test_csi246.dat"}},
  {{38, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 37, 1, 5, {}, 1, 12, 2, 59201, 236, 21377, 3, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols47.dat"}, {"test_data/pucch_processor_format2_test_harq47.dat"}, {"test_data/pucch_processor_format2_test_sr47.dat"}, {"test_data/pucch_processor_format2_test_csi147.dat"}, {"test_data/pucch_processor_format2_test_csi247.dat"}},
  {{74, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 4, 70, 0, {}, 3, 12, 2, 40940, 475, 4058, 3, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols48.dat"}, {"test_data/pucch_processor_format2_test_harq48.dat"}, {"test_data/pucch_processor_format2_test_sr48.dat"}, {"test_data/pucch_processor_format2_test_csi148.dat"}, {"test_data/pucch_processor_format2_test_csi248.dat"}},
  {{267, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 72, 195, 58, {}, 2, 12, 2, 39868, 2, 34681, 3, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols49.dat"}, {"test_data/pucch_processor_format2_test_harq49.dat"}, {"test_data/pucch_processor_format2_test_sr49.dat"}, {"test_data/pucch_processor_format2_test_csi149.dat"}, {"test_data/pucch_processor_format2_test_csi249.dat"}},
  {{239, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 79, 160, 34, {}, 1, 12, 2, 36734, 865, 8205, 3, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols50.dat"}, {"test_data/pucch_processor_format2_test_harq50.dat"}, {"test_data/pucch_processor_format2_test_sr50.dat"}, {"test_data/pucch_processor_format2_test_csi150.dat"}, {"test_data/pucch_processor_format2_test_csi250.dat"}},
  {{206, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 95, 111, 89, {}, 1, 12, 2, 33673, 962, 999, 3, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols51.dat"}, {"test_data/pucch_processor_format2_test_harq51.dat"}, {"test_data/pucch_processor_format2_test_sr51.dat"}, {"test_data/pucch_processor_format2_test_csi151.dat"}, {"test_data/pucch_processor_format2_test_csi251.dat"}},
  {{273, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 166, 107, 152, {}, 1, 12, 2, 2017, 380, 27925, 3, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols52.dat"}, {"test_data/pucch_processor_format2_test_harq52.dat"}, {"test_data/pucch_processor_format2_test_sr52.dat"}, {"test_data/pucch_processor_format2_test_csi152.dat"}, {"test_data/pucch_processor_format2_test_csi252.dat"}},
  {{245, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 19, 226, 12, {}, 1, 12, 2, 46949, 692, 15624, 3, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols53.dat"}, {"test_data/pucch_processor_format2_test_harq53.dat"}, {"test_data/pucch_processor_format2_test_sr53.dat"}, {"test_data/pucch_processor_format2_test_csi153.dat"}, {"test_data/pucch_processor_format2_test_csi253.dat"}},
  {{208, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 87, 121, 43, {}, 3, 12, 2, 28774, 575, 57323, 7, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols54.dat"}, {"test_data/pucch_processor_format2_test_harq54.dat"}, {"test_data/pucch_processor_format2_test_sr54.dat"}, {"test_data/pucch_processor_format2_test_csi154.dat"}, {"test_data/pucch_processor_format2_test_csi254.dat"}},
  {{159, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 101, 58, 35, {}, 2, 12, 2, 29562, 73, 2381, 7, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols55.dat"}, {"test_data/pucch_processor_format2_test_harq55.dat"}, {"test_data/pucch_processor_format2_test_sr55.dat"}, {"test_data/pucch_processor_format2_test_csi155.dat"}, {"test_data/pucch_processor_format2_test_csi255.dat"}},
  {{272, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 18, 254, 14, {}, 1, 12, 2, 58619, 460, 48945, 7, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols56.dat"}, {"test_data/pucch_processor_format2_test_harq56.dat"}, {"test_data/pucch_processor_format2_test_sr56.dat"}, {"test_data/pucch_processor_format2_test_csi156.dat"}, {"test_data/pucch_processor_format2_test_csi256.dat"}},
  {{274, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 43, 231, 10, {}, 1, 12, 2, 465, 988, 45653, 7, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols57.dat"}, {"test_data/pucch_processor_format2_test_harq57.dat"}, {"test_data/pucch_processor_format2_test_sr57.dat"}, {"test_data/pucch_processor_format2_test_csi157.dat"}, {"test_data/pucch_processor_format2_test_csi257.dat"}},
  {{272, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 49, 223, 25, {}, 1, 12, 2, 64966, 755, 16289, 7, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols58.dat"}, {"test_data/pucch_processor_format2_test_harq58.dat"}, {"test_data/pucch_processor_format2_test_sr58.dat"}, {"test_data/pucch_processor_format2_test_csi158.dat"}, {"test_data/pucch_processor_format2_test_csi258.dat"}},
  {{188, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 30, 158, 26, {}, 1, 12, 2, 62342, 973, 6134, 7, 0, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols59.dat"}, {"test_data/pucch_processor_format2_test_harq59.dat"}, {"test_data/pucch_processor_format2_test_sr59.dat"}, {"test_data/pucch_processor_format2_test_csi159.dat"}, {"test_data/pucch_processor_format2_test_csi259.dat"}},
  {{218, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 33, 185, 20, {}, 4, 12, 2, 33156, 726, 60758, 7, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols60.dat"}, {"test_data/pucch_processor_format2_test_harq60.dat"}, {"test_data/pucch_processor_format2_test_sr60.dat"}, {"test_data/pucch_processor_format2_test_csi160.dat"}, {"test_data/pucch_processor_format2_test_csi260.dat"}},
  {{141, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 76, 65, 52, {}, 2, 12, 2, 49143, 348, 32343, 7, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols61.dat"}, {"test_data/pucch_processor_format2_test_harq61.dat"}, {"test_data/pucch_processor_format2_test_sr61.dat"}, {"test_data/pucch_processor_format2_test_csi161.dat"}, {"test_data/pucch_processor_format2_test_csi261.dat"}},
  {{240, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 9, 231, 4, {}, 1, 12, 2, 46577, 851, 39158, 7, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols62.dat"}, {"test_data/pucch_processor_format2_test_harq62.dat"}, {"test_data/pucch_processor_format2_test_sr62.dat"}, {"test_data/pucch_processor_format2_test_csi162.dat"}, {"test_data/pucch_processor_format2_test_csi262.dat"}},
  {{208, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 115, 93, 48, {}, 1, 12, 2, 25278, 706, 57888, 7, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols63.dat"}, {"test_data/pucch_processor_format2_test_harq63.dat"}, {"test_data/pucch_processor_format2_test_sr63.dat"}, {"test_data/pucch_processor_format2_test_csi163.dat"}, {"test_data/pucch_processor_format2_test_csi263.dat"}},
  {{136, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 39, 97, 36, {}, 1, 12, 2, 22484, 288, 47337, 7, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols64.dat"}, {"test_data/pucch_processor_format2_test_harq64.dat"}, {"test_data/pucch_processor_format2_test_sr64.dat"}, {"test_data/pucch_processor_format2_test_csi164.dat"}, {"test_data/pucch_processor_format2_test_csi264.dat"}},
  {{152, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 46, 106, 33, {}, 1, 12, 2, 25870, 101, 24336, 7, 1, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols65.dat"}, {"test_data/pucch_processor_format2_test_harq65.dat"}, {"test_data/pucch_processor_format2_test_sr65.dat"}, {"test_data/pucch_processor_format2_test_csi165.dat"}, {"test_data/pucch_processor_format2_test_csi265.dat"}},
  {{192, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 57, 135, 24, {}, 5, 12, 2, 23, 650, 58231, 7, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols66.dat"}, {"test_data/pucch_processor_format2_test_harq66.dat"}, {"test_data/pucch_processor_format2_test_sr66.dat"}, {"test_data/pucch_processor_format2_test_csi166.dat"}, {"test_data/pucch_processor_format2_test_csi266.dat"}},
  {{115, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 30, 85, 25, {}, 3, 12, 2, 56227, 763, 3139, 7, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols67.dat"}, {"test_data/pucch_processor_format2_test_harq67.dat"}, {"test_data/pucch_processor_format2_test_sr67.dat"}, {"test_data/pucch_processor_format2_test_csi167.dat"}, {"test_data/pucch_processor_format2_test_csi267.dat"}},
  {{155, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 146, 9, 108, {}, 2, 12, 2, 36926, 722, 33467, 7, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols68.dat"}, {"test_data/pucch_processor_format2_test_harq68.dat"}, {"test_data/pucch_processor_format2_test_sr68.dat"}, {"test_data/pucch_processor_format2_test_csi168.dat"}, {"test_data/pucch_processor_format2_test_csi268.dat"}},
  {{164, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 9, 155, 5, {}, 1, 12, 2, 54003, 990, 63080, 7, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols69.dat"}, {"test_data/pucch_processor_format2_test_harq69.dat"}, {"test_data/pucch_processor_format2_test_sr69.dat"}, {"test_data/pucch_processor_format2_test_csi169.dat"}, {"test_data/pucch_processor_format2_test_csi269.dat"}},
  {{275, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 264, 11, 17, {}, 1, 12, 2, 47177, 223, 24907, 7, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols70.dat"}, {"test_data/pucch_processor_format2_test_harq70.dat"}, {"test_data/pucch_processor_format2_test_sr70.dat"}, {"test_data/pucch_processor_format2_test_csi170.dat"}, {"test_data/pucch_processor_format2_test_csi270.dat"}},
  {{67, 14, {{0, 0}, cyclic_prefix::NORMAL, {0}, 1, 66, 0, {}, 1, 12, 2, 32699, 157, 48795, 7, 4, 0, 0}}, {"test_data/pucch_processor_format2_test_input_symbols71.dat"}, {"test_data/pucch_processor_format2_test_harq71.dat"}, {"test_data/pucch_processor_format2_test_sr71.dat"}, {"test_data/pucch_processor_format2_test_csi171.dat"}, {"test_data/pucch_processor_format2_test_csi271.dat"}},
    // clang-format on
};

} // namespace srsgnb
