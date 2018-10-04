// Copyright (c) 2012-2013 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "cryptonote_config.h"

namespace nodetool
{
  const static boost::uuids::uuid CRYONOTE_NETWORK = { { 0x13, 0xf1, 0x1c, 0x3c, 0xeb, 0xbe, 0xef, 0x1a, 0xff, 0x31, 0x4a, 0x57, 0x66, 0x01, 0x01, 0xef } };
  const static boost::uuids::uuid CRYONOTE_TEST_NETWORK = { { 0xcc, 0xf7, 0x2c, 0x01, 0xd3, 0xf9, 0x12, 0xe4, 0xa3, 0x6b, 0xb8, 0xba, 0x56, 0x35, 0x9d, 0xfe } };

  inline boost::uuids::uuid network_id()
  {
    return cryptonote::config::testnet ? CRYONOTE_TEST_NETWORK : CRYONOTE_NETWORK;
  }
}
