// Copyright (c) 2014-2015 The Pebblecoin developers
// Copyright (c) 2012-2013 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

#include <boost/algorithm/clamp.hpp>

#include "common/int-util.h"
#include "crypto/hash.h"
#include "difficulty.h"

namespace cryptonote
{
  using std::size_t;
  using std::uint64_t;
  using std::vector;

#if defined(_MSC_VER)
#include <windows.h>
#include <winnt.h>

  static inline void mul(uint64_t a, uint64_t b, uint64_t &low, uint64_t &high) {
    low = mul128(a, b, &high);
  }

#else

  static inline void mul(uint64_t a, uint64_t b, uint64_t &low, uint64_t &high) {
    typedef unsigned __int128 uint128_t;
    uint128_t res = (uint128_t) a * (uint128_t) b;
    low = (uint64_t) res;
    high = (uint64_t) (res >> 64);
  }

#endif

  static inline bool cadd(uint64_t a, uint64_t b) {
    return a + b < a;
  }

  static inline bool cadc(uint64_t a, uint64_t b, bool c) {
    return a + b < a || (c && a + b == (uint64_t) -1);
  }

  bool check_hash(const crypto::hash &hash, difficulty_type difficulty) {
    uint64_t low, high, top, cur;
    // First check the highest word, this will most likely fail for a random hash.
    mul(swap64le(((const uint64_t *) &hash)[3]), difficulty, top, high);
    if (high != 0) {
      return false;
    }
    mul(swap64le(((const uint64_t *) &hash)[0]), difficulty, low, cur);
    mul(swap64le(((const uint64_t *) &hash)[1]), difficulty, low, high);
    bool carry = cadd(cur, low);
    cur = high;
    mul(swap64le(((const uint64_t *) &hash)[2]), difficulty, low, high);
    carry = cadc(cur, low, carry);
    carry = cadc(high, top, carry);
    return !carry;
  }

  difficulty_type next_difficulty_v1(std::vector<std::uint64_t> timestamps,
    std::vector<difficulty_type> cumulative_difficulties, size_t target_seconds)
  {
    if (timestamps.size() > DIFFICULTY_WINDOW)
    {
      timestamps.resize(DIFFICULTY_WINDOW);
      cumulative_difficulties.resize(DIFFICULTY_WINDOW);
    }

    size_t length = timestamps.size();
    assert(length == cumulative_difficulties.size());
    if (length <= 1)
    {
      return 1;
    }

    uint64_t weighted_timespans = 0;
    for (size_t i = 1; i < length; i++)
    {
      uint64_t timespan;
      if (timestamps[i - 1] >= timestamps[i])
      {
        timespan = 1;
      }else
      {
        timespan = timestamps[i] - timestamps[i - 1];
      }
      if (timespan > 10 * target_seconds)
      {
        timespan = 10 * target_seconds;
      }
      weighted_timespans += i * timespan;
    }

    // N = length - 1
    uint64_t minimum_timespan = target_seconds * (length - 1) / 2;
    if (weighted_timespans < minimum_timespan)
    {
      weighted_timespans = minimum_timespan;
    }

    difficulty_type total_work = cumulative_difficulties.back() - cumulative_difficulties.front();
    assert(total_work > 0);

    uint64_t low, high;
    // adjust = 0.99 for N=60 ; length = N + 1
    uint64_t target = 99 * (length / 2) * target_seconds / 100;
    mul(total_work, target, low, high);
    if (high != 0)
    {
      return 0;
    }
    return low / weighted_timespans;
  }

  difficulty_type next_difficulty(uint64_t height, std::vector<uint64_t> timestamps,
    std::vector<difficulty_type> cumulative_difficulties, size_t target_seconds/*=cryptonote::config::difficulty_target()*/)
  {
    return next_difficulty_v1(std::move(timestamps), std::move(cumulative_difficulties), target_seconds);
  }
}
