#ifndef HUFFMAN_COUNT_H
#define HUFFMAN_COUNT_H

#include <stdlib.h>
#include <stdio.h>

#include "huffman_node.h"

struct huffman_count {
  struct huffman_node* node;
  size_t count;
};

void huffman_count_sort(struct huffman_count* counts, size_t start, size_t end);
struct huffman_node* huffman_tree_from_counts(struct huffman_count* counts, size_t num);

#endif