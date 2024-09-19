#ifndef ENCODER_H
#define ENCODER_H

#include <stdio.h>

int encoder_compress_to_stream(FILE *input, FILE *output);

int encoder_decompress_to_stream(FILE *input, FILE *output);

#endif /* end of include guard: ENCODER_H */
