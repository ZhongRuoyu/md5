#ifndef MD5_IMPL_H_
#define MD5_IMPL_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void MD5Compress(uint32_t state[], const uint8_t block[]);
size_t MD5Padding(uint8_t output[], size_t message_length);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // MD5_IMPL_H_
