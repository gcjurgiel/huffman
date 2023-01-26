#ifndef HUFFMAN_COUNT_H
#define HUFFMAN_COUNT_H

#include <stdlib.h>
#include <stdio.h>

#include "huffman_node.h"

// Keeps track of character counts while building huffman tree
struct huffman_count {
  struct huffman_node* node;
  size_t count;
};

// Sort for least used characters
void huffman_count_sort(struct huffman_count* counts, size_t start, size_t end);

// build tree
struct huffman_node* huffman_tree_from_counts(struct huffman_count* counts, size_t num);

#endif