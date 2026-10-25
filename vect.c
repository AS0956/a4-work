/**
 * Vector implementation.
 *
 * - Implement each of the functions to create a working growable array (vector).
 * - Do not change any of the structs
 * - When submitting, You should not have any 'printf' statements in your vector 
 *   functions.
 *
 * IMPORTANT: The initial capacity and the vector's growth factor should be 
 * expressed in terms of the configuration constants in vect.h
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "vect.h"

/** Main data structure for the vector. */
struct vect {
  char **data;             /* Array containing the actual data. */
  unsigned int size;       /* Number of items currently in the vector. */
  unsigned int capacity;   /* Maximum number of items the vector can hold before growing. */
};

/** Return a new copy of the given string. The caller must free it. */
char *copy_string(const char *str) {
  assert(str != NULL);

  char *copy = malloc(strlen(str) + 1);   /* +1 for the '\0' at the end */
  assert(copy != NULL);

  strcpy(copy, str);
  return copy;
}

/** Make the vector's array bigger by VECT_GROWTH_FACTOR. */
void vect_grow(vect_t *v) {
  assert(v != NULL);
  assert(v->capacity < VECT_MAX_CAPACITY);

  unsigned int new_capacity;
  if (v->capacity > VECT_MAX_CAPACITY / VECT_GROWTH_FACTOR) {
    new_capacity = VECT_MAX_CAPACITY;      /* growing more would go past the max */
  } else {
    new_capacity = v->capacity * VECT_GROWTH_FACTOR;
  }

  v->data = realloc(v->data, new_capacity * sizeof(char *));
  assert(v->data != NULL);
  v->capacity = new_capacity;
}

/** Construct a new empty vector. */
vect_t *vect_new() {
  vect_t *v = malloc(sizeof(vect_t));
  assert(v != NULL);

  v->data = calloc(VECT_INITIAL_CAPACITY, sizeof(char *));
  assert(v->data != NULL);

  v->size = 0;
  v->capacity = VECT_INITIAL_CAPACITY;

  return v;
}

/** Delete the vector, freeing all memory it occupies. */
void vect_delete(vect_t *v) {
  if (v == NULL) {
    return;
  }

  /* the vector owns its strings, so free each one first */
  for (unsigned int i = 0; i < v->size; i++) {
    free(v->data[i]);
  }

  free(v->data);
  free(v);
}

/** Get the element at the given index. */
const char *vect_get(vect_t *v, unsigned int idx) {
  assert(v != NULL);
  assert(idx < v->size);

  return v->data[idx];
}

/** Get a copy of the element at the given index. The caller is responsible
 *  for freeing the memory occupied by the copy. */
char *vect_get_copy(vect_t *v, unsigned int idx) {
  assert(v != NULL);
  assert(idx < v->size);

  return copy_string(v->data[idx]);
}

/** Set the element at the given index. */
void vect_set(vect_t *v, unsigned int idx, const char *elt) {
  assert(v != NULL);
  assert(idx < v->size);
  assert(elt != NULL);

  /* copy first, in case elt is the same string we are about to free */
  char *copy = copy_string(elt);
  free(v->data[idx]);
  v->data[idx] = copy;
}

/** Add an element to the back of the vector. */
void vect_add(vect_t *v, const char *elt) {
  assert(v != NULL);
  assert(elt != NULL);

  if (v->size == v->capacity) {
    vect_grow(v);
  }

  v->data[v->size] = copy_string(elt);
  v->size++;
}

/** Remove the last element from the vector. */
void vect_remove_last(vect_t *v) {
  assert(v != NULL);
  assert(v->size > 0);

  v->size--;
  free(v->data[v->size]);
  v->data[v->size] = NULL;
}

/** The number of items currently in the vector. */
unsigned int vect_size(vect_t *v) {
  assert(v != NULL);

  return v->size;
}

/** The maximum number of items the vector can hold before it has to grow. */
unsigned int vect_current_capacity(vect_t *v) {
  assert(v != NULL);

  return v->capacity;
}
