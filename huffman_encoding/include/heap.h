#ifndef _HEAP_H_
#define _HEAP_H_

#include "huffman.h"

/* Provides implementation of min heap */

typedef struct heap_s {
    int capacity;
    int size;
    tree **nodes;
} heap;

heap *init_heap(int capacity);
void destroy_heap(heap *root);
tree *poll_heap(heap *root);
tree *remove_from_heap(heap *root);
void insert_into_heap(heap *root, tree *node);
void heapify (heap *p, int i);

int get_parent(int index);
int get_child_left (int index);
int get_child_right (int index);

#ifdef DEBUG
    void print_heap(heap *root);
#endif /* DEBUG */

#endif /* _HEAP_H_ */