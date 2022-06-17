/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

/// \file
/// \brief Example application of SSB transmitted over a radio interface.
///
/// This proof of concept illustrates the integration of the upper and lower physical layer functionalities
/// together with a real-time radio interface (either real, e.g. UHD, or virtual, e.g. ZMQ) by generating and
/// transmitting the Synchronization Signal Block (SSB).
///
/// The application supports different working profiles, run <tt> radio_ssb -h </tt> for usage details.
/// \cond

#include "../radio/radio_notifier_sample.h"
#include "lower_phy_example_factory.h"
#include "rx_symbol_handler_example.h"
#include "srsgnb/phy/adapters/phy_rg_gateway_adapter.h"
#include "srsgnb/phy/adapters/phy_rx_symbol_adapter.h"
#include "srsgnb/phy/adapters/phy_timing_adapter.h"
#include "srsgnb/radio/radio_factory.h"
#include "srsgnb/support/executors/task_worker.h"
#include "srsgnb/support/math_utils.h"
#include "upper_phy_ssb_example.h"
#include <atomic>
#include <csignal>
#include <getopt.h>
#include <string>

struct configuration_profile {
  std::string           name;
  std::string           description;
  std::function<void()> function;
};

using namespace srsgnb;

static std::string log_level = "warning";

// Program parameters.
static subcarrier_spacing                        scs                        = subcarrier_spacing::kHz15;
static unsigned                                  max_processing_delay_slots = 4;
static unsigned                                  ul_to_dl_slot_offset       = 1;
static cyclic_prefix                             cp                         = cyclic_prefix::NORMAL;
static double                                    dl_center_freq             = 3489.42e6;
static double                                    ssb_center_freq            = 3488.16e6;
static double                                    tx_gain                    = 60.0;
static double                                    rx_freq                    = 3.5e9;
static double                                    rx_gain                    = 60.0;
static unsigned                                  nof_ports                  = 1;
static unsigned                                  nof_sectors                = 1;
static std::string                               driver_name                = "uhd";
static std::string                               device_arguments           = "type=b200";
static std::vector<std::string>                  tx_channel_args            = {};
static std::vector<std::string>                  rx_channel_args            = {};
static double                                    sampling_rate_hz           = 23.04e6;
static unsigned                                  bw_rb                      = 52;
static radio_configuration::over_the_wire_format otw_format     = radio_configuration::over_the_wire_format::SC16;
static unsigned                                  duration_slots = 60000;
static bool                                      zmq_loopback   = true;
static ssb_pattern_case                          ssb_pattern    = ssb_pattern_case::A;

