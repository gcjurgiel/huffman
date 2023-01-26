#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "huffman/huffman_string.h"
#include "huffman/huffman_node.h"
#include "huffman/huffman_count.h"


int main(int argc, char** argv) {

  const char* input_file_path = "../../Plato.pdf";
  const char* output_file_path = "../../Comp.huff";

  FILE* in_file_ptr = fopen(input_file_path, "r");
  if (!in_file_ptr) {
    printf("Error: File \"%s\" not found.\n", input_file_path);
    return -1;
  }

  struct huffman_count counts[256];
  for (int i = 0; i < 256; i++) {
    counts[i].node = huffman_node_alloc(NULL, NULL, i);
    counts[i].count = 0;
  }
  
  char current = getc(in_file_ptr);
  while (current != EOF){
    if (counts[current].count == SIZE_MAX) {
      printf("Error: Character counts overflowed\n");
      return -1;
    }
    counts[current].count++;
    current = getc(in_file_ptr);
  }

  struct huffman_node* tree = huffman_tree_from_counts(counts, 256);

  FILE* out_file_ptr = fopen(output_file_path, "w");
  if (!out_file_ptr) {
    printf("Error: File \"%s\" not found.\n", output_file_path);
    return -1;
  }

  huffman_node_write(tree, out_file_ptr);

  char bits = 0, bit_count = 0;
  rewind(in_file_ptr);
  char current_char = getc(in_file_ptr);
  while (current_char != EOF) {
    struct huffman_node* current_node = tree;
    while(current_node->zeroLeaf || current_node->oneLeaf) {
      bits = bits << 1;
      bit_count++;
      
      if (huffman_string_has_char(current_node->zeroChars, current_char)) {
        current_node = current_node->zeroLeaf;
      } else {
        bits = bits | 1;
        current_node = current_node->oneLeaf;
      }

      if (bit_count == 8) {
        putc(bits, out_file_ptr);
        bits = 0;
        bit_count = 0;
      }
    }
    current_char = getc(in_file_ptr);
  }

  putc(bits, out_file_ptr);
  bits = 0;
  bit_count = 0;

  huffman_node_free(tree);
  fclose(in_file_ptr);
  fclose(out_file_ptr);

  return 0;
}