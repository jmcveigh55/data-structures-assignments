#ifndef _HASH_H_
#define _HASH_H_

#include "list.h"

typedef struct openHashTable_s {
    size_t size;
    list **table;
} openHashTable;

openHashTable *load_hash_table_from_file(char *dict_file);
static openHashTable *initialize_hash_table(const int num_bins);
static size_t hash_table_determine_size(char *file_name);
int hash_table_insert(openHashTable *hash_table, char* word);
int hash_table_search(const openHashTable *hash_table, const char *word);
static int hash(const int num_bins, const char *word);
void destroy_hash_table(openHashTable *hash_table);

#ifdef DEBUG
    void print_hash(openHashTable *hash_table);
#endif /* DEBUG */

#endif /* _HASH_H_ */