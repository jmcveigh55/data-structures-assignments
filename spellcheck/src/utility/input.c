#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "utility/input.h"

/*
 * Print the program usage.
 */
void print_usage(void)
{
    printf("usage: ./check [-h] [-d] DICTIONARY_FILE\n");
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
    input->dictionary_file = NULL;

    /* Arguments with flags */
	while ((option = getopt(argc, argv, ":hd:")) != -1) {
        switch (option) {
            case 'h': /* Print usage */
                goto usage_and_exit;
            case '?': /* Unknown */
                fprintf(stderr, "check: invalid option.\n");
            default:
                goto usage_and_exit;
        }
    }

    /* Arguments without flags */
    for(; optind < argc; optind++) {     
        if (!input->dictionary_file) {
            input->dictionary_file = strdup(argv[optind]);
            if (!input->dictionary_file) {
                perror("check: Failed to parse input: ");
                exit(EXIT_FAILURE);
            }
        }
        else {
            fprintf(stderr, "check: invalid argument \"%s\".\n", argv[optind]);
            goto usage_and_exit;
        }
    }

    if (!input->dictionary_file) {
        fprintf(stderr, "check: missing argument.\n");
        goto usage_and_exit;
    }

    return;

usage_and_exit:
	print_usage();
	exit(EXIT_FAILURE);
}

/*
 * Parse the standard input for word
 *
 * input: input struct to store input details.
 */
int parse_stdin_for_word(struct input_s *input)
{
    char temp[256];
    char *c;

    scanf("%s", temp);
    getchar(); // Skip the newline character.
    input->word = strdup(temp);
    if (!input->word) {
        perror("check: Failed to parse input: ");
        exit(EXIT_FAILURE);
    }

    if (!strcmp(input->word, "exit"))
        return 0;

    for (c = temp; *c; c++) {
        if (!isalpha(*c)) { /* If the string contains a space */
            fprintf(stderr, "check: \"%s\" contains non-letters, try again\n", temp);
            return parse_stdin_for_word(input);
        }
    }

    return 1;
}

/*
 * Parse the standard input for write back.
 *
 * input: input struct to store input details.
 */
void parse_stdin_for_wb(struct input_s *input)
{
    char temp;

    scanf("%c", &temp);
    getchar(); // Skip the newline character.

    if (temp != 'y' && temp != 'n') {
        fprintf(stderr, "check: \"%c\" is not a valid choice.\n", temp);
        parse_stdin_for_wb(input);
        return;
    }

    input->wb = (temp == 'y') ? 1 : 0;
}

/*
 * Destroy the input struct members.
 *
 * input: input struct to store input details.
 */
void destroy_input(struct input_s *input)
{
    free(input->dictionary_file);
    free(input->word);
}
