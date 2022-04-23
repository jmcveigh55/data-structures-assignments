#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "astar.h"
#include "board.h"
#include "utilities.h"

queue_t *init_queue(void)
{
    queue_t *new = malloc(sizeof(*new));
    check_malloc_error(new, "solve: Failed to malloc queue");
    new->size = 0;
    new->head = new->last = NULL;
    return new;
}

void destroy_node(queue_node_t *node)
{
    destroy_board(node->board);
    free(node);
}

void destroy_queue(queue_t *queue)
{
    queue_node_t *temp;

    while(queue->head) {
        temp = dequeue(queue);
        destroy_board(temp->board);
        free(temp);
    }
    free(queue);
}

queue_node_t *dequeue(queue_t *queue)
{
    if (!queue->head)
        return NULL;

    queue->size--;
    queue_node_t *temp = queue->head;
    queue->head = queue->head->next;
    temp->next = NULL;
    return temp;
}

void enqueue(queue_t *queue, board_t *board)
{
    queue_node_t *new = malloc(sizeof(*new));
    check_malloc_error(new, "solve: Failed to malloc board node");
    new->board = board;
    new->next = NULL;

    queue->size++;
    if (!queue->head) {
        queue->head = queue->last = new;
        return;
    }
    queue->last->next = new;
    queue->last = new;
}

int search_queue_for_board_in_place(queue_t *queue, board_t *board)
{
    size_t i;
    int found = 0;
    queue_node_t *temp = queue->head;
    
    if (!queue->size)
        return 0;

    while((temp) && (!found)) {
        for (i = 0; i < board->dimension * board->dimension; i++) {
            found = 1; // Assume found
            if (temp->board->table[i] != board->table[i]) {
                found = 0; // Set not found if difference encountered
                break;
            }
        }

        temp = temp->next;
    }

    return found;
}

board_t *solve_board_using_astar(board_t *goal_board, board_t *initial_board)
{
    board_t *solved_board;
    heap_node *test_heap_node, *initial_heap_node, *up_heap_node, *right_heap_node, *down_heap_node, *left_heap_node;

    if (compare_boards(goal_board, initial_board))
        return initial_board;

    heap *root = init_heap(HEAP_SIZE);
    
    initial_heap_node = init_heap_node();
    initial_heap_node->priority = 0;
    initial_heap_node->board = initial_board;
    insert_into_heap(root, initial_heap_node);

    /* Intermediate Board Storage (Queue DS used but operated on like a linked list) */
    queue_t *intermediate_boards = init_queue();

    solved_board = NULL;
    while (!solved_board) {
        test_heap_node = poll_heap(root);
        if (!test_heap_node) {
            printf("solve: Attempting to dequeue from empty heap.\n");
            exit(EXIT_FAILURE);
        }

        /* Check if current board is the goal board */
        if (compare_boards(goal_board, test_heap_node->board)) {
                solved_board = test_heap_node->board;
                break;
        }

        /* Check if current board has already been visited */
        if (search_queue_for_board_in_place(intermediate_boards, test_heap_node->board))
            goto skip_node;
        else
            enqueue(intermediate_boards, copy_board(test_heap_node->board));

        /* Generate heuristics for each move */
        up_heap_node = init_heap_node();
        up_heap_node->board = copy_board(test_heap_node->board);
        if (move_up(up_heap_node->board) != INVALID) {
            up_heap_node->priority = get_manhattan_distance_heuristic(up_heap_node->board);
            insert_into_heap(root, up_heap_node);
        }
        else
            destroy_heap_node(up_heap_node);

        right_heap_node = init_heap_node();
        right_heap_node->board = copy_board(test_heap_node->board);
        if (move_right(right_heap_node->board) != INVALID) {
            right_heap_node->priority = get_manhattan_distance_heuristic(right_heap_node->board);
            insert_into_heap(root, right_heap_node);
        }
        else
            destroy_heap_node(right_heap_node);

        down_heap_node = init_heap_node();
        down_heap_node->board = copy_board(test_heap_node->board);
        if (move_down(down_heap_node->board) != INVALID) {
            down_heap_node->priority = get_manhattan_distance_heuristic(down_heap_node->board);
            insert_into_heap(root, down_heap_node);
        }
        else
            destroy_heap_node(down_heap_node);

        left_heap_node = init_heap_node();
        left_heap_node->board = copy_board(test_heap_node->board);
        if (move_left(left_heap_node->board) != INVALID) {
            left_heap_node->priority = get_manhattan_distance_heuristic(left_heap_node->board);
            insert_into_heap(root, left_heap_node);
        }
        else
            destroy_heap_node(left_heap_node);

skip_node:
        destroy_heap_node(test_heap_node);
    }

    destroy_heap(root);
    destroy_queue(intermediate_boards);
    return solved_board;
}

int get_manhattan_distance_heuristic(board_t *board)
{
    int i;
    int total_manhattan_distance = 0;

    for (i = 0; i < board->dimension * board->dimension; i++)
        total_manhattan_distance += compute_manhattan_distance(board->table[i], i, board->dimension);
    
    return total_manhattan_distance + (int)board->moves->size;
}

int compute_manhattan_distance(int value, int idx, int dim)
{
    return abs((idx % dim) - ((value - 1) % dim)) + abs((idx / dim) - ((value - 1) / dim));
}

heap *init_heap(int capacity)
{
    heap *new = malloc(sizeof(*new));
    new->nodes = malloc(capacity * sizeof(*new->nodes));
    new->capacity = capacity;
    new->size = 0;

    for (int i = 0; i < new->capacity; i++)
        new->nodes[i] = init_heap_node();

    return new;
}

void destroy_heap(heap *root)
{
    free(root->nodes);
    free(root);
}

heap_node *poll_heap(heap *root)
{
	if (!root->size) {
        printf("solve: polling heap of size zero.\n");
		exit(EXIT_FAILURE);
	}

    heap_node *old = root->nodes[0];
    root->nodes[0] = root->nodes[--root->size];

    heapify(root, 0);

    return old;
}

void insert_into_heap(heap *root, heap_node *node)
{
    root->size++;
    int i = root->size - 1;
	while ((i > 0) && (node->priority < root->nodes[get_parent(i)]->priority)) {
		root->nodes[i] = root->nodes[get_parent(i)];
		i = get_parent(i);
	}
	root->nodes[i] = node;
}

void heapify(heap *root, int i)
{
    heap_node *temp;
    int smallestIdx = i;
    int left = get_child_left(i);
    int right = get_child_right(i);
 
    // If left child is larger than root
    if ((left < root->size) &&
        (root->nodes[left]->priority < root->nodes[smallestIdx]->priority))
        smallestIdx = left;
 
    // If right child is larger than smallestIdx so far
    if ((right < root->size) &&
        (root->nodes[right]->priority < root->nodes[smallestIdx]->priority))
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

heap_node *init_heap_node(void)
{
    heap_node *new = malloc(sizeof(*new));
    return new;
}

void destroy_heap_node(heap_node *node)
{
    free(node->board);
    free(node);
}
