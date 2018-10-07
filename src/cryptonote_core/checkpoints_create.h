// Copyright (c) 2012-2013 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "include_base_utils.h"

#include "cryptonote_config.h"
#include "checkpoints.h"

#define ADD_CHECKPOINT(height, hash)  CHECK_AND_ASSERT(checkpoints.add_checkpoint(height, hash), false);

namespace cryptonote
{
  inline bool create_checkpoints(cryptonote::checkpoints& checkpoints)
  {
    if (cryptonote::config::testnet)
    {
      ADD_CHECKPOINT(0, "c72c922ba53c4364ab8fa7e3571163d84125874dbb70167da7c8e44d4992f8a1");
    }
    else
    {
    }

    return true;
  }
}
