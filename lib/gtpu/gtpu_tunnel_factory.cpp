/*
 *
 * Copyright 2013-2022 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#include "srsgnb/gtpu/gtpu_tunnel_factory.h"
#include "gtpu_tunnel_impl.h"

/// Notice this would be the only place were we include concrete class implementation files.

using namespace srsgnb;

std::unique_ptr<gtpu_tunnel> srsgnb::create_gtpu_tunnel(gtpu_tunnel_creation_message& msg)
{
  return std::make_unique<gtpu_tunnel_impl>(msg.cfg, *msg.rx_lower, *msg.tx_upper);
}