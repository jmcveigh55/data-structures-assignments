#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "counter.h"

counterHashTable *counterTable_from_file(char *file_name)
{
    counterHashTable *cht = init_counterTable();
    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char c;
    while((c = fgetc(fp)) != EOF)
        increment_counterTable(cht, c);
    
    fclose(fp);
    return cht;
}

counterHashTable *init_counterTable(void)
{
    counterHashTable *new = malloc(sizeof(*new));
    new->size = ASCII_CHARACTER_COUNT;
    for (int i = 0; i < new->size; i++)
        new->table[i] = 0;
    return new;
}

void destroy_counterTable(counterHashTable *cht)
{
    free(cht);
}

void increment_counterTable(counterHashTable *cht, char ascii_char)
{
    cht->table[hash_ascii(ascii_char)]++;
}

int hash_ascii(char ascii_char)
{
    return (int)ascii_char;
}

counterCollection *counterCollection_from_file(char *file_name)
{
    counterHashTable *cht = counterTable_from_file(file_name);
    counterCollection *cc = counterCollection_from_counterHashTable(cht);

    destroy_counterTable(cht);
    return cc;
}

counterCollection *counterCollection_from_counterHashTable(counterHashTable *cht)
{
    int i, j;
    counterCollection *collection = init_counterCollection();
    
    collection->size = 0;
    for (i = 0; i < cht->size; i++)
        if (cht->table[i])
            collection->size++;
    
    collection->entries = malloc(collection->size * sizeof(*collection->entries));
    j = 0;
    for (i = 0; i < cht->size; i++) {
        if (cht->table[i]) {
            collection->entries[j].character = (char)i;
            collection->entries[j].frequency = cht->table[i];
            collection->entries[j].id = j;
            j++;
        }
    }

    return collection;
}

counterCollection *init_counterCollection(void)
{
    counterCollection *new = malloc(sizeof(*new));
    return new;
}

void destroy_counterCollection(counterCollection *cc)
{
    free(cc->entries);
    free(cc);
}

void counterCollection_selectionSort(counterCollection *collection)
{
    counter temp;
    int i, j, midx;
 
    for (i = 0; i < collection->size - 1; i++) {
        midx = i;
        for (j = i + 1; j < collection->size; j++)
            if (collection->entries[j].frequency < collection->entries[midx].frequency)
                midx = j;
        // Swap
        temp = collection->entries[midx];
        collection->entries[midx] = collection->entries[i];
        collection->entries[i] = temp;
    }
}

#ifdef DEBUG
    void print_counterHashTable(counterHashTable *cht)
    {
        for (int i = 0; i < cht->size; i++)
            if (cht->table[i])
                printf("cht->table[%d] -> char = %c freq. = %d\n", i, (char)i, cht->table[i]);
    }

    void print_counterCollection(counterCollection *cc)
    {
        for (int i = 0; i < cc->size; i++)
            printf("cc->entries[%d] -> char = %c freq. = %d\n", i, cc->entries[i].character, cc->entries[i].frequency);
    }
#endif /* DEBUG */
