#include <stdlib.h>
#include <stdio.h>

#include "bfs.h"
#include "board.h"
#include "utilities.h"

bfs_queue_t *init_queue(void)
{
    bfs_queue_t *new = malloc(sizeof(*new));
    check_malloc_error(new, "solve: Failed to malloc queue");
    new->size = 0;
    new->head = new->last = NULL;
    return new;
}

bfs_node_t *dequeue(bfs_queue_t *queue)
{
    if (!queue->head)
        return NULL;

    queue->size--;
    bfs_node_t *temp = queue->head;
    queue->head = queue->head->next;
    temp->next = NULL;
    return temp;
}

void enqueue(bfs_queue_t *queue, board_t *board)
{
    bfs_node_t *new = malloc(sizeof(*new));
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

int search_queue_for_board_in_place(bfs_queue_t *queue, board_t *board)
{
    size_t i;
    int found = 0;
    bfs_node_t *temp = queue->head;
    
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

board_t *solve_board_using_bfs(board_t *goal_board, board_t *initial_board)
{
    board_t *solved_board, *temp_board;

    if (compare_boards(goal_board, initial_board))
        return initial_board;

    /* BFS Queue */
    bfs_queue_t *queue = init_queue();
    enqueue(queue, initial_board);

    /* Intermediate Board Storage (Queue DS used but operated on like a linked list) */
    bfs_queue_t *intermediate_boards = init_queue();

    solved_board = NULL;
    while (!solved_board) {
        bfs_node_t *test_node = dequeue(queue);
        if (!test_node) {
            printf("solve: Attempting to dequeue from empty queue.\n");
            exit(EXIT_FAILURE);
        }

        /* Check if current board has already been visited */
        if (search_queue_for_board_in_place(intermediate_boards, test_node->board))
            goto skip_node;
        else
            enqueue(intermediate_boards, copy_board(test_node->board));

        /* Do applicable moves and queue mutated boards if valid */

        if (test_node->board->last_move != DOWN) {
            temp_board = copy_board(test_node->board);
            if (move_up(temp_board) != INVALID) {
                if (compare_boards(goal_board, temp_board)) {
                    solved_board = temp_board;
                    break;
                }

                enqueue(queue, temp_board);
            }
            else
                destroy_board(temp_board);
        }
        
        if (test_node->board->last_move != LEFT) {
            temp_board = copy_board(test_node->board);
            if (move_right(temp_board) != INVALID) {
                if (compare_boards(goal_board, temp_board)) {
                    solved_board = temp_board;
                    break;
                }

                enqueue(queue, temp_board);
            }
            else
                destroy_board(temp_board);
        }
        
        if (test_node->board->last_move != UP) {
            temp_board = copy_board(test_node->board);
            if (move_down(temp_board) != INVALID) {
                if (compare_boards(goal_board, temp_board)) {
                    solved_board = temp_board;
                    break;
                }

                enqueue(queue, temp_board);
            }
            else
                destroy_board(temp_board);
        }

        if (test_node->board->last_move != RIGHT) {
            temp_board = copy_board(test_node->board);
            if (move_left(temp_board) != INVALID) {
                if (compare_boards(goal_board, temp_board)) {
                    solved_board = temp_board;
                    break;
                }

                enqueue(queue, temp_board);
            }
            else
                destroy_board(temp_board);
        }

skip_node:
        destroy_node(test_node);
    }

    destroy_queue(queue);
    destroy_queue(intermediate_boards);
    return solved_board;
}

void destroy_node(bfs_node_t *node)
{
    destroy_board(node->board);
    free(node);
}

void destroy_queue(bfs_queue_t *queue)
{
    bfs_node_t *temp;

    while(queue->head) {
        temp = dequeue(queue);
        destroy_board(temp->board);
        free(temp);
    }
    free(queue);
}
