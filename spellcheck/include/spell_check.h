#ifndef _SPELL_CHECK_H_
#define _SPELL_CHECK_H_

#include "utility/input.h"
#include "hash.h"
#include "list.h"

int spell_check(struct input_s *input);
list *spell_check_find_suggestions(list *similar_list, char *word);
void spell_check_print_best(list *head, int num_suggestions);
list *spell_check_suggestions_by_swap(list *similar_list, char *word);
list *spell_check_suggestions_by_remove(list *similar_list, char *word);
list *spell_check_suggestions_by_insert(list *similar_list, char *word);
list *spell_check_suggestions_by_replace(list *similar_list, char *word);

#endif /* _SPELL_CHECK_H_ */