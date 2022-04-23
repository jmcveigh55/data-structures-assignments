#include <stdio.h>

#include "utilities.h"
#include "board.h"
#include "parser.h"
#include "astar.h"

int main(int argc, char *argv[])
{
    struct input_s input;
    parse_input(argc, argv, &input);

    /* Parse Inital Board and Use Dimension for Goal Board */
    board_t *initial_board = parse_board_file(input.board_file);
    board_t *goal_board = generate_completed_board(initial_board->dimension);

#ifdef DEBUG
    printf("********** INTIAL BOARD **********\n");
    print_board(initial_board);
    printf("******** END INITIAL BOARD ********\n\n");

    printf("*********** GOAL BOARD ***********\n");
    print_board(goal_board);
    printf("********* END GOAL BOARD *********\n\n");
#endif /* DEBUG */

    /* Check if possible to solve */
    if (!board_has_solution(initial_board))
        goto no_solution;

    /* Solve using A Star */
    board_t *solved_board = solve_board_using_astar(goal_board, initial_board);

#ifdef DEBUG
    printf("********** SOLVED BOARD **********\n");
    print_board(solved_board);
    printf("******** END SOLVED BOARD ********\n\n");
#endif /* DEBUG */

    /* Write the output file per handout spec */
    write_board_to_file(&input, solved_board);

    printf("Solution for \"%s\" is located in \"%s\"\n", input.board_file, input.output_file);
    destroy_board(goal_board);
    destroy_board(solved_board);
    destroy_input(&input);
    return 0;

no_solution:
    printf("No solution for \"%s\"\n", input.board_file);
    destroy_board(initial_board);
    destroy_board(goal_board);
    destroy_input(&input);
    return 0;
}
