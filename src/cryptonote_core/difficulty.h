// Copyright (c) 2012-2013 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include <cstdint>
#include <vector>

#include "crypto/hash.h"
#include "cryptonote_config.h"

namespace cryptonote
{
  typedef std::uint64_t difficulty_type;

  bool check_hash(const crypto::hash &hash, difficulty_type difficulty);
  difficulty_type next_difficulty(uint64_t height, std::vector<uint64_t> timestamps,
    std::vector<difficulty_type> cumulative_difficulties, size_t target_seconds = cryptonote::config::difficulty_target());

  difficulty_type next_difficulty_v1(std::vector<std::uint64_t> timestamps, std::vector<difficulty_type> cumulative_difficulties, size_t target_seconds);
}
