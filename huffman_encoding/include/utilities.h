#ifndef _UTILITIES_H_
#define _UTILITIES_H_

struct input_s {
    int mode;
    char *table_file;
    char *text_file;
    char *encoded_file;
};

static void print_usage(void);
void parse_input(int argc, char *argv[], struct input_s *input);
void destroy_input(struct input_s *input);
int get_file_line_count(char *file_name);

#endif /* _UTILITIES_H_ */