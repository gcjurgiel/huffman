#include "huffman_node.h"

struct huffman_node* huffman_node_alloc(struct huffman_node* zeroLeaf,
                                        struct huffman_node* oneLeaf,
                                        char value) {

  // Allocate struct
  struct huffman_node* node = malloc(sizeof(struct huffman_node));
  if (!node) { return NULL; }

  // Add child leaves
  if (zeroLeaf) {
    node->zeroLeaf = zeroLeaf;
    node->zeroChars = huffman_string_concat(zeroLeaf->zeroChars, zeroLeaf->oneChars);
  } else {
    node->zeroLeaf = NULL;
    node->zeroChars = huffman_string_alloc(1);
    huffman_string_push_back(node->zeroChars, value);
  }

  if (oneLeaf) {
    node->oneLeaf = oneLeaf;
    node->oneChars = huffman_string_concat(oneLeaf->zeroChars, oneLeaf->oneChars);
  } else {
    node->oneLeaf = NULL;
    node->oneChars = huffman_string_alloc(0);
  }

  // Set value
  node->value = value;

  // return node
  return node;
}

void huffman_node_free(struct huffman_node* node) {
  // Recursivly free child nodes.
  if (node->zeroLeaf) { free(node->zeroLeaf); }
  if (node->oneLeaf) { free(node->oneLeaf); }

  // Free strings.
  huffman_string_free(node->zeroChars);
  huffman_string_free(node->oneChars);

  // Free struct.
  free(node);
}

struct huffman_node* huffman_node_read(FILE* file) {
  // get next character.
  char parent_char = getc(file);

  // Check if null (ending) node.
  if (parent_char != 0) {

    // Read the two child nodes.
    struct huffman_node* zero_node = huffman_node_read(file);
    struct huffman_node* one_node = huffman_node_read(file);

    // Return the parent node.
    return huffman_node_alloc(zero_node, one_node, parent_char);
  }

  // Return null if the ending node.
  return NULL;
}

void huffman_node_write(struct huffman_node* tree, FILE* file) {
  // Check if this is the null (ending) node
  if (tree) {
    // Write this node
    putc(tree->value, file);

    // Write child nodes
    huffman_node_write(tree->zeroLeaf, file);
    huffman_node_write(tree->oneLeaf, file);
    return;
  }

  // write the null character to signify a null (ending) node
  putc(0, file);
  return;
}

void huffman_node_encoder(FILE* input, FILE* output, struct huffman_node* tree) {
  
}

void huffman_node_decode(FILE* input, FILE* output, struct huffman_node* tree) {

}