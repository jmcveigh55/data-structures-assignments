#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "board.h"
#include "utilities.h"

board_t *parse_board_file(const char *board_file)
{
    int i;
    size_t len;
    char *piece = NULL, *line = NULL;

    FILE *fp;
    fp = fopen(board_file, "r");
    if (!fp) {
        perror("solve: Unable to open file");
        exit(EXIT_FAILURE);
    }

    board_t *board = malloc(sizeof(*board));
    check_malloc_error(board, "solve: Failed to malloc board");
    board->last_move = INITIAL;
    board->dimension = 0;
    board->moves = init_move_history();

    while (getline(&line, &len, fp) != -1) {
        line[strlen(line) - 1] = '\0'; /* replace \n with \0. */

        if (!strcmp(line, "")) /* Blank line */
            continue;
        else if (!strcmp(line, "#k")) { /* Dimension header */
            if (getline(&line, &len, fp) == -1)
                break;
            line[strlen(line) - 1] = '\0'; /* replace \n with \0. */

            board->dimension =  atoi(line);
            board->table = malloc(board->dimension * board->dimension * sizeof(*board->table));
            check_malloc_error(board->table, "solve: Failed to malloc table");
        }
        else if (!strcmp(line, "#initial state")) { /* Initial state header */
            if (getline(&line, &len, fp) == -1) {
                printf("solve: Initial state header read but missing data\n");
                break;
            }
            line[strlen(line) - 1] = '\0'; /* replace \n with \0. */
            piece = strdup(line);
            check_malloc_error(piece, "solve: Failed to strdup piece");

            board->table[0] = atoi(strtok(piece, " "));
            if (!board->table[0])
                board->position = 0;
            for (i = 1; i < board->dimension * board->dimension; i++) {
                board->table[i] = atoi(strtok(NULL, " "));
                if (!board->table[i])
                    board->position = i;
            }

            free(piece);
        }
        else { /* Unknown line in board file */
            line[strlen(line) - 1] = '\0'; /* replace \n with \0. */
            printf("solve: Unknown line parsed in board file: \"%s\"\n", line);
            printf("The following headers are accepted: \"#k\" and \"#initial state\".\n");
            exit(EXIT_FAILURE);
        }
    }

    free(line);
    fclose(fp);
    return board;
}
