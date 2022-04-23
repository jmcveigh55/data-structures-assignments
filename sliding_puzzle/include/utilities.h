#ifndef _UTILITIES_H_
#define _UTILITIES_H_

/*
 * MACRO: check_malloc_error. If failed, print msg and exit failure.
 */
#define check_malloc_error(field, msg) { \
    if (!field) { \
        perror(msg); \
        exit(EXIT_FAILURE); \
    } \
}

/*
 * MACRO: alloc_input_arg. Short-hand allocate string argument,
 *   provide error checking.
 */
#define alloc_input_arg(field, arg) { \
    input->field = strdup(arg); \
    check_malloc_error(input->field, "solve: Failed to parse argument"); \
}

struct input_s {
    char *board_file;
    char *output_file;
};

static void print_usage(void);
void parse_input(int argc, char *argv[], struct input_s *input);
void destroy_input(struct input_s *input);

#endif /* _UTILITIES_H_ */