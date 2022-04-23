#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "hash.h"
#include "list.h"
#include "utility/misc.h"

openHashTable *load_hash_table_from_file(char *dict_file)
{
	size_t len;
	char *line_buf;
	size_t size = hash_table_determine_size(dict_file);

	openHashTable *hash_table = initialize_hash_table(size);
	FILE *dict_fd = fopen(dict_file, "r");
	if (!dict_fd)
		return NULL;

	line_buf = NULL;
	while (getline(&line_buf, &len, dict_fd) != -1) {
		line_buf[strlen(line_buf) - 1] = '\0';
		if (line_buf[strlen(line_buf) - 1] == '\r')
			line_buf[strlen(line_buf) - 1] = '\0';

		if (!hash_table_insert(hash_table, line_buf))
			return NULL;
	}
	
	free(line_buf);

	fclose(dict_fd);
	return hash_table;
}

static openHashTable *initialize_hash_table(const int num_bins)
{
	int i;
	openHashTable *hash_table = malloc(sizeof(*hash_table));
	if (!hash_table)
		return NULL;

	hash_table->table = malloc(num_bins * sizeof(list*));
	if (!hash_table->table)
		return NULL;
	
	for (i = 0; i < num_bins; i++)
		hash_table->table[i] = NULL;
	
	hash_table->size = num_bins;
	
	return hash_table;
}

static size_t hash_table_determine_size(char *file_name)
{
	size_t words_total = get_file_line_count(file_name);
	return (size_t)log2(words_total) + 1; // 2^(size-1) < total words < 2^(size)
}

int hash_table_insert(openHashTable *hash_table, char* word)
{
	int hash_bin = hash(hash_table->size, word);
	return linked_list_push_head(&hash_table->table[hash_bin], word, 0);
}

int hash_table_search(const openHashTable *hash_table, const char *word)
{
	int hash_value = hash(hash_table->size, word);
	if (linked_list_search(hash_table->table[hash_value], word))
		return -1; /* found */
	return hash_value; /* not found */
}

/*
 * Hash by summing and rounding to bin.
 */
static int hash(const int num_bins, const char *word)
{
	int score, hash_value;

	score = (int)string_score(word) * num_bins;
	hash_value = round_to_multiple_of(num_bins+1, score) % num_bins;

	assert(hash_value > -1);
	assert(hash_value < num_bins);

	return hash_value;
}

void destroy_hash_table(openHashTable *hash_table)
{
	int i;
	for (i = 0; i < hash_table->size; i++)
		destroy_linked_list(hash_table->table[i]);
	free(hash_table->table);
	free(hash_table);
}

#ifdef DEBUG
	void print_hash(openHashTable *hash_table)
	{
		int i;
		
		for (i = 0; i < hash_table->size; i++) {
			printf("hash_table->table[%d]:\t", i);
			linked_list_print(hash_table->table[i]);
		}
	}
#endif /* DEBUG */
