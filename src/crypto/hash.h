// Copyright (c) 2012-2013 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <stddef.h>

#include "common/types.h"
#include "generic-ops.h"
#include "misc_log_ex.h"

#include "packing.h"

#include <boost/program_options/variables_map.hpp>

namespace crypto
{
  extern "C"
  {
#include "crypto_core/hash-ops.h"
  }

  PACK(POD_CLASS hash
  {
    char data[HASH_SIZE];
  })

  static_assert(sizeof(hash) == HASH_SIZE, "Invalid structure size");

  /*
    CryptoNight hash functions
  */
  void cn_fast_hash(const void *data, std::size_t length, hash &hash);
  hash cn_fast_hash(const void *data, std::size_t length);
  void cn_slow_hash(const void *data, std::size_t length, hash &hash);

  void tree_hash(const hash *hashes, std::size_t count, hash &root_hash);
}

CRYPTO_MAKE_HASHABLE(hash)
