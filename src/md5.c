#include "md5.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "md5_impl.h"

static const uint32_t kMD5IV[kMD5StateSize / 32] = {
    0x67452301,
    0xefcdab89,
    0x98badcfe,
    0x10325476,
};

uint8_t *MD5(const uint8_t *data, size_t len, unsigned char *md) {
  static uint8_t digest[kMD5DigestLength];

  struct MD5Context ctx;
  MD5Init(&ctx);
  MD5Update(&ctx, data, len);
  uint8_t *ret = (md != NULL) ? md : digest;
  MD5Final(ret, &ctx);
  return ret;
}

void MD5Init(struct MD5Context *ctx) {
  memcpy(ctx->state, kMD5IV, sizeof(kMD5IV));
  ctx->length = 0;
}

void MD5Update(struct MD5Context *ctx, const void *data, size_t len) {
  for (size_t i = 0; i < len; ++i) {
    ctx->block[ctx->length % (kMD5BlockSize / 8)] = ((const uint8_t *)data)[i];
    ++ctx->length;
    if (ctx->length % (kMD5BlockSize / 8) == 0) {
      MD5Compress(ctx->state, ctx->block);
    }
  }
}

void MD5Final(uint8_t digest[], const struct MD5Context *ctx) {
  uint8_t padded_buffer[2 * (kMD5BlockSize / 8)];
  size_t original_length = ctx->length % (kMD5BlockSize / 8);
  memcpy(padded_buffer, ctx->block, original_length);

  size_t padding_length =
      MD5Padding(padded_buffer + original_length, ctx->length);
  size_t padded_length = original_length + padding_length;
  assert(padded_length == kMD5BlockSize / 8 ||
         padded_length == 2ULL * (kMD5BlockSize / 8));

  uint32_t state[kMD5StateSize / 32];
  memcpy(state, ctx->state, sizeof(state));
  for (size_t offset = 0; offset < padded_length;
       offset += (kMD5BlockSize / 8)) {
    MD5Compress(state, padded_buffer + offset);
  }

  for (size_t i = 0; i < kMD5DigestLength / 4; ++i) {
    digest[4 * i] = state[i] & 0xff;
    digest[4 * i + 1] = (state[i] >> 8) & 0xff;
    digest[4 * i + 2] = (state[i] >> 16) & 0xff;
    digest[4 * i + 3] = (state[i] >> 24) & 0xff;
  }
}
