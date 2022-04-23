#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

static list *initialize_linked_list(void)
{
    list *new = malloc(sizeof(*new));
    if (!new)
        return NULL;
    new->word = NULL;
    new->next = NULL;

    return new;
}

void destroy_linked_list(list *head)
{
    list *node;

    while (head) {
        node = head;
        head = head->next;
        free(node->word);
        free(node);
    }
}

int linked_list_push_head(list **head, const char *word, const int score)
{
    list *new = initialize_linked_list();
    if (!new)
        return 0;

    new->word = strdup(word);
    if (!new->word)
        return 0;

    new->score = score;

    new->next = *head;
    *head = new;
    return 1;
}

int linked_list_search(list *head, const char *word)
{
    list *node = head;

    while(node) {
        if (!strcmp(node->word, word))
            return 1;
        node = node->next;
    }
    return 0;
}

list *linked_list_concat(list* l1, list* l2)
{
    list *node;

    if (!l1)
        return l2;
    else if (!l2)
        return l1;

    node = l1;
    while(node->next)
        node = node->next;

    node->next = l2;

    return l1;
}

int linked_list_length(list *head)
{
    int len = 0;
    list *node = head;

    while(node) {
        len++;
        node = node->next;
    }
    return len;
}

#ifdef DEBUG
    void linked_list_print(list *head)
    {
        list *node = head;

        while(node) {
            printf(" %s ->", node->word);
            node = node->next;
        }
        printf(" NULL\n");
    }
#endif /* DEBUG */