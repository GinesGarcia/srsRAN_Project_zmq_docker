/*
 *
 * Copyright 2021-2023 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "srsran/srslog/srslog.h"
#include <fstream>
#include <string>
#include <thread>

#pragma once

/// \brief Check whether the CPU scaling governor is set to performance.
///
/// \param[in] logger to print warnings.
/// \return True if we were able to read the sysfs scaling governor information.
inline bool check_cpu_governor(srslog::basic_logger& logger)
{
  unsigned int n_cpus        = std::thread::hardware_concurrency();
  std::string  filename_base = "/sys/devices/system/cpu/cpu";
  for (unsigned int i = 0; i < n_cpus; ++i) {
    std::string   filename = filename_base + std::to_string(i) + "/cpufreq/scaling_governor";
    std::ifstream input(filename);
    if (input.fail()) {
      logger.warning("Could not check scaling governor. filename={} error=\"{}\"", filename, strerror(errno));
      return false;
    }
    std::string gov;
    std::getline(input, gov);
    if (input.fail()) {
      logger.warning("Could not check scaling governor. filename={} error=\"{}\"", filename, strerror(errno));
      return false;
    }
    if (gov == "performance") {
      logger.debug("CPU{} scaling governor is set to performance", i);
    } else {
      logger.warning(
          "CPU{} scaling governor is not set to performance, which may hinder performance. You can set it to "
          "performance using the "
          "\"srsran_performance\" script",
          i);
    }
  }
  return true;
}

/// \brief Check whether the DRM KMS polling is set.
///
/// \param[in] logger to print warnings.
/// \return True if we were able to read the sysfs for the DRM KMS polling information.
inline bool check_drm_kms_polling(srslog::basic_logger& logger)
{
  std::string   filename = "/sys/module/drm_kms_helper/parameters/poll";
  std::ifstream input(filename);
  if (input.fail()) {
    logger.warning("Could not check DRM KMS polling. filename={} error=\"{}\"", filename, strerror(errno));
    return false;
  }
  std::string polling;
  std::getline(input, polling);
  if (input.fail()) {
    logger.warning("Could not check DRM KMS polling. filename={} error=\"{}\"", filename, strerror(errno));
    return false;
  }
  if (polling == "N") {
    logger.debug("DRM KMS polling is disabled");
  } else {
    logger.warning("DRM KMS polling is enabled, which may hinder performance. You can disable it using the "
                   "\"srsran_performance\" script");
  }
  return true;
}