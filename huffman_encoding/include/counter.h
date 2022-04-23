#ifndef _COUNTER_H_
#define _COUNTER_H_

/* implements a counter for 8-bit ascii characters */

#define ASCII_CHARACTER_COUNT 256

typedef struct counterHashTable_s {
    int size;
    int table[ASCII_CHARACTER_COUNT];
} counterHashTable;

typedef struct counter_s{
    char character;
    int frequency;
    int id;
} counter;

typedef struct counterCollection_s{
    int size;
    counter *entries;
} counterCollection;

counterHashTable *counterTable_from_file(char *file_name);
counterHashTable *init_counterTable(void);
void destroy_counterTable(counterHashTable *cht);
void increment_counterTable(counterHashTable *cht, char ascii_char);
int hash_ascii(char ascii_char);

counterCollection *counterCollection_from_file(char *file_name);
counterCollection *counterCollection_from_counterHashTable(counterHashTable *cht);
counterCollection *init_counterCollection(void);
void destroy_counterCollection(counterCollection *cc);
void counterCollection_selectionSort(counterCollection *collection);

#ifdef DEBUG
    void print_counterHashTable(counterHashTable *cht);
    void print_counterCollection(counterCollection *cc);
#endif /* DEBUG */

#endif /* _COUNTER_H_ */