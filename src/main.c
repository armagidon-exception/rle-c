#include "encoder.h"
#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct opts_s {
  char *filename;
  char *output_name;
  bool decompress;
};

bool read_options(int argc, char *argv[], struct opts_s *opts) {
  opts->decompress = false;
  int o;
  while ((o = getopt(argc, argv, "r:do:")) != -1) {
    switch (o) {
    case 'd':
      opts->decompress = true;
      break;
    case 'o': {
      int l = strlen(optarg);
      char *cpy = malloc(l + 1);
      strcpy(cpy, optarg);
      opts->output_name = cpy;
      break;
    }
    default:
      return false;
    }
  }

  if (optind == argc) {
    fprintf(stderr, "Input file not specified\n");
    return false;
  }

  if (opts->output_name == NULL) {
    fprintf(stderr, "Output file not specified\n");
    return false;
  }

  return true;
}

int main(int argc, char *argv[]) {
  struct opts_s ops;
  if (!read_options(argc, argv, &ops))
    return -1;

  char *filename = argv[optind];
  FILE *file = fopen(filename, "r");
  FILE *new_file = fopen(ops.output_name, "w");
  if (!file) {
    fprintf(stderr, "File %s not found\n", filename);
    return -1;
  }

  if (!ops.decompress) {
   encoder_compress_to_stream(file, new_file);
    printf("Compressed to %s\n", ops.output_name);
  } else {
    encoder_decompress_to_stream(file, new_file);
    printf("Decompressed to %s\n", ops.output_name);
  }

  free(ops.output_name);
  fclose(new_file);
  fclose(file);
}
