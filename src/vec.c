#include "vec.h"
#include <stdint.h>

void vec_init(char_vector *vec, size_t capacity) {
  vec->size = 0;
  vec->array = (uint8_t *)calloc(capacity, sizeof(int));
  vec->capacity = capacity;
}

char_vector *new_vec(size_t initial_size) {
  char_vector *v = (char_vector *)malloc(sizeof(char_vector));
  vec_init(v, initial_size);
  return v;
}
void vec_destroy(char_vector *vec) {
  free(vec->array);
  free(vec);
}

void vec_print(char_vector *vec) {
  for (int i = 0; i < vec->size; i++)
    printf("%d ", vec->array[i]);
  printf("\n");
}

void vec_push(char_vector *vec, int el) {
  if (vec->size + 1 >= vec->capacity) {
    vec->array = (uint8_t *)reallocarray(vec->array, vec->capacity * 2, 1);
    vec->capacity *= 2;
    if (vec->array == NULL) {
      perror("Failed to push value");
      return;
    }
  }
  vec->array[vec->size++] = el;
}
int vec_pop(char_vector *vec, uint8_t *o) {
  if (!vec->size)
    return false;
  uint8_t res = vec->array[vec->size - 1];
  if (o)
    *o = res;
  vec->size--;

  return true;
}

int vec_get(char_vector *vec, size_t index) {
  if (vec->size <= index) {
    assert("Index out of bounds");
    return -1;
  }
  return vec->array[index];
}
void vec_clear(char_vector *vec) { vec->size = 0; }

void vec_rev(char_vector *vec) {
  for (int i = 0; i < (vec->size + 1) / 2; i++) {
    int temp = vec->array[i];
    vec->array[i] = vec->array[vec->size - 1 - i];
    vec->array[vec->size - 1 - i] = temp;
  }
}

char_vector *vec_copy(char_vector *vec) {
  char_vector *o = new_vec(vec->capacity);
  o->capacity = vec->capacity;
  o->size = vec->size;
  for (int i = 0; i < vec->size; i++) {
    o->array[i] = vec->array[i];
  }
  return o;
}

int vec_fwrite(char_vector *vec, FILE *file) {
  return fwrite(vec->array, 1, vec->size, file);
}
