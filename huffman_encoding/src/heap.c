#include <stdlib.h>
#include <stdio.h>

#include "heap.h"

heap *init_heap(int capacity)
{
    heap *new = malloc(sizeof(*new));
    new->nodes = malloc(capacity * sizeof(*new->nodes));
    new->capacity = capacity;
    new->size = 0;

    for (int i = 0; i < new->capacity; i++) {
        new->nodes[i] = init_tree();
        new->nodes[i]->right = new->nodes[i]->left = NULL;
    }

    return new;
}

void destroy_heap(heap *root)
{
    free(root->nodes);
    free(root);
}

tree *poll_heap(heap *root)
{
	if (!root->size) {
        printf("huffman: polling heap of size zero.\n");
		exit(EXIT_FAILURE);
	}

    tree *old = root->nodes[0];
    root->nodes[0] = root->nodes[--root->size];

    heapify(root, 0);

    return old;
}

void insert_into_heap(heap *root, tree *node)
{
    root->size++;
    int i = root->size - 1;
	while ((i > 0) && (node->data.frequency < root->nodes[get_parent(i)]->data.frequency)) {
		root->nodes[i] = root->nodes[get_parent(i)];
		i = get_parent(i);
	}
	root->nodes[i] = node;
}

void heapify(heap *root, int i)
{
    tree *temp;
    int smallestIdx = i;
    int left = get_child_left(i);
    int right = get_child_right(i);
 
    // If left child is larger than root
    if ((left < root->size) &&
        (root->nodes[left]->data.frequency < root->nodes[smallestIdx]->data.frequency))
        smallestIdx = left;
 
    // If right child is larger than smallestIdx so far
    if ((right < root->size) &&
        (root->nodes[right]->data.frequency < root->nodes[smallestIdx]->data.frequency))
        smallestIdx = right;
 
    // If smallestIdx is not root
    if (smallestIdx != i) {
        temp = root->nodes[i];
        root->nodes[i] = root->nodes[smallestIdx];
        root->nodes[smallestIdx] = temp;
 
        heapify(root, smallestIdx);
    }
}

int get_parent(int index)
{
    return (int)((index-1)/2);
}

int get_child_left (int index)
{
	return (index * 2) + 1;
}

int get_child_right (int index)
{
	return (index * 2) + 2;
}

#ifdef DEBUG
    void print_heap(heap *root)
    {
        for (int i = 0; i < root->size; i++)
            printf("i: %d, id: %d, char: %c, freq: %d\n", i, root->nodes[i]->data.id, root->nodes[i]->data.character, root->nodes[i]->data.frequency);
    }
#endif /* DEBUG */
