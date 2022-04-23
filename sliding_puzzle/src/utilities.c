#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "utilities.h"

/*
 * Print the program usage.
 */
void print_usage(void)
{
    printf("usage: ./solve boardFile.txt\n");
    printf("optional arguments:\n");
    printf("\t-h\t\t: Print the usage statement.\n");
}

/*
 * Parse the command line input
 *
 * argc: argument count.
 * argv: array of argument strings.
 * input: input struct to store input details.
 */
void parse_input(int argc, char *argv[], struct input_s *input)
{
	int option;
    input->board_file = NULL;
    input->output_file = NULL;

    /* Arguments with flags */
	while ((option = getopt(argc, argv, ":h")) != -1) {
        switch (option) {
            case 'h': /* Print usage */
                goto usage_and_exit;
            case '?': /* Unknown */
                fprintf(stderr, "solve: invalid option.\n");
            default:
                goto usage_and_exit;
        }
    }

    /* Arguments without flags */
    for(; optind < argc; optind++) {
        if (!input->board_file) {
            alloc_input_arg(board_file, argv[optind]);
        }
        else if (!input->output_file) {
            alloc_input_arg(output_file, argv[optind]);
        }
        else {
            fprintf(stderr, "solve: invalid argument \"%s\".\n", argv[optind]);
            goto usage_and_exit;
        }
    }

    if ((!input->board_file) || (!input->output_file)) {
        fprintf(stderr, "solve: missing argument.\n");
        goto usage_and_exit;
    }

    return;

usage_and_exit:
	print_usage();
	exit(EXIT_FAILURE);
}

/*
 * Destroy the input struct members.
 *
 * input: input struct to store input details.
 */
void destroy_input(struct input_s *input)
{
    free(input->board_file);
    free(input->output_file);
}
