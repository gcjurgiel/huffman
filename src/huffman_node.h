#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H

#include <stdlib.h>
#include <stdio.h>

#include "huffman_string.h"

// Node of huffman tree
struct huffman_node {
  char value;

  // Zero side of node
  struct huffman_string* zeroChars;
  struct huffman_node* zeroLeaf;

  // One side of node
  struct huffman_string* oneChars;
  struct huffman_node* oneLeaf;
};

// Memory managment functions
struct huffman_node* huffman_node_alloc(struct huffman_node* zeroLeaf,
                                        struct huffman_node* oneLeaf,
                                        char value);

void huffman_node_free(struct huffman_node* node);

// File IO
struct huffman_node* huffman_node_read(FILE* file);
void huffman_node_write(struct huffman_node* tree, FILE* file);

#endif
