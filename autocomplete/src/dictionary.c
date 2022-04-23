#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

#include "dictionary.h"

/*
 * Load the dictionary.
 *
 * dictionary_file: the file containing the dictionary data.
 */
entry **load_dictionary(char *dictionary_file, char *delimiter)
{
	entry **dictionary;
	char *dict_sorted_path, dict_file_path[PATH_MAX];
	sprintf(dict_file_path, "./%s", dictionary_file);

	dict_sorted_path = get_dictionary_sorted_name(dict_file_path);

	if (access(dict_sorted_path, F_OK)) {
		dictionary = load_dictionary_from_file(dict_file_path, delimiter);
		sort_dictionary(dictionary);
		write_dictionary_to_file(dict_sorted_path, dictionary, delimiter);
	}	
	else
		dictionary = load_dictionary_from_file(dict_sorted_path, delimiter);

	free(dict_sorted_path);
    return dictionary;
}

/*
 * Load the dictionary from the supplied file.
 *
 * dict_file_path: the file containing the dictionary data.
 * delimiter: the token that separates the data.
 */
entry **load_dictionary_from_file(char *dict_file_path, char *delimiter)
{
	FILE *dict_fd;
	size_t len, i;
    char *line;
    entry **dictionary = malloc(sizeof(entry*) * DICTIONARY_SIZE);

	dict_fd = fopen(dict_file_path, "r");
	if (!dict_fd) {
		perror("auto: Error opening reference file");
		exit(EXIT_FAILURE);
	}

	line = NULL;
    i = 0;
	while (getline(&line, &len, dict_fd) != -1) {
        line[strlen(line) - 1] = '\0';
        dictionary[i] = malloc(sizeof(dictionary[i]));
		if (!dictionary) {
			perror("auto: Failed to malloc dictionary[i]: ");
			exit(EXIT_FAILURE);
		}

		dictionary[i]->word = strdup(strtok(line, delimiter));
        dictionary[i]->weight = atof(strtok(NULL, "\n"));
        i++;
	}

	fclose(dict_fd);
	free(line);

	return dictionary;
}

/*
 * Get the formatted sorted version name for a dictrionary file.
 *
 * dict_file_path: the original dictionary file name.
 */
char *get_dictionary_sorted_name(char *dict_file_path)
{
    char *sorted_name, *sorted_name_w_data, *sorted_name_w_ext, *last_period;

    if (!dict_file_path)
		return NULL;
	
	sorted_name = malloc(strlen(dict_file_path)+1);
    if (!sorted_name) {
		perror("auto: Failed to malloc ret: ");
		exit(EXIT_FAILURE);
	}

    strcpy(sorted_name, dict_file_path);
    last_period = strrchr (sorted_name, '.');
    if (last_period)
        *last_period = '\0';

	sorted_name_w_data = strconcat("data/", sorted_name);
	sorted_name_w_ext = strconcat(sorted_name_w_data, ".sorted"),

	free(sorted_name);
	free(sorted_name_w_data);
    return sorted_name_w_ext;
}

/*
 * Sort the dictionary alphabetically using bubble sort.
 * (ascending order)
 *
 * dictionary: the array containing reference data.
 */
void sort_dictionary(entry **dictionary)
{
	int i, j;
	entry *x, *y, temp;

	for (i=0; i < DICTIONARY_SIZE-1; i++) {
		for (j=0; j < DICTIONARY_SIZE-i-1; j++) {
			if (strcmp(dictionary[j]->word, dictionary[j+1]->word) > 0) {
				temp = *dictionary[j];
    			*dictionary[j] = *dictionary[j+1];
    			*dictionary[j+1] = temp;
			}
		}
	}
}

/*
 * Free all the data in the dictionary array.
 *
 * dictionary: the array containing reference data.
 */
void destroy_dictionary(entry **dictionary)
{
    size_t i;

    for (i=0; i < DICTIONARY_SIZE; i++) {
		free(dictionary[i]->word);
        free(dictionary[i]);
    }

    free(dictionary);
}

/*
 * Write dictionary data to designated file.
 *
 * dict_file_path: the file path to write data to.
 * dictionary: the array containing reference data.
 * delimiter: the token to sperate the data with.
 */
void write_dictionary_to_file(char *dict_file_path, entry **dictionary, char *delimiter)
{
	FILE *fp;
    size_t i;

	if (dict_file_path) {
		fp = fopen(dict_file_path, "w+");
		if (!fp) {
			perror("auto: Failed to create dict_file_path: ");
			exit(EXIT_FAILURE);
		}
	}
	else
		fp = stdout;

    for (i=0; i < DICTIONARY_SIZE; i++)
        fprintf(fp, "%s%s%f\n",dictionary[i]->word, delimiter, dictionary[i]->weight);
	
	fclose(fp);
}

/*
 * Print the dictionary to stdout.
 *
 * dictionary: the array containing reference data.
 */
void print_dictionary(entry **dictionary)
{
	write_dictionary_to_file(NULL, dictionary, ",");
}

/*
 * Combines the functions of malloc and strcat into one.
 *
 * str1: the left side of the concatenation.
 * str2: the right side of the concatenation.
 */
char* strconcat(char *str1, const char *str2)
{
	char *str = NULL;
	size_t len1 = 0;
	size_t len2 = 0;

	len1 = strlen(str1);
	len2 = strlen(str2);
	
	if (!(str = calloc(sizeof(char), (len1 + len2 + 1))))
		return NULL;
	if (str1)
		memcpy(str, str1, len1);
	if (str2)
		memcpy(str + len1, str2, len2);
	return (str);
}