/// Defines a set of configuration profiles.
static const std::vector<configuration_profile> profiles = {
    {"b200_20MHz",
     "Single channel B200 USRP 20MHz bandwidth.",
     []() {
       // Do nothing.
     }},
    {"b200_50MHz",
     "Single channel B200 USRP 50MHz bandwidth.",
     []() {
       device_arguments = "type=b200";
       sampling_rate_hz = 61.44e6;
       bw_rb            = 270;
       otw_format       = radio_configuration::over_the_wire_format::SC12;
     }},
    {"x300_50MHz",
     "Single channel X3x0 USRP 50MHz bandwidth.",
     []() {
       device_arguments = "type=x300";
       sampling_rate_hz = 92.16e6;
       tx_gain          = 10;
       rx_gain          = 10;
     }},
    {"zmq_20MHz_n7",
     "Single 20MHz FDD in band n7 using ZMQ.",
     []() {
       driver_name      = "zmq";
       device_arguments = "";
       scs              = subcarrier_spacing::kHz15;
       sampling_rate_hz = 61.44e6;
       bw_rb            = 106;
       otw_format       = radio_configuration::over_the_wire_format::DEFAULT;
       dl_center_freq   = 2680.1e6;
       ssb_center_freq  = 2679.65e6;
       ssb_pattern      = ssb_pattern_case::A;

       if (zmq_loopback) {
         // Prepare ZMQ addresses using in-process communication instead of TCP. It avoids port collision and allows
         // parallel execution.
         for (unsigned channel_id = 0; channel_id != nof_ports * nof_sectors; ++channel_id) {
           fmt::memory_buffer buffer;
           fmt::format_to(buffer, "inproc://#{}", channel_id);
           tx_channel_args.emplace_back(to_string(buffer));
           rx_channel_args.emplace_back(to_string(buffer));
         }
       } else {
         // Prepare ZMQ addresses using TCP. Transmitter ports start at 5000+channel_id while receivers connect to
         // 6000+channel_id.
         unsigned port_base   = 5000;
         unsigned port_offset = 1000;
         for (unsigned channel_id = 0; channel_id != nof_ports * nof_sectors; ++channel_id) {
           tx_channel_args.emplace_back("tcp://*:" + std::to_string(port_base + channel_id));
           rx_channel_args.emplace_back("tcp://localhost:" + std::to_string(port_base + channel_id + port_offset));
         }
       }
     }},
    {"zmq_20MHz_n78",
     "Single 20MHz TDD in band n78 using ZMQ.",
     []() {
       driver_name      = "zmq";
       device_arguments = "";
       scs              = subcarrier_spacing::kHz30;
       sampling_rate_hz = 61.44e6;
       bw_rb            = 52;
       otw_format       = radio_configuration::over_the_wire_format::DEFAULT;
       dl_center_freq   = 3489.42e6;
       ssb_center_freq  = 3488.16e6;
       ssb_pattern      = ssb_pattern_case::C;

       if (zmq_loopback) {
         // Prepare ZMQ addresses using in-process communication instead of TCP. It avoids port collision and allows
         // parallel execution.
         for (unsigned channel_id = 0; channel_id != nof_ports * nof_sectors; ++channel_id) {
           fmt::memory_buffer buffer;
           fmt::format_to(buffer, "inproc://#{}", channel_id);
           tx_channel_args.emplace_back(to_string(buffer));
           rx_channel_args.emplace_back(to_string(buffer));
         }
       } else {
         // Prepare ZMQ addresses using TCP. Transmitter ports start at 5000+channel_id while receivers connect to
         // 6000+channel_id.
         unsigned port_base   = 5000;
         unsigned port_offset = 1000;
         for (unsigned channel_id = 0; channel_id != nof_ports * nof_sectors; ++channel_id) {
           tx_channel_args.emplace_back("tcp://*:" + std::to_string(port_base + channel_id));
           rx_channel_args.emplace_back("tcp://localhost:" + std::to_string(port_base + channel_id + port_offset));
         }
       }
     }},
};

// Global instances.
static std::mutex                             stop_execution_mutex;
static std::atomic<bool>                      stop      = {false};
static std::unique_ptr<lower_phy_controller>  lower_phy = nullptr;
static std::unique_ptr<radio_session>         radio     = nullptr;
static std::unique_ptr<upper_phy_ssb_example> upper_phy = nullptr;

static void stop_execution()
{
  // Make sure this function is not executed simultaneously.
  std::unique_lock<std::mutex> lock(stop_execution_mutex);

  // Skip if stop has already been signaled.
  if (stop) {
    return;
  }

  // Signal program to stop.
  stop = true;

  // Stop radio. It stops blocking the radio transmit and receive operations. The timing handler prevents the PHY from
  // free running.
  if (radio != nullptr) {
    radio->stop();
  }

  // Stop the timing handler. It stops blocking notifier and allows the PHY to free run.
  upper_phy->stop();

  // Stop PHY.
  if (lower_phy != nullptr) {
    lower_phy->stop();
  }
}

static void signal_handler(int sig)
{
  stop_execution();
}

static void usage(std::string prog)
{
  fmt::print("Usage: {} [-P profile] [-D duration] [-v level] [-o file name]\n", prog);
  fmt::print("\t-P Profile. [Default {}]\n", profiles.front().name);
  for (const configuration_profile& profile : profiles) {
    fmt::print("\t\t {:<30}{}\n", profile.name, profile.description);
  }
  fmt::print("\t-D Duration in slots. [Default {}]\n", duration_slots);
  fmt::print("\t-L Set ZMQ loopback. Set to 0 to disable, otherwise true [Default {}].\n", zmq_loopback);
  fmt::print("\t-v Logging level. [Default {}]\n", log_level);
  fmt::print("\t-h print this message.\n");
}

