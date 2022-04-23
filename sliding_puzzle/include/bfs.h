#ifndef __BFS_H_
#define __BFS_H_

#include "board.h"

typedef struct bfs_node_s {
    board_t *board;
    struct bfs_node_s *next;
} bfs_node_t;

typedef struct bfs_queue_s {
    size_t size;
    bfs_node_t *head;
    bfs_node_t *last;
} bfs_queue_t;

bfs_queue_t *init_queue(void);
bfs_node_t *dequeue(bfs_queue_t *queue);
void enqueue(bfs_queue_t *queue, board_t *board);
int search_queue_for_board_in_place(bfs_queue_t *queue, board_t *board);
board_t *solve_board_using_bfs(board_t *goal_board, board_t *initial_board);
void destroy_node(bfs_node_t *node);
void destroy_queue(bfs_queue_t *queue);

#endif /* __BFS_H_ */
