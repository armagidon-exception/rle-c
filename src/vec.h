#ifndef VECTOR_H
#define VECTOR_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  uint8_t *array;
  size_t size;
  size_t capacity;
} char_vector;

void vec_init(char_vector *vec, size_t capacity);

char_vector *new_vec(size_t initial_size);
void vec_destroy(char_vector *vec);

void vec_print(char_vector *vec);

void vec_push(char_vector *vec, int el);
int vec_pop(char_vector *vec, uint8_t *o);
uint8_t vec_peek(char_vector *vec);

int vec_get(char_vector *vec, size_t index);
void vec_clear(char_vector *vec);
void vec_rev(char_vector *vec);
char_vector *vec_copy(char_vector *vec);
int vec_fwrite(char_vector *vec, FILE *file);

#endif /* end of include guard: VECTOR_H */