static void parse_args(int argc, char** argv)
{
  std::string profile_name;

  int opt = 0;
  while ((opt = getopt(argc, argv, "D:P:L:v:h")) != -1) {
    switch (opt) {
      case 'P':
        if (optarg != nullptr) {
          profile_name = std::string(optarg);
        }
        break;
      case 'D':
        if (optarg != nullptr) {
          duration_slots = std::strtol(optarg, nullptr, 10);
        }
        break;
      case 'L':
        if (optarg != nullptr) {
          zmq_loopback = (std::strtol(optarg, nullptr, 10) > 0);
        }
        break;
      case 'v':
        log_level = std::string(optarg);
        break;
      case 'h':
      default:
        usage(argv[0]);
        exit(-1);
    }
  }

  // Search profile if set.
  if (!profile_name.empty()) {
    bool found = false;
    for (const configuration_profile& profile : profiles) {
      if (strcmp(profile_name.c_str(), profile.name.c_str()) == 0) {
        profile.function();
        found = true;
        break;
      }
    }
    if (!found) {
      usage(argv[0]);
      srsran_terminate("Invalid profile {}.", profile_name);
    }
  }
}

static radio_configuration::radio create_radio_configuration()
{
  radio_configuration::radio radio_config = {};
  radio_config.sampling_rate_hz           = sampling_rate_hz;
  radio_config.otw_format                 = otw_format;
  radio_config.args                       = device_arguments;
  radio_config.log_level                  = log_level;
  for (unsigned sector_id = 0; sector_id != nof_sectors; ++sector_id) {
    // For each channel in the streams...
    radio_configuration::stream tx_stream_config;
    radio_configuration::stream rx_stream_config;
    for (unsigned port_id = 0; port_id != nof_ports; ++port_id) {
      // Create channel configuration and append it to the previous ones.
      radio_configuration::channel tx_ch_config;
      tx_ch_config.freq.center_frequency_hz = dl_center_freq;
      tx_ch_config.gain_dB                  = tx_gain;
      if (!tx_channel_args.empty()) {
        tx_ch_config.args = tx_channel_args[sector_id * nof_ports + port_id];
      }
      tx_stream_config.channels.emplace_back(tx_ch_config);

      radio_configuration::channel rx_ch_config;
      rx_ch_config.freq.center_frequency_hz = rx_freq;
      rx_ch_config.gain_dB                  = rx_gain;
      if (!rx_channel_args.empty()) {
        rx_ch_config.args = rx_channel_args[sector_id * nof_ports + port_id];
      }
      rx_stream_config.channels.emplace_back(rx_ch_config);
    }
    radio_config.tx_streams.emplace_back(tx_stream_config);
    radio_config.rx_streams.emplace_back(rx_stream_config);
  }
  return radio_config;
}

lower_phy_configuration create_lower_phy_configuration(unsigned                      dft_size_15kHz,
                                                       double                        rx_to_tx_delay,
                                                       float                         tx_scale,
                                                       lower_phy_rx_symbol_notifier* rx_symbol_notifier,
                                                       lower_phy_timing_notifier*    timing_notifier,
                                                       resource_grid_pool*           ul_resource_grid_pool)
{
  lower_phy_configuration phy_config;
  phy_config.dft_size_15kHz             = dft_size_15kHz;
  phy_config.numerology                 = to_numerology_value(scs);
  phy_config.max_processing_delay_slots = max_processing_delay_slots;
  phy_config.ul_to_dl_slot_offset       = ul_to_dl_slot_offset;
  phy_config.nof_dl_rg_buffers          = max_processing_delay_slots * 2;
  phy_config.rx_to_tx_delay             = rx_to_tx_delay;
  phy_config.tx_scale                   = tx_scale;
  phy_config.cp                         = cp;
  phy_config.bb_gateway                 = &radio->get_baseband_gateway();
  phy_config.rx_symbol_notifier         = rx_symbol_notifier;
  phy_config.timing_notifier            = timing_notifier;
  phy_config.ul_resource_grid_pool      = ul_resource_grid_pool;
  for (unsigned sector_id = 0; sector_id != nof_sectors; ++sector_id) {
    lower_phy_sector_description sector_config;
    sector_config.bandwidth_rb = bw_rb;
    sector_config.dl_freq_hz   = dl_center_freq;
    sector_config.ul_freq_hz   = rx_freq;
    for (unsigned port_id = 0; port_id < nof_ports; ++port_id) {
      lower_phy_sector_port_mapping port_mapping;
      port_mapping.stream_id  = sector_id;
      port_mapping.channel_id = port_id;
      sector_config.port_mapping.push_back(port_mapping);
    }
    phy_config.sectors.push_back(sector_config);
    phy_config.nof_channels_per_stream.push_back(nof_ports);
  }
  phy_config.log_level = log_level;

  return phy_config;
}

