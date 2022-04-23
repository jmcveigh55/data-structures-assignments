#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "autocomplete.h"
#include "dictionary.h"

/* TODO: Switch to binary search and take matches until not matched.
 * Can assume dictionary is already sorted.
 *
 * Search the dictionary for completions of the query.
 * Implemented using nested linear search.
 * 
 * dictionary: reference dictionary containing all possible completions.
 * query: the subquery to use to find possible completions.
 */
list *search_for_completions(entry** dictionary, char* query)
{
    int i, count, found, first;
    list *completions = NULL;

    found = binary_search_dictionary(dictionary, query, 0, DICTIONARY_SIZE-1);
    if (found == -1)
        return NULL;

    first = get_first_match(dictionary, query, found);

    i = first;
    while (!strncmp(dictionary[i]->word, query, strlen(query))) {
        push_head_completion_list(&completions, dictionary[i]);
        i++;
    }

    return completions;
}

/*
 * Binary search a dictionary for a query. Return index where found.
 * 
 * dictionary: reference dictionary containing all possible completions.
 * query: the subquery to use to find possible completions.
 * left: the left search boundry
 * right: the right search boundry
 */
int binary_search_dictionary(entry** dictionary, char *query, int left, int right)
{
    int check;
    int middle;

    if (right > left) {
        middle = (left + (right - 1)) / 2; // Floors since using ints.

        check = strncmp(dictionary[middle]->word, query, strlen(query)); // Compares indiv. chars.
        if (!check)
            return middle;
        else if (check > 0)
            return binary_search_dictionary(dictionary, query, left, middle - 1);
        else
            return binary_search_dictionary(dictionary, query, middle + 1, right);
    }

    return -1;
}

/*
 * Find first index where query is matched in dictionary.
 * 
 * dictionary: reference dictionary containing all possible completions.
 * query: the subquery to use to find possible completions.
 */
int get_first_match(entry** dictionary, char *query, int index)
{
    if (index == 0)
        return 0;

    if (!strncmp(dictionary[index-1]->word, query, strlen(query)))
        return get_first_match(dictionary, query, index-1);

    return index;
}

/*
 * Sort the list of completions by weight using bubble sort.
 * (ascending order)
 *
 * completions: the list of completions for the given query.
 */
void sort_completion_list(list **completions)
{
    int swapped;
    list *cur, *last;
    entry *temp;
    
    last = NULL;

    do
    {
        swapped = 0;
        cur = *completions;
  
        while (cur->next != last) {
            if (cur->data->weight > cur->next->data->weight) { 
                temp = cur->data;
                cur->data = cur->next->data;
                cur->next->data = temp;
                swapped = 1;
            }
            cur = cur->next;
        }
        last = cur;
    }
    while (swapped);

}

/*
 * Reverse the list of completions.
 *
 * completions: the list of completions for the given query.
 */
void reverse_completion_list(list **completions)
{
    list  *prev, *cur, *next;

    prev = NULL;
    cur = *completions;

    while (cur) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    *completions = prev;
}

/*
 * Push data to front of the completion list.
 *
 * completions: the list of completions for the given query.
 * data: data to be pushed.
 */
void push_head_completion_list(list **completions, entry *data)
{
    list *new;
    
    new = malloc(sizeof(list));
    new->data = malloc(sizeof(*data));
    new->data = data;
    new->next = *completions;
    *completions = new;
}

/*
 * Print the completion list
 *
 * completions: the list of completions for the given query.
 */
void print_completions(list *completions)
{
    list *node = completions;

    int i = 0;
    while(node) {
        printf("%s,%f\n", node->data->word, node->data->weight);
        node = node->next;
    }
}

/*
 * Free all the nodes in the completion list.
 *
 * completions: the list of completions for the given query.
 */
void destroy_completion_list(list *completions)
{
    list *node;

    while (completions) {
        node = completions;
        completions = completions->next;
        free(node);
    }
}
