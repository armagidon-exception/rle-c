#include "encoder.h"
#include "vec.h"
#include <stdint.h>

struct file_iterator_s {
  uint8_t buf[BUFSIZ];
  FILE *file;
  size_t cursor;
  size_t count;
};

static int next_byte(struct file_iterator_s *iter, uint8_t *byte) {
  if (iter->cursor >= iter->count) {
    if (!feof(iter->file) && !ferror(iter->file)) {
      iter->count = fread(iter->buf, 1, BUFSIZ, iter->file);
      iter->cursor = 0;
    } else {
      return 1;
    }
  }
  *byte = iter->buf[iter->cursor++];
  return 0;
}

static void find_rarest_byte(FILE *file, uint8_t *minByte) {
  uint8_t *table = calloc(256, 1);

  uint8_t buf[BUFSIZ];
  while (!feof(file) && !ferror(file)) {
    size_t count = fread(buf, 1, BUFSIZ, file);
    for (int i = 0; i < count; i++) {
      uint8_t byte = buf[i];
      table[byte]++;
    }
  }

  int minI = 0;
  size_t min = INT64_MAX;
  for (int i = 0; i < 256; i++) {
    if (table[i] < min) {
      minI = i;
      min = table[i];
    }
  }
  *minByte = minI;

  free(table);
}

static void handle_repeating_char(FILE *output, size_t repCount,
                                  uint8_t minByte, uint8_t prev) {
  if (repCount > 3 || prev == minByte) {
    size_t t = repCount / 255;
    uint8_t wbuf[3] = {minByte, 255, prev};
    for (int i = 0; i < t; i++) {
      fwrite(wbuf, 1, 3, output);
    }
    wbuf[1] = repCount % 255;
    fwrite(wbuf, 1, 3, output);
  } else {
    for (int i = 0; i < repCount; i++) {
      fwrite(&prev, 1, 1, output);
    }
  }
}

int encoder_compress_to_stream(FILE *input, FILE *output) {
  uint8_t minByte;
  find_rarest_byte(input, &minByte);
  rewind(input);

  struct file_iterator_s iter = {.file = input, .count = 0, .cursor = 0};
  uint8_t prev, current;
  if (next_byte(&iter, &prev)) {
    return 0;
  } else if (iter.count == 1) {
    fwrite(&minByte, 1, 1, output);
    fwrite(&prev, 1, 1, output);
    return 0;
  }

  fwrite(&minByte, 1, 1, output);
  bool rep = false;
  size_t repCount = 0;
  char_vector *nonrep = new_vec(BUFSIZ);

  vec_push(nonrep, prev);

  while (!next_byte(&iter, &current)) {
    if (prev == current) {
      if (!rep) {
        repCount = 2;
        rep = true;
        vec_pop(nonrep, 0);
        vec_fwrite(nonrep, output);
        vec_clear(nonrep);
      } else {
        repCount++;
      }
    } else {
      if (rep) {
        handle_repeating_char(output, repCount, minByte, prev);
        rep = false;
      }

      vec_push(nonrep, current);
    }
    prev = current;
  }

  if (rep) {
    handle_repeating_char(output, repCount, minByte, prev);
  } else {
    vec_fwrite(nonrep, output);
  }

  vec_destroy(nonrep);
  return 0;
}

int encoder_decompress_to_stream(FILE *input, FILE *output) {
  struct file_iterator_s iter = {.file = input, .count = 0, .cursor = 0};
  uint8_t minByte;
  if (next_byte(&iter, &minByte)) {
    fprintf(stderr, "Malformed archive: could not read min byte");
    return 1;
  }
  uint8_t current;
  while (!next_byte(&iter, &current)) {
    if (current != minByte) {
      fwrite(&current, 1, 1, output);
    } else {
      uint8_t count, byte;
      if (next_byte(&iter, &count)) {
        fprintf(stderr, "Malformed archive: could not read count of byte");
        return 1;
      }
      if (next_byte(&iter, &byte)) {
        fprintf(stderr, "Malformed archive: could not read repeated byte");
        return 1;
      }
      uint8_t *outputBuf = malloc(count);
      memset(outputBuf, byte, count);
      fwrite(outputBuf, 1, count, output);
      free(outputBuf);
    }
  }
  return 0;
}