int main(int argc, char** argv)
{
  // Parse arguments.
  parse_args(argc, argv);

  // Make sure thread pool logging matches the test level.
  srslog::fetch_basic_logger("POOL").set_level(srslog::str_to_basic_level(log_level));

  // Derived parameters.
  unsigned dft_size_15kHz = static_cast<unsigned>(sampling_rate_hz / 15e3);
  float    tx_scale       = 1.0F / std::sqrt(NRE * bw_rb);
  double   rx_to_tx_delay = static_cast<double>(ul_to_dl_slot_offset * 1e-3) / pow2(to_numerology_value(scs));

  // Make sure parameters are valid.
  srsran_always_assert(std::remainder(sampling_rate_hz, 15e3) < 1e-3,
                       "Sampling rate ({:.3f} MHz) must be multiple of 15kHz.",
                       sampling_rate_hz / 1e6);
  srsran_always_assert(cp.is_valid(to_numerology_value(scs), dft_size_15kHz / pow2(to_numerology_value(scs))),
                       "The cyclic prefix ({}) numerology ({}) and sampling rate ({:.3f}) combination is invalid .",
                       cp.to_string(),
                       to_numerology_value(scs),
                       sampling_rate_hz);

  // Radio asynchronous task executor.
  task_worker                    async_task_worker("async_thread", nof_sectors + 1);
  std::unique_ptr<task_executor> async_task_executor = make_task_executor(async_task_worker);

  // Lower PHY RT task executor.
  task_worker                    rt_task_worker("phy_rt_thread", 1, false, os_thread_realtime_priority::MAX_PRIO);
  std::unique_ptr<task_executor> rt_task_executor = make_task_executor(rt_task_worker);

  // Create radio factory.
  std::unique_ptr<radio_factory> factory = create_radio_factory(driver_name);
  srsran_always_assert(factory, "Driver %s is not available.", driver_name.c_str());

  // Create radio configuration. Assume 1 sector per stream.
  radio_configuration::radio radio_config = create_radio_configuration();

  // Create notification handler
  radio_notifier_spy notification_handler(log_level);

  // Create radio.
  radio = factory->create(radio_config, *async_task_executor, notification_handler);
  srsran_assert(radio, "Failed to create radio.");

  // Create symbol handler.
  rx_symbol_handler_example rx_symbol_handler(log_level);

  // Create UL resource grid pool configuration.
  std::unique_ptr<resource_grid_pool> ul_rg_pool = nullptr;
  {
    resource_grid_pool_config rg_pool_config = {};
    rg_pool_config.nof_sectors               = 1;
    rg_pool_config.nof_slots                 = max_processing_delay_slots * 2;
    for (unsigned sector_id = 0; sector_id != rg_pool_config.nof_sectors; ++sector_id) {
      for (unsigned slot_id = 0; slot_id != rg_pool_config.nof_slots; ++slot_id) {
        rg_pool_config.grids.push_back(create_resource_grid(nof_ports, get_nsymb_per_slot(cp), bw_rb * NRE));
        srsran_always_assert(rg_pool_config.grids.back(), "Failed to create resource grid.");
      }
    }

    // Create UL resource grid pool.
    ul_rg_pool = create_resource_grid_pool(rg_pool_config);
    srsran_always_assert(ul_rg_pool, "Failed to create resource grid pool for UL");
  }

  // Create adapters.
  phy_rx_symbol_adapter  rx_symbol_adapter;
  phy_rg_gateway_adapter rg_gateway_adapter;
  phy_timing_adapter     timing_adapter;

  // Create lower physical layer.
  {
    lower_phy_configuration phy_config = create_lower_phy_configuration(
        dft_size_15kHz, rx_to_tx_delay, tx_scale, &rx_symbol_adapter, &timing_adapter, ul_rg_pool.get());
    lower_phy = create_lower_phy(phy_config);
    srsran_assert(lower_phy, "Failed to create lower physical layer.");
  }

  double scs_Hz = static_cast<double>(1000U * scs_to_khz(scs));

  // Frequency of PointA in Hz.
  double dl_pointA_freq_Hz = dl_center_freq - scs_Hz * NRE * bw_rb / 2;
  // Frequency of the lowest SS/PBCH block subcarrier.
  double ssb_lowest_freq_Hz = ssb_center_freq - (scs_Hz * NRE * SSB_BW_RB / 2);
  // SSB frequency from PointA to the lowest SS/PBCH block subcarrier in Hz.
  double ssb_offset_pointA_Hz = ssb_lowest_freq_Hz - dl_pointA_freq_Hz;
  // SSB frequency from PointA to the lowest SS/PBCH block subcarrier in 15kHz subcarriers.
  unsigned ssb_offset_pointA_subc_15kHz = static_cast<unsigned>(ssb_offset_pointA_Hz / 15e3);
  // SSB frequency from PointA to the lowest SS/PBCH block subcarrier in PRB of the SCS.
  unsigned ssb_offset_pointA_subc_rb = ssb_offset_pointA_subc_15kHz / (NRE * pow2(to_numerology_value(scs)));
  // Remainder SSB frequency from PointA to the lowest SS/PBCH block subcarrier in 15kHz subcarriers.
  unsigned ssb_subcarrier_offset_subc_15kHz =
      ssb_offset_pointA_subc_15kHz - ssb_offset_pointA_subc_rb * (NRE * pow2(to_numerology_value(scs)));

  upper_phy_ssb_example::configuration upper_phy_sample_config;
  upper_phy_sample_config.log_level                        = log_level;
  upper_phy_sample_config.max_nof_prb                      = bw_rb;
  upper_phy_sample_config.max_nof_ports                    = nof_ports;
  upper_phy_sample_config.rg_pool_size                     = 2 * max_processing_delay_slots;
  upper_phy_sample_config.ldpc_encoder_type                = "generic";
  upper_phy_sample_config.gateway                          = &rg_gateway_adapter;
  upper_phy_sample_config.ssb_config.phys_cell_id          = 500;
  upper_phy_sample_config.ssb_config.cp                    = cyclic_prefix::NORMAL;
  upper_phy_sample_config.ssb_config.period_ms             = 5;
  upper_phy_sample_config.ssb_config.beta_pss_dB           = 0.0;
  upper_phy_sample_config.ssb_config.ssb_idx               = {0};
  upper_phy_sample_config.ssb_config.L_max                 = 8;
  upper_phy_sample_config.ssb_config.ssb_subcarrier_offset = ssb_subcarrier_offset_subc_15kHz;
  upper_phy_sample_config.ssb_config.ssb_offset_pointA     = ssb_offset_pointA_subc_rb;
  upper_phy_sample_config.ssb_config.pattern_case          = ssb_pattern;
  upper_phy                                                = upper_phy_ssb_example::create(upper_phy_sample_config);
  srsran_assert(upper_phy, "Failed to create upper physical layer.");

  // Connect adapters.
  rx_symbol_adapter.connect(&rx_symbol_handler);
  timing_adapter.connect(upper_phy.get());
  rg_gateway_adapter.connect(lower_phy.get());

  // Set signal handler.
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);
  signal(SIGHUP, signal_handler);
  signal(SIGQUIT, signal_handler);
  signal(SIGKILL, signal_handler);

  // Start processing.
  lower_phy->start(*rt_task_executor);

  // Receive and transmit per block basis.
  for (unsigned slot_count = 0; slot_count != duration_slots && !stop; ++slot_count) {
    // Wait for PHY to detect a TTI boundary.
    upper_phy->wait_tti_boundary();
  }

  // Stop execution.
  stop_execution();

  // Stop workers.
  async_task_worker.stop();
  rt_task_worker.stop();

  // Prints radio notification summary (number of overflow, underflow and other events).
  notification_handler.print();

  // Avoids pending log messages before destruction starts.
  srslog::flush();

  return 0;
}

/// \endcond