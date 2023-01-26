#include "huffman_string.h"

#include <stdbool.h>
#include <string.h>

struct huffman_string* huffman_string_alloc(size_t capacity) {
  // Allocat  and initialize object
  struct huffman_string* new_string = malloc(sizeof(struct huffman_string));
  if (!new_string) { return NULL; }
  new_string->length = 0;
  new_string->capacity = capacity;

  // Allocate and initailize data
  new_string->data = malloc(capacity + 1);
  if (!new_string->data) { free(new_string); return NULL; }
  *(new_string->data) = '\0';

  // return object
  return new_string;
}

struct huffman_string* huffman_string_copy(struct huffman_string* string) {
  // Allocat enew object 
  struct huffman_string* new_string = huffman_string_alloc(string->length);
  if (!new_string) { return NULL; }

  // Copy data
  memcpy(new_string->data, string->data, string->length + 1);
  new_string->length = string->length;

  // Return object
  return new_string;
}

void huffman_string_realloc(struct huffman_string* string, size_t new_capacity) {
  // Check cpacity
  if (string->capacity >= new_capacity) { return; }

  // Allocat more space
  string->data = realloc(string->data, new_capacity + 1);
  if (!string->data) { return; }
  string->capacity = new_capacity;
}

void huffman_string_free(struct huffman_string* string) {
  // Free data and object
  free(string->data);
  free(string);
}

char* huffman_string_ptr(struct huffman_string* string, size_t index) {
  // Bounds check
  if (string->length <= index) { return NULL; }
  return string->data + index;
}

char huffman_string_get(struct huffman_string* string, size_t index) {
  // Bounds check
  if (string->length <= index) { return '\0'; }
  return string->data[index];
}

void huffman_string_set(struct huffman_string* string, size_t index, char value) {
  // Bounds check
  if (string->length <= index) { return; }
  string->data[index] = value;
}

void huffman_string_push_back(struct huffman_string* string, char value) {
  // Capacity
  if (string->length == string->capacity) {
    huffman_string_realloc(string, 2 * string->capacity);
  }

  // Add data
  string->data[string->length++] = value;
  string->data[string->length] = '\0';
}

char huffman_string_pop_back(struct huffman_string* string) {
  return string->data[string->length--];
}

bool huffman_string_has_char(struct huffman_string* str, char val) {
  for (int i = 0; i < str->length; i++) {
    if (str->data[i] == val) { return true; }
  }

  return false;
}

void huffman_string_append(struct huffman_string* string, struct huffman_string* to_append) {
  // Add space
  huffman_string_realloc(string, string->length + to_append->length);

  // Copy data
  memcpy(string->data + string->length, to_append->data, to_append->length);
  string->length += to_append->length;
}

struct huffman_string* huffman_string_concat(struct huffman_string* str1, struct huffman_string* str2) {
  // Allocate space
  struct huffman_string* new_string = huffman_string_alloc(str1->length + str2->length);
  if (!new_string) { return NULL; }

  // Copy data over
  memcpy(new_string->data, str1->data, str1->length);
  memcpy(new_string->data + str1->length, str2->data, str2->length);
  new_string->length = str1->length + str2->length;

  // return new object
  return new_string;
}

struct huffman_string* huffman_string_from_c_str(char* c_str) {
  // Allocta espace
  size_t new_length = strlen(c_str);
  struct huffman_string* new_string = huffman_string_alloc(new_length);
  if (!new_string) { return NULL; }

  // Copy data
  memcpy(new_string->data, c_str, new_length);
  new_string->length = new_length;

  // Return object
  return new_string;
}

void huffman_string_append_c_str(struct huffman_string* string, char* c_str) {
  // Allocat espace
  size_t append_length = strlen(c_str);
  huffman_string_realloc(string, string->length + append_length);

  // Copy data over
  memcpy(string->data + string->length, c_str, append_length);
  string->length += append_length;
}