#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "utilities.h"

/*
 * Print the program usage.
 */
static void print_usage(void)
{
    printf("usage: ./game_of_life [-h] [-d] input_file.txt \"partial query\"\n");
    printf("optional arguments:\n");
    printf("\t-h\t\t: Print the usage statement.\n");
    printf("\t-d\t\t: Set the delimiter for the reference dictionary.\n");
}

/*
 * Parse the command line input
 *
 * argc: argument count.
 * argv: array of argument strings.
 */
void parse_input(int argc, char *argv[], struct input_s *input)
{
	int option;
    char *default_delimiter = " ";
    input->dictionary_file = NULL;
    input->query = NULL;
    input->delimiter = strdup(default_delimiter);

    /* Optional arguments with flags */
	while ((option = getopt(argc, argv, ":hd:")) != -1) {
        switch (option) {
            case 'h': /* Print usage */
                goto usage_and_exit;
            case 'd': /* Set delimiter */
                input->delimiter = strdup(optarg);
                break;
            case '?': /* Unknown */
                fprintf(stderr, "auto: invalid option.\n");
            default:
                goto usage_and_exit;
        }
    }

    /* Optional Arduments without flags */
    for(; optind < argc; optind++) {     
        if (!input->dictionary_file)
            input->dictionary_file = strdup(argv[optind]);
        else if (!input->query)
            input->query = strdup(argv[optind]);
        else {
            fprintf(stderr, "auto: invalid argument \"%s\".\n", argv[optind]);
            goto usage_and_exit;
        }
    }

    if (!input->query || !input->dictionary_file) {
        fprintf(stderr, "auto: missing argument.\n");
        goto usage_and_exit;
    }

    return;

usage_and_exit:
	print_usage();
	exit(EXIT_FAILURE);
}

void destroy_input(struct input_s *input)
{
    free(input->query);
    free(input->dictionary_file);
    free(input->delimiter);
}
