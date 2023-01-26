#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "huffman_string.h"
#include "huffman_node.h"
#include "huffman_count.h"

int compress(char* input_file_path, char* output_file_path);
int decompress(char* input_file_path, char* output_file_path);

int main(int argc, char** argv) {
  // Help message
  if (argc < 2 || strcmp("-h", argv[1]) == 0 || 
                  strcmp("--help", argv[1]) == 0 || 
                  strcmp("-?", argv[1]) == 0) {
    
    printf("OVERVIEW: huffman file compresser\n\n"
           "USAGE: huffman [options] input_file output_file\n\n"
           "OPTIONS:\n"
           "-x extract/decompress an already compressed file\n"
           "-c compress a text file\n\n");

    return 0;
  }

  if (argc == 4 && strcmp("-c", argv[1]) == 0) {
    return compress(argv[2], argv[3]);
  } else if (argc == 4 && strcmp("-x", argv[1]) == 0) {
    return decompress(argv[2], argv[3]);
  } else {
    printf("huffman: error: Invalid argumants.\n"
           "run `huffman --help` for help\n");
    return -1;
  }
}

int compress(char* input_file_path, char* output_file_path) {
  
  // Open Input file
  FILE* in_file_ptr = fopen(input_file_path, "r");
  if (!in_file_ptr) {
    printf("Error: File \"%s\" not found.\n", input_file_path);
    return -1;
  }

  // Allocate space for counts
  struct huffman_count counts[256];
  for (int i = 0; i < 256; i++) {
    counts[i].node = huffman_node_alloc(NULL, NULL, i);
    counts[i].count = 0;
  }

  // Add end of file character
  counts[255].count = 1;
  
  // Count characters
  char current = getc(in_file_ptr);
  while (!feof(in_file_ptr)){
    counts[current].count++;
    current = getc(in_file_ptr);
  }

  // Build tree from counts
  struct huffman_node* tree = huffman_tree_from_counts(counts, 256);

  // Check for tree
  if (!tree) {
    printf("File is blank. Theres Noithign to Compress :)\n");
    fclose(in_file_ptr);
    return 0;
  }

  // Open output file
  FILE* out_file_ptr = fopen(output_file_path, "w");
  if (!out_file_ptr) {
    printf("Error: File \"%s\" not found.\n", output_file_path);
    return -1;
  }
 
  // Write tree
  huffman_node_write(tree, out_file_ptr);

  // rewind input file for compressing
  rewind(in_file_ptr);

  // keep track of the current character
  char current_char;
  
  // keep track of bits wrriten into byte
  char bits = 0, bit_count = 0;

  // Uses do while loop to over read by one allsoing for the endo of file byte to be written.
  do {
    // Get next char
    current_char = getc(in_file_ptr);

    // Resets current node to the head, and checks if leaf node
    struct huffman_node* current_node = tree;
    while(current_node->zeroLeaf && current_node->oneLeaf) {
      
      // Traverse tree
      if (huffman_string_has_char(current_node->zeroChars, current_char)) {
        current_node = current_node->zeroLeaf;
      } else {
        // Write bit if one side
        bits = bits | 1;
        current_node = current_node->oneLeaf;
      }

      // Shift to next bit
      bits = bits << 1;
      bit_count++;

      // Write byte if full and reset bits
      if (bit_count == 7) {
        putc(bits, out_file_ptr);
        bits = 0;
        bit_count = 0;
      }
    }
  } while (!feof(in_file_ptr));

  // Write the final byte
  putc(bits << (7 - bit_count), out_file_ptr);

  // Free meory and files
  huffman_node_free(tree);
  fclose(in_file_ptr);
  fclose(out_file_ptr);

  return 0;
}

int decompress(char* input_file_path, char* output_file_path) {
 
  // Open input file
  FILE* in_file_ptr = fopen(input_file_path, "r");
  if (!in_file_ptr) {
    printf("Error: File \"%s\" not found.\n", input_file_path);
    return -1;
  }
  
  // Read huffman tree
  struct huffman_node* tree = huffman_node_read(in_file_ptr);
 
  // Open output file
  FILE* out_file_ptr = fopen(output_file_path, "w");
  if (!out_file_ptr) {
    printf("Error: File \"%s\" not found.\n", output_file_path);
    return -1;
  }

  // keep track of byte and bit count
  char current_bits = getc(in_file_ptr), bit_count = 0;
  while (!feof(in_file_ptr)) {

    // Reset current node to head and check for leafe node
    struct huffman_node* current_node = tree;
    while (current_node->zeroLeaf && current_node->oneLeaf) {
      // Check current bit and follow proper leg of the tree
      if (current_bits & 0b10000000) {
        current_node = current_node->oneLeaf;
      } else {
        current_node = current_node->zeroLeaf;
      }

      // Shift to the next bit
      current_bits = current_bits << 1;
      bit_count++; 

      // If all the bits in the current byte are exausted get the next one.
      if (bit_count == 7) {
        bit_count = 0;
        current_bits = getc(in_file_ptr);
        // Check for end of file
        if (feof(in_file_ptr)) { break; }
      }
    }

    // Catches end of file
    if (current_node->value == -1) { break; }

    // After reaching leaf node, write chracter
    putc(current_node->value, out_file_ptr);
  }

  // free memory and files
  huffman_node_free(tree);
  fclose(in_file_ptr);
  fclose(out_file_ptr);

  return 0;
}