#ifndef __BOARD_H_
#define __BOARD_H_

#include <stdlib.h>

#include "utilities.h"

typedef enum { INITIAL=0, UP, RIGHT, DOWN, LEFT, INVALID } move_t;

typedef struct move_history_node_s {
    unsigned int piece;
    struct move_history_node_s *next;
} move_history_node_t;

typedef struct move_history_s {
    size_t size;
    move_history_node_t *head;
    move_history_node_t *last;
} move_history_t;

typedef struct board_s {
    size_t dimension;
    move_t last_move;
    size_t position;
    move_history_t *moves;
    unsigned int *table;
} board_t;

move_t move_up(board_t *board);
move_t move_right(board_t *board);
move_t move_down(board_t *board);
move_t move_left(board_t *board);
void move_swap(board_t *board, int next);

move_history_t *init_move_history(void);
void insert_move_history(move_history_t *mh, unsigned int swapped);
move_history_t *copy_move_history(move_history_t *mh);
void destroy_move_history(move_history_t *mh);

board_t *init_board(size_t dimension);
board_t *copy_board(board_t *board);
int compare_boards(board_t *board1, board_t *board2);
int board_has_solution(board_t *board);
board_t *generate_completed_board(size_t dimension);
void write_board_to_file(struct input_s *input, board_t *board);
void destroy_board(board_t *board);

#ifdef DEBUG
    char *move_from_value(move_t move);
    void print_move_history(move_history_t *mh);
    void print_board(const board_t *board);
#endif /* DEBUG */

#endif /* __BOARD_H_ */
