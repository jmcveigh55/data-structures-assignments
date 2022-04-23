#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "utilities.h"

/*
 * MACRO: alloc_input_arg. Short-hand allocate string argument,
 *   provide error checking.
 */
#define alloc_input_arg(field, arg) { \
    input->field = strdup(arg); \
    if (!input->field) { \
        perror("huffman: Failed to parse argument"); \
        exit(EXIT_FAILURE); \
    } \
}

/*
 * Print the program usage.
 */
void print_usage(void)
{
    printf("usage: ./huffman [encode plainText.txt codeTable.txt encodedText.txt]");
    printf(" | [decode codeTable.txt encodedText.txt plainText.txt]\n");
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
    input->mode = -1;
    input->table_file = NULL;
    input->text_file = NULL;
    input->encoded_file = NULL;

    /* Arguments with flags */
	while ((option = getopt(argc, argv, ":h")) != -1) {
        switch (option) {
            case 'h': /* Print usage */
                goto usage_and_exit;
            case '?': /* Unknown */
                fprintf(stderr, "huffman: invalid option.\n");
            default:
                goto usage_and_exit;
        }
    }

    /* Arguments without flags */
    for(; optind < argc; optind++) {
        if (input->mode == -1) {
            if (!strcmp(argv[optind], "encode")) {
                input->mode = 0;
            }
            else if (!strcmp(argv[optind], "decode")) {
                input->mode = 1;
            }
            else {
                fprintf(stderr, "huffman: invalid mode \"%s\".\n", argv[optind]);
                goto usage_and_exit;
            }
        }
        else {
            if (!input->mode) { // encode
                if (!input->text_file) {
                    alloc_input_arg(text_file, argv[optind]);
                }
                else if (!input->table_file) {
                    alloc_input_arg(table_file, argv[optind]);
                }
                else if (!input->encoded_file) {
                    alloc_input_arg(encoded_file, argv[optind]);
                }
                else {
                    fprintf(stderr, "huffman: invalid argument \"%s\".\n", argv[optind]);
                    goto usage_and_exit;
                }
            }
            else { // decode
                if (!input->table_file) {
                    alloc_input_arg(table_file, argv[optind]);
                }
                else if (!input->encoded_file) {
                    alloc_input_arg(encoded_file, argv[optind]);
                }
                else if (!input->text_file) {
                    alloc_input_arg(text_file, argv[optind]);
                }
                else {
                    fprintf(stderr, "huffman: invalid argument \"%s\".\n", argv[optind]);
                    goto usage_and_exit;
                }
            }
        }
    }

    if (!input->table_file || !input->text_file || !input->encoded_file) {
        fprintf(stderr, "huffman: missing argument.\n");
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
    free(input->table_file);
    free(input->text_file);
    free(input->encoded_file);
}

int get_file_line_count(char *file_name)
{
    int line_count = 0;

    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char c;
    while((c = fgetc(fp)) != EOF)
        if (c == '\n')
            line_count += 1;
    
    fclose(fp);
    return line_count;
}
