#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "md5.h"

enum { kBufsize = (1 << 20) /* 1 MiB */ };

static void PrintDigest(uint8_t digest[], size_t digest_length) {
  for (size_t i = 0; i < digest_length; ++i) {
    printf("%02x", digest[i]);
  }
  printf("\n");
}

int main() {
  struct MD5Context md5_ctx;
  MD5Init(&md5_ctx);

  uint8_t buffer[kBufsize];
  for (;;) {
    size_t len = fread(buffer, sizeof(uint8_t), kBufsize, stdin);
    if (len == 0) {
      if (feof(stdin)) {
        break;
      }
      if (ferror(stdin)) {
        perror("Error reading from input");
        break;
      }
      fprintf(stderr, "Unknown error");
    }

    MD5Update(&md5_ctx, buffer, len);
  }

  uint8_t md5_digest[kMD5DigestLength];
  MD5Final(md5_digest, &md5_ctx);
  PrintDigest(md5_digest, kMD5DigestLength);
}
