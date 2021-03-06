// Copyright (c) 2018, The CryoNote Developers.
// Portions Copyright (c) 2012-2013, The CryptoNote Developers.
//
// All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "hash.h"

namespace crypto
{
  void cn_fast_hash(const void *data, std::size_t length, hash &hash)
  {
    cn_fast_hash(data, length, reinterpret_cast<char *>(&hash));
  }

  hash cn_fast_hash(const void *data, std::size_t length)
  {
    hash h;
    cn_fast_hash(data, length, reinterpret_cast<char *>(&h));
    return h;
  }

  void cn_slow_hash(const void *data, std::size_t length, hash &hash)
  {
    cn_slow_hash(data, length, reinterpret_cast<char *>(&hash));
  }

  void tree_hash(const hash *hashes, std::size_t count, hash &root_hash)
  {
    tree_hash(reinterpret_cast<const char (*)[HASH_SIZE]>(hashes), count, reinterpret_cast<char *>(&root_hash));
  }
}
