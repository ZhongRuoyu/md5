#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "md5.h"

enum { kBufsize = (1 << 20) /* 1 MiB */ };

static void PrintDigest(uint8_t digest[], size_t digest_length,
                        const char *filename) {
  for (size_t i = 0; i < digest_length; ++i) {
    printf("%02x", digest[i]);
  }
  if (filename != NULL) {
    printf("  %s", filename);
  }
  printf("\n");
}

static void ProcessFile(const char *filename) {
  FILE *file;
  const char *effective_filename;
  if (strcmp(filename, "-") == 0) {
    file = stdin;
    effective_filename = "stdin";
  } else {
    file = fopen(filename, "rb");
    effective_filename = filename;
    if (file == NULL) {
      fprintf(stderr, "Error opening %s: %s\n", effective_filename,
              // NOLINTNEXTLINE(concurrency-mt-unsafe)
              strerror(errno));
      return;
    }
  }

  struct MD5Context md5_ctx;
  MD5Init(&md5_ctx);

  uint8_t buffer[kBufsize];
  for (;;) {
    size_t len = fread(buffer, sizeof(uint8_t), kBufsize, file);
    if (len == 0) {
      if (feof(file)) {
        break;
      }
      if (ferror(file)) {
        fprintf(stderr, "Error reading from %s: %s\n", effective_filename,
                // NOLINTNEXTLINE(concurrency-mt-unsafe)
                strerror(errno));
        goto cleanup;
      }
      fprintf(stderr, "Unknown error processing %s", effective_filename);
      goto cleanup;
    }

    MD5Update(&md5_ctx, buffer, len);
  }

  uint8_t md5_digest[kMD5DigestLength];
  MD5Final(md5_digest, &md5_ctx);
  PrintDigest(md5_digest, kMD5DigestLength, filename);

cleanup:
  if (file != stdin) {
    fclose(file);
  }
}

int main(int argc, char *argv[]) {
  if (argc > 1) {
    for (int i = 1; i < argc; ++i) {
      ProcessFile(argv[i]);
    }
  } else {
    ProcessFile("-");
  }
}
