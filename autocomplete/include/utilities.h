#ifndef _UTILITIES_H_
#define _UTILITIES_H_

struct input_s {
    char *dictionary_file;
    char *query;
    char *delimiter;
};

static void print_usage(void);
void parse_input(int argc, char *argv[], struct input_s *input);
void destroy_input(struct input_s *input);

#endif /* _UTILITIES_H_ */
