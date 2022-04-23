#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spell_check.h"
#include "utility/input.h"
#include "utility/misc.h"
#include "hash.h"
#include "list.h"

int spell_check(struct input_s *input)
{
    int i, input_hash_value, mistyped, num_suggestions = 3;
    list *suggestions, *new_suggestions;
    openHashTable *hash_table = load_hash_table_from_file(input->dictionary_file);
    if (!hash_table)
        return EXIT_FAILURE;

#ifdef DEBUG
    print_hash(hash_table);
#endif /* DEBUG */

    printf("Enter text to be spell checked: ");
    while (parse_stdin_for_word(input)) {
        /* Search dictionary for input word. */
        input_hash_value = hash_table_search(hash_table, input->word);
        mistyped = (input_hash_value != -1) ? 1 : 0;

        if (mistyped) {
            printf("Mistyped word: %s\n", input->word);

            /* Present suggestions to the user. Print top n. */
            suggestions = spell_check_find_suggestions(hash_table->table[input_hash_value], input->word);
            if (linked_list_length(suggestions) < num_suggestions) { // check other bins until a suggestion is found.
                for (i=0; i < hash_table->size; i++) {
                    if (i != input_hash_value) {
                        new_suggestions = spell_check_find_suggestions(hash_table->table[i], input->word);
                        suggestions = linked_list_concat(suggestions, new_suggestions);
                        if (linked_list_length(suggestions) >= num_suggestions)
                            break;
                    }
                }
            }

            if (!suggestions)
                printf("No suggestions were found for '%s'\n", input->word);
            else {
                printf("Suggested words for mistyped word '%s': ", input->word);
                spell_check_print_best(suggestions, num_suggestions);
                destroy_linked_list(suggestions);
                suggestions = NULL;
            }

            /* Ask user if they want to append the input word to the file.*/
            printf("Would you like to add \'%s\' to the hash? (y/n): ", input->word);
            parse_stdin_for_wb(input);
            if (input->wb) {
                hash_table_insert(hash_table, input->word);
                printf("'%s' has been added to the hash.\n", input->word);
            }
        }

        free(input->word);
        printf("Enter text to be spell checked: ");
    }

    destroy_hash_table(hash_table);
    return EXIT_SUCCESS;
}

list *spell_check_find_suggestions(list *similar_list, char *word)
{
    list *suggestions_swap = NULL;
    list *suggestions_remove = NULL;
    list *suggestions_insert = NULL;
    list *suggestions_replace = NULL;
    list *suggestions = NULL;

    /* * Go through similar_list and calculate a score for each word.
       * Subtract input_score from the word score. (Closer to 0 is better).
       * Set object score to absolute value of offset score.
       * Push object in suggestions. */

    /* Do each suggestion search, append values together. */
    suggestions_swap = spell_check_suggestions_by_swap(similar_list, word);
    suggestions_remove = spell_check_suggestions_by_remove(similar_list, word);
    suggestions_insert = spell_check_suggestions_by_insert(similar_list, word);
    suggestions_replace = spell_check_suggestions_by_replace(similar_list, word);

    suggestions = suggestions_swap;
    suggestions = linked_list_concat(suggestions, suggestions_remove);
    suggestions = linked_list_concat(suggestions, suggestions_insert);
    suggestions = linked_list_concat(suggestions, suggestions_replace);

    return suggestions;
}

void spell_check_print_best(list *head, int num_suggestions)
{
    list *node = head;

    while(node && num_suggestions) {
        printf("%s", node->word);
        node = node->next;
        num_suggestions--;
        if (num_suggestions && node)
            printf(", ");
        else
            printf("\n");
    }
}
/*
 * Swap adjacent letters.
 */
list *spell_check_suggestions_by_swap(list *similar_list, char *word)
{
    // swap each adjacent letter and search for word in hash.
    int i, len, test_score, word_score;
    char *test_word;
    list *suggestions = NULL, *node = similar_list;
    
    word_score = (int)string_score(word);
    len = strlen(word);

    while(node) {
        for (i=0; i < len-1; i++) {
            test_word = swap_chars(word, i);
            if (!strcmp(node->word, test_word)) {
                test_score = abs((int)string_score(node->word) - word_score);
                linked_list_push_head(&suggestions, node->word, test_score);
                free(test_word);
                break;
            }
            free(test_word);
        }
        node = node->next;
    }
    return suggestions;
}

/*
 * Remove letters.
 */
list *spell_check_suggestions_by_remove(list *similar_list, char *word)
{
    // loop modifying a copy of word, remove a letter and seach for word in hash.
    int i, len, test_score, word_score;
    char *test_word;
    list *suggestions = NULL, *node = similar_list;
    
    word_score = (int)string_score(word);
    len = strlen(word);

    while(node) {
        for (i=0; i < len; i++) {
            test_word = remove_char(word, i);
            if (!strcmp(node->word, test_word)) {
                test_score = abs((int)string_score(node->word) - word_score);
                linked_list_push_head(&suggestions, node->word, test_score);
                free(test_word);
                break;
            }
            free(test_word);
        }
        node = node->next;
    }
    return suggestions;
}


/*
 * Add letters.
 */
list *spell_check_suggestions_by_insert(list *similar_list, char *word)
{
    // loop modifying a copy of word, add a letter and seach for word in hash.
    int i, len, found, test_score, word_score;
    char *c, *test_word, test_chars[] = "etaionshrdw";
    list *suggestions = NULL, *node = similar_list;

    word_score = (int)string_score(word);    
    len = strlen(word);

    while(node) {
        found = 0;
        for (i=0; i < len+1; i++) {
            if (found)
                break;

            for (c=test_chars; *c; c++) {
                test_word = insert_char(word, *c, i);
                if (!strcmp(node->word, test_word)) {
                    test_score = abs((int)string_score(node->word) - word_score);
                    linked_list_push_head(&suggestions, node->word, test_score);
                    found = 1;
                    free(test_word);
                    break;
                }
                free(test_word);
            }
        }
        node = node->next;
    }
    return suggestions;
}

/*
 * Replace letters.
 */
list *spell_check_suggestions_by_replace(list *similar_list, char *word)
{
    // loop modifying a copy of word, add a letter and seach for word in hash.
    int i, len, found, test_score, word_score;
    char *c, *test_word, test_chars[] = "etaionshrdw";
    list *suggestions = NULL, *node = similar_list;

    word_score = (int)string_score(word);    
    len = strlen(word);

    while(node) {
        found = 0;
        for (i=0; i < len+1; i++) {
            if (found)
                break;

            for (c=test_chars; *c; c++) {
                test_word = replace_char(node->word, *c, i);
                if (!strcmp(word, test_word)) {
                    test_score = abs((int)string_score(node->word) - word_score);
                    linked_list_push_head(&suggestions, node->word, test_score);
                    found = 1;
                    free(test_word);
                    break;
                }
                free(test_word);
            }
        }
        node = node->next;
    }

    return suggestions;
}
