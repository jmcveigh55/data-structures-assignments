#include <stdlib.h>
#include <stdio.h>

#include "utilities.h"
#include "autocomplete.h"

int main(int argc, char *argv[])
{
    entry **dictionary;
    list *completions;
    struct input_s input;

    parse_input(argc, argv, &input); /* parse input and optional flags */
    dictionary = load_dictionary(input.dictionary_file, input.delimiter); /* load dictionary from file or pre-sorted file */

    if ((completions = search_for_completions(dictionary, input.query))) {
        sort_completion_list(&completions); /* sorts into ascending order */
        reverse_completion_list(&completions); /* reverse into descending order */

        print_completions(completions);
        destroy_completion_list(completions);
    }
    else /* No completions found */
        printf("-1\n");

    destroy_dictionary(dictionary);
    destroy_input(&input);

    return 0;
}
