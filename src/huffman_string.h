#ifndef HUFFMAN_STRING_H
#define HUFFMAN_STRING_H

#include <stdlib.h>
#include <stdbool.h>

// String object to manage memory
struct huffman_string {
  char* data;
  size_t length;
  size_t capacity;
};

// Memory related functions
struct huffman_string* huffman_string_alloc(size_t cap);
struct huffman_string* huffman_string_copy(struct huffman_string* str);
void huffman_string_realloc(struct huffman_string* str, size_t cap);
void huffman_string_free(struct huffman_string* str);

// Interation related functions
char* huffman_string_ptr(struct huffman_string* str, size_t i);
char huffman_string_get(struct huffman_string* str, size_t i);
void huffman_string_set(struct huffman_string* str, size_t i, char val);

// Check for character
bool huffman_string_has_char(struct huffman_string* str, char val);

// Stack functions
void huffman_string_push_back(struct huffman_string* str, char val);
char huffman_string_pop_back(struct huffman_string* str);

// Concatination functions
void huffman_string_append(struct huffman_string* str, struct huffman_string* app);
struct huffman_string* huffman_string_concat(struct huffman_string* str1, struct huffman_string* str2);

// C string interoperability largly for testing
struct huffman_string* huffman_string_from_c_str(char* c_str);
void huffman_string_append_c_str(struct huffman_string* str, char* c_str);

#endif