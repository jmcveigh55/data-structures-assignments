#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#define DICTIONARY_SIZE 50000

typedef struct entry_s entry;
struct entry_s {
    char *word;
    float weight;
};

entry **load_dictionary(char *dictionary_file, char *delimiter);
entry **load_dictionary_from_file(char *dict_file_path, char *delimiter);
char *get_dictionary_sorted_name(char *dict_file_path);
void sort_dictionary(entry **dictionary);
void destroy_dictionary(entry **dictionary);
void print_dictionary(entry **dictionary);
void write_dictionary_to_file(char *dict_file_path, entry **dictionary, char *delimiter);
char* strconcat(char *str1, const char *str2);

#endif /* _DICTIONARY_H_ */