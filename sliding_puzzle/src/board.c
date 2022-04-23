#include <stdio.h>
#include <string.h>

#include "board.h"
#include "utilities.h"

move_t move_up(board_t *board)
{
    if (board->last_move == DOWN) // Avoid undoing the previous move.
        return INVALID;

    int next = (int)board->position - (int)board->dimension;
    board->last_move = UP;

    if (next < 0) // Moving outside board via UP.
        return INVALID;
    move_swap(board, next);
    return board->last_move;
}

move_t move_right(board_t *board)
{
    if (board->last_move == LEFT) // Avoid undoing the previous move.
        return INVALID;

    int next = (int)board->position + 1;
    board->last_move = RIGHT;

    if ((board->position % board->dimension) == board->dimension - 1) // Moving to next line via RIGHT.
        return INVALID;
    move_swap(board, next);
    return board->last_move;
}

move_t move_down(board_t *board)
{
    if (board->last_move == UP) // Avoid undoing the previous move.
        return INVALID;

    int next = (int)board->position + (int)board->dimension;
    board->last_move = DOWN;

    if (next > (board->dimension * board->dimension - 1)) // Moving outside board via DOWN.
        return INVALID;
    move_swap(board, next);
    return board->last_move;
}

move_t move_left(board_t *board)
{
    if (board->last_move == RIGHT) // Avoid undoing the previous move.
        return INVALID;

    int next = (int)board->position - 1;
    board->last_move = LEFT;

    if ((board->position % board->dimension) == 0) // Moving to next line via LEFT.
        return INVALID;

    move_swap(board, next);
    return board->last_move;
}

void move_swap(board_t *board, int next)
{
    unsigned int temp = board->table[board->position];
    board->table[board->position] = board->table[next];
    board->table[next] = temp;

    insert_move_history(board->moves, board->table[board->position]); // Maintain Move History

    board->position = (size_t)next;
}

move_history_t *init_move_history(void)
{
    move_history_t *new = malloc(sizeof(*new));
    check_malloc_error(new, "solve: Failed to malloc move history");
    new->size = 0;
    new->head = new->last = NULL;
    return new;
}

void insert_move_history(move_history_t *mh, unsigned int swapped)
{
    move_history_node_t *new = malloc(sizeof(*new));
    check_malloc_error(new, "solve: Failed to malloc move history node");
    new->piece = swapped;
    new->next = NULL;

    mh->size++;
    if (!mh->head) {
        mh->head = mh->last = new;
        return;
    }
    mh->last->next = new;
    mh->last = new;
}

move_history_t *copy_move_history(move_history_t *mh)
{
    move_history_t *new = init_move_history();
    move_history_node_t *temp = mh->head;

    while(temp) {
        insert_move_history(new, temp->piece);
        temp = temp->next;
    }
    return new;
}

void destroy_move_history(move_history_t *mh)
{
    move_history_node_t *temp;

    while(mh->head) {
        temp = mh->head;
        mh->head = mh->head->next;
        free(temp);
    }
    free(mh);
}

board_t *init_board(size_t dimension)
{
    board_t *new = malloc(sizeof(*new));
    new->dimension = dimension;
    new->moves = NULL;
    new->last_move = INITIAL;
    new->position = dimension * dimension;
    new->table = malloc(dimension * dimension * sizeof(*new->table));
    check_malloc_error(new->table, "solve: Failed to malloc table");
    return new;
}

board_t *copy_board(board_t *board)
{
    board_t *new = init_board(board->dimension);
    new->last_move = board->last_move;
    new->position = board->position;
    new->moves = copy_move_history(board->moves); // Also copy Move History
    memcpy(new->table, board->table, new->dimension * new->dimension * sizeof(*new->table));
    return new;
}

int compare_boards(board_t *board1, board_t *board2)
{
    size_t i;
    if (board1->dimension != board2->dimension)
        return 0; // Not even same matrix dimensions

    for (i = 0; i < board1->dimension * board1->dimension; i++)
        if (board1->table[i] != board2->table[i])
            return 0; // Different
    return 1; // Same
}

int board_has_solution(board_t *board)
{
    /* Check following cases:
        1. if dimension is odd
            a. number of inversions in even
        2. if dimension is even
            a. position is on even row from bottom and number of inversions is odd
            b. position is on odd row from bottom and number of inversions is even
     */
    
    size_t i, j, pos, level;
    int inversions = 0;
    /* Calculate number of inversions */
    for (i = 0; i < board->dimension * board->dimension - 1; i++) {
        if (i == board->position)
            continue;
        for (j = i + 1; j < board->dimension * board->dimension; j++) {
            if (j == board->position)
                continue;
            if (board->table[i] > board->table[j])
                inversions++;
        }
    }

    if (board->dimension % 2) { // Dimension is odd
        if (!(inversions % 2)) // Inversions in even
            return 1;
    }
    else { // Dimension is even
        pos = board->position;
        level = 0;
        while (pos < board->dimension * board->dimension) {
            pos += board->dimension;
            level++;
        }

        if ((level % 2) && !(inversions % 2)) // odd and even
            return 1;
        else if (!(level % 2) && (inversions % 2)) // even and odd
            return 1;
    }

    return 0; // no condition is true
}

board_t *generate_completed_board(size_t dimension)
{
    size_t i;

    board_t *new = init_board(dimension);
    for (i = 0; i < (new->dimension * new->dimension - 1); i++)
        new->table[i] = i + 1;
    new->table[i] = 0;
    new->position = i;
    return new;
}

void write_board_to_file(struct input_s *input, board_t *board)
{
    FILE *output_fp = fopen(input->output_file, "w");
    if (!output_fp) {
        perror("Failed to create/open output file");
        exit(EXIT_FAILURE);
    }

    fprintf(output_fp, "#number of moves\n%zu\n#moves\n", board->moves->size);
    if (board->moves->size) {
        move_history_node_t *mh_node = board->moves->head;

        while (mh_node) {
            fprintf(output_fp, "%u ", mh_node->piece);
            mh_node = mh_node->next;
        }
        fprintf(output_fp, "\n");

    }
    else {
        fprintf(output_fp, "no solution\n");
    }
}

void destroy_board(board_t *board)
{
    if (board->moves)
        destroy_move_history(board->moves);
    free(board->table);
    free(board);
}

#ifdef DEBUG
    char *move_from_value(move_t move)
    {
        switch (move) {
            case 0: return "INITAL";
            case 1: return "UP";
            case 2: return "RIGHT";
            case 3: return "DOWN";
            case 4: return "LEFT";
            default: return "INVALID";
        }
    }

    void print_move_history(move_history_t *mh)
    {
        move_history_node_t *mh_node = mh->head;

        while (mh_node->piece) {
            printf("%u ", mh_node->piece);
            mh_node = mh_node->next;
        }
        printf("\n");
    }

    void print_board(const board_t *board)
    {
        printf("Dimension: %zux%zu\n", board->dimension, board->dimension);
        printf("Last Move: %s\n", move_from_value(board->last_move));
        printf("Current position: %zu\n", board->position);
        printf("Current Table:");
        for (size_t i = 0; i < board->dimension * board->dimension; i++) {
            if (!(i % board->dimension)) // Go to next row.
                printf("\n\t");
            if (board->table[i] < 10) // Right justify the table.
                printf(" ");
            printf(" %u", board->table[i]);
        }
        printf("\n");
    }
#endif /* DEBUG */
