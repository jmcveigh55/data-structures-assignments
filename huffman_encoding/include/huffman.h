#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#include "counter.h"

typedef struct tree_s tree;
struct tree_s {
    counter data;
    tree *left, *right;
};

typedef struct huff_code_s {
    counter data;
    char *code;
} huff_code;

tree *init_tree(void);
void destroy_tree(tree *node);

tree *huffman_tree_from_counterCollection(counterCollection *cc);
void huffman_code_walk(tree *root, int gen, char *code, huff_code *codes);
huff_code *huffman_compile_codes(tree *root, int size);

int huffman_write_encoded_file(tree *root, huff_code *codes, int codes_size, char *text_file_name, char *en_file_name);
void huffman_write_code_table_file(huff_code *codes, int codes_size, char *ct_file_name);

huff_code *read_codes_from_code_table_file(char *file_name, int *codes_size);
void huffman_write_decoded_file(huff_code *codes, int codes_size, char *en_file_name, char *text_file_name);

#ifdef DEBUG
	void print_tree(tree *root);
    void print_huffman_codes(huff_code *codes, int size);
#endif /* DEBUG */

#endif /* _HUFFMAN_H_ */