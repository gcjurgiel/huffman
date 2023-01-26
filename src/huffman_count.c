#include "huffman_count.h"

#include <stdbool.h>

// Counts linked list node for keeping the list sorted
struct huffman_count_list {
  bool head;
  struct huffman_count count;
  struct huffman_count_list* next;
};

// Allocate the head of the list
struct huffman_count_list* huffman_count_list_head() {
  struct huffman_count_list* list = malloc(sizeof(struct huffman_count_list));
  if (!list) { return NULL; }
  list->count.count = 0;
  list->count.node = NULL;
  list->head = true;
  list->next = NULL;
  return list;
}

// Allocate an entry to the list
struct huffman_count_list* huffman_count_list_alloc(struct huffman_count count) {
  struct huffman_count_list* list = malloc(sizeof(struct huffman_count_list));
  if (!list) { return NULL; }
  list->count = count;
  list->head = false;
  list->next = NULL;
  return list;
}

// inster element in list below
struct huffman_count_list* huffman_count_list_insert(struct huffman_count_list* below, struct huffman_count count) {
  // new entry object
  struct huffman_count_list* new_entry = huffman_count_list_alloc(count);

  // Pointer swap
  new_entry->next = below->next;
  below->next = new_entry;

  // Return newly made entry
  return new_entry;
}

// Build linked list from array
struct huffman_count_list* huffman_count_list_build(struct huffman_count* counts, size_t num) {
  // Allocate space
  if (num < 1) { return NULL; }
  struct huffman_count_list* head = huffman_count_list_head();

  // Interativly add to list
  struct huffman_count_list* prev = head;
  for (int i = 0; i < num; i++) {
    if (counts[i].count == 0) { continue; }
    prev = huffman_count_list_insert(prev, counts[i]);
  }

  // Return list
  return head;
}

// Sort count array with Quick Sort
void huffman_count_sort(struct huffman_count* counts, size_t first, size_t last) {
  if (first < last) {
    size_t i = first, j = last;
    struct huffman_count temp;
    while (i < j) {
      // Skip over sorted elements
      while (counts[i].count <= counts[first].count && i < last) { i++; }
      while (counts[j].count > counts[first].count) { j--; }

      // Swap around pivot
      if(i < j) {
        struct huffman_count temp = counts[i];
        counts[i]=counts[j];
        counts[j]=temp;
      }
    }

    // Final swap
    temp = counts[first];
    counts[first] = counts[j];
    counts[j] = temp;

    // Recur to the next level
    huffman_count_sort(counts, first, j - 1);
    huffman_count_sort(counts, j + 1, last);
  }
}

struct huffman_node* huffman_tree_from_counts(struct huffman_count* counts, size_t num) {
  // Get linked list
  huffman_count_sort(counts, 0, num - 1);
  struct huffman_count_list* list_head = huffman_count_list_build(counts, num);
 
  while (list_head->next && list_head->next->next) {
    // Combine top two element into tree
    struct huffman_count_list* zero = list_head->next;
    struct huffman_count_list* one = list_head->next->next;

    struct huffman_count new_count = {huffman_node_alloc(zero->count.node, one->count.node, 29), 
                                      zero->count.count + one->count.count};

    // Free top two list entries and remove them from the list
    list_head->next = list_head->next->next->next;
    free(zero);
    free(one);
    
    // Insert newly creted tree back into the sorted list
    struct huffman_count_list* prev = list_head;
    while (prev) {
      if (!prev->next || prev->next->count.count > new_count.count) {
        huffman_count_list_insert(prev, new_count);
        break;
      }
      prev = prev->next;
    }
  }
  
  if(!list_head->next) { return NULL; }

  // Return the final tree
  struct huffman_node* final_node = list_head->next->count.node;
  free(list_head->next);
  free(list_head);
  return final_node;
}