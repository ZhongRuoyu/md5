#ifndef MD5_H_
#define MD5_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t *MD5(const uint8_t *data, size_t len, unsigned char *md);

enum { kMD5DigestLength = 16 };
enum { kMD5StateSize = 128 };
enum { kMD5BlockSize = 512 };
struct MD5Context {
  uint32_t state[kMD5StateSize / 32];
  uint8_t block[kMD5BlockSize / 8];
  size_t length;
};
void MD5Init(struct MD5Context *ctx);
void MD5Update(struct MD5Context *ctx, const void *data, size_t len);
void MD5Final(uint8_t digest[], const struct MD5Context *ctx);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // MD5_H_
