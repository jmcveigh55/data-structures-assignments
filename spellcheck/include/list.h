#ifndef _LIST_H_
#define _LIST_H_

typedef struct list_s {
    char *word;
    int score;
    struct list_s *next;
} list;

static list *initialize_linked_list(void);
void destroy_linked_list(list *head);
int linked_list_push_head(list **head, const char *word, const int score);
int linked_list_search(list *head, const char *word);
list *linked_list_concat(list* l1, list* l2);
int linked_list_length(list *head);

#ifdef DEBUG
	void linked_list_print(list *head);
#endif /* DEBUG */

#endif /* _LIST_H_ */