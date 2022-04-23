#ifndef _MISC_H_
#define _MISC_H_

int round_to_multiple_of(int multiple, int num);
unsigned int get_file_line_count(char *file_name);
float string_score(const char *word);
char* swap_chars(char *str, int pos);
char* remove_char(char *str, int pos);
char* insert_char(char *str, char c, int pos);
char* replace_char(char *str, char c, int pos);

#endif /* _MISC_H_ */
