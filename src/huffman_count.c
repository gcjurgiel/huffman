#include "huffman_count.h"

#include <stdbool.h>

struct huffman_count_list {
  bool head;
  struct huffman_count count;
  struct huffman_count_list* next;
};

struct huffman_count_list* huffman_count_list_head() {
  struct huffman_count_list* list = malloc(sizeof(struct huffman_count_list));
  if (!list) { return NULL; }
  list->count.count = 0;
  list->count.node = NULL;
  list->head = true;
  list->next = NULL;
  return list;
}

struct huffman_count_list* huffman_count_list_alloc(struct huffman_count count) {
  struct huffman_count_list* list = malloc(sizeof(struct huffman_count_list));
  if (!list) { return NULL; }
  list->count = count;
  list->head = false;
  list->next = NULL;
  return list;
}

void huffman_count_list_free_all(struct huffman_count_list* list) {
  if (list) {
    huffman_count_list_free_all(list->next);
    free(list);
  }
}

struct huffman_count_list* huffman_count_list_insert(struct huffman_count_list* below, struct huffman_count count) {
  struct huffman_count_list* new_entry = huffman_count_list_alloc(count);
  new_entry->next = below->next;
  below->next = new_entry;
  return new_entry;
}

struct huffman_count_list* huffman_count_list_build(struct huffman_count* counts, size_t num) {
  if (num < 1) { return NULL; }
  struct huffman_count_list* head = huffman_count_list_head();
  struct huffman_count_list* prev = head;

  for (int i = 0; i < num; i++) {
    if (counts[i].count == 0) { continue; }
    prev = huffman_count_list_insert(prev, counts[i]);
  }

  return head;
}

void huffman_count_sort(struct huffman_count* counts, size_t first, size_t last) {
  if (first < last) {
    size_t i = first, j = last;
    struct huffman_count temp;
    while (i < j) {
      while (counts[i].count <= counts[first].count && i < last) { i++; }
      while (counts[j].count > counts[first].count) { j--; }

      if(i < j) {
        struct huffman_count temp = counts[i];
        counts[i]=counts[j];
        counts[j]=temp;
      }
    }

    temp = counts[first];
    counts[first] = counts[j];
    counts[j] = temp;

    huffman_count_sort(counts, first, j - 1);
    huffman_count_sort(counts, j + 1, last);
  }
}

struct huffman_node* huffman_tree_from_counts(struct huffman_count* counts, size_t num) {
  huffman_count_sort(counts, 0, num - 1);
  struct huffman_count_list* list_head = huffman_count_list_build(counts, num);

  while (list_head->next->next) {
    struct huffman_count_list* zero = list_head->next;
    struct huffman_count_list* one = list_head->next->next;
    list_head->next = list_head->next->next->next;

    struct huffman_count new_count = {huffman_node_alloc(zero->count.node, one->count.node, 29), 
                                      zero->count.count + one->count.count};

    free(zero);
    free(one);
    
    struct huffman_count_list* prev = list_head;
    while (prev) {
      if (!prev->next || prev->next->count.count > new_count.count) {
        huffman_count_list_insert(prev, new_count);
        break;
      }
      prev = prev->next;
    }
  }

  struct huffman_node* final_node = list_head->next->count.node;
  huffman_count_list_free_all(list_head);
  return final_node;
}