#ifndef __ASTAR_H_
#define __ASTAR_H_

#include "board.h"

#define HEAP_SIZE 512

typedef struct queue_node_s {
    board_t *board;
    struct queue_node_s *next;
} queue_node_t;

typedef struct queue_s {
    int size;
    queue_node_t *head;
    queue_node_t *last;
} queue_t;

typedef struct heap_node_s heap_node;
struct heap_node_s {
    int priority;
    board_t *board;
};

typedef struct heap_s {
    int capacity;
    int size;
    heap_node **nodes;
} heap;

queue_t *init_queue(void);
void destroy_queue(queue_t *queue);
void destroy_node(queue_node_t *node);
queue_node_t *dequeue(queue_t *queue);
void enqueue(queue_t *queue, board_t *board);

int search_queue_for_board_in_place(queue_t *queue, board_t *board);
board_t *solve_board_using_astar(board_t *goal_board, board_t *initial_board);

int get_manhattan_distance_heuristic(board_t *board);
int compute_manhattan_distance(int value, int idx, int dim);

heap *init_heap(int capacity);
void destroy_heap(heap *root);
heap_node *poll_heap(heap *root);
void insert_into_heap(heap *root, heap_node *node);
void heapify (heap *p, int i);
int get_parent(int index);
int get_child_left (int index);
int get_child_right (int index);

heap_node *init_heap_node(void);
void destroy_heap_node(heap_node *node);

#endif /* __ASTAR_H_ */
