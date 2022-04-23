#ifndef _AUTOCOMPLETE_H_
#define _AUTOCOMPLETE_H_

#include "dictionary.h"

typedef struct list_s list;
struct list_s {
    entry *data;
    list *next;
};

list *search_for_completions(entry** dictionary, char* query);
int binary_search_dictionary(entry** dictionary, char *query, int left, int right);
int get_first_match(entry** dictionary, char *query, int index);
void sort_completion_list(list **completions);
void reverse_completion_list(list **completions);
void push_head_completion_list(list **completions, entry *data);
void print_completions(list *completions);
void destroy_completion_list(list *completions);

#endif /* _AUTOCOMPLETE_H_ */