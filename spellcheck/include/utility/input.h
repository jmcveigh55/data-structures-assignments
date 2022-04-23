#ifndef _INPUT_H_
#define _INPUT_H_

struct input_s {
    char *dictionary_file;
    char *word;
    int wb;
};

static void print_usage(void);
void parse_input(int argc, char *argv[], struct input_s *input);
int parse_stdin_for_word(struct input_s *input);
void parse_stdin_for_wb(struct input_s *input);
void destroy_input(struct input_s *input);

#endif /* _INPUT_H_ */
