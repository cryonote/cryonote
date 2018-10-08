#pragma once

#include <stdint.h>

typedef struct
{
  uint32_t h[8], s[4], t[2];
  int buflen, nullt;
  uint8_t buf[64];
} state;

typedef struct
{
  state inner;
  state outer;
} hmac_state;

void blake256_init(state *S);
void blake256_update(state *S, const uint8_t *data, uint64_t datalen);
void blake256_final(state *S, uint8_t *digest);
void blake256_hash(uint8_t *out, const uint8_t *in, uint64_t inlen);
