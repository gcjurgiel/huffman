#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "huffman/huffman_string.h"
#include "huffman/huffman_node.h"
#include "huffman/huffman_count.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

void print_tree(struct huffman_node* node, int depth) {
  if (!node) { printf("\n%.*sNULL", depth, "||||||||||"); return; }
  printf("\n%.*s%c", depth, "|||||||||||", node->value);
  printf("\n%.*szero: ", depth, "|||||||||||");
  print_tree(node->zeroLeaf, depth + 1);
  printf("\n%.*sone: ", depth, "||||||||||||||");
  print_tree(node->oneLeaf, depth + 1);
}

int main(int argc, char** argv) {

  const char* input_file_path = "../../Comp.huff";
  const char* output_file_path = "../../Decomp.pdf";

  FILE* in_file_ptr = fopen(input_file_path, "r");
  if (!in_file_ptr) {
    printf("Error: File \"%s\" not found.\n", input_file_path);
    return -1;
  }

  struct huffman_node* tree = huffman_node_read(in_file_ptr);
 
  FILE* out_file_ptr = fopen(output_file_path, "w");
  if (!out_file_ptr) {
    printf("Error: File \"%s\" not found.\n", output_file_path);
    return -1;
  }

  char current_bits = getc(in_file_ptr), bit_count = 0;
  // printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(current_bits));
  // print_tree(tree, 0); 
  while (true) {
    struct huffman_node* current_node = tree;
    while (current_node->zeroLeaf && current_node->oneLeaf) {
      if ((current_bits & 0b10000000) == 0) {
        current_node = current_node->oneLeaf;
      } else {
        current_node = current_node->zeroLeaf;
      }

      current_bits = current_bits << 1;
      bit_count++; 

      if (bit_count == 8) {
        bit_count = 0;
        current_bits = getc(in_file_ptr);
        if (feof(in_file_ptr)) { printf("end of file"); goto end_of_decode; }
      }
    }
    putc(current_node->value, out_file_ptr);
    putchar(current_node->value);
  }
  end_of_decode:

  huffman_node_free(tree);
  fclose(in_file_ptr);
  fclose(out_file_ptr);

  return 0;
}