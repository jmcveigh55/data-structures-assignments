#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utilities.h"
#include "heap.h"
#include "huffman.h"

tree *init_tree(void)
{
    tree *new = malloc(sizeof(*new));
    new->left = new->right = NULL;
    return new;
}

void destroy_tree(tree *node)
{
    free(node);
}

tree *huffman_tree_from_counterCollection(counterCollection *cc)
{
    int i, entry_count;
	tree *t1, *t2, *joint;
	heap *root;

	root = init_heap(cc->size);
	for (i = 0; i < cc->size; i++) {
		t1 = init_tree();
		t1->data = cc->entries[i];
		insert_into_heap(root, t1);
	}

    entry_count = root->size - 1;
	for (i = 0; i < entry_count; i++) {
		t1 = poll_heap(root);
		t2 = poll_heap(root);

		joint = init_tree();
		joint->left = t1;
		joint->right = t2;

		joint->data.id = -1;
		joint->data.frequency = t1->data.frequency + t2->data.frequency;
		insert_into_heap(root, joint);
	}

	/* return the huffman tree. (located at root) */
	return poll_heap(root);
}

void huffman_code_walk(tree *root, int gen, char *code, huff_code *codes)
{
	/* If current node is a leaf */
	if ((!root->left) && (!root->right)) {
		code[gen] = '\0';

		codes[root->data.id].data = root->data;
		codes[root->data.id].code = strdup(code);
	}

	/* Traverse down edges */
	if (root->left) {
		code[gen] = '0';
		huffman_code_walk(root->left, gen+1, code, codes);
	}

	if (root->right) {
		code[gen] = '1';
		huffman_code_walk(root->right, gen+1, code, codes);
	}
}

huff_code *huffman_compile_codes(tree *root, int size)
{
	char code[ASCII_CHARACTER_COUNT] = "";
	huff_code *codes = malloc(size * sizeof(*codes));
	huffman_code_walk(root, 0, code, codes);
	return codes;
}

int huffman_write_encoded_file(tree *root, huff_code *codes, int codes_size, char *text_file_name, char *en_file_name)
{
	int i, encoded_size;

    FILE *text_fp = fopen(text_file_name, "r");
    if (!text_fp) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

	FILE *en_fp = fopen(en_file_name, "w");
	if (!en_fp) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    char c;
	encoded_size = 0;
    while((c = fgetc(text_fp)) != EOF) {
		for (i = 0; i < codes_size; i++) {
			if (codes[i].data.character == c) {
				fprintf(en_fp, "%s", codes[i].code);
				encoded_size += strlen(codes[i].code);
			}
		}
	}
    
    fclose(text_fp);
	fclose(en_fp);
	return encoded_size;
}

void huffman_write_code_table_file(huff_code *codes, int codes_size, char *ct_file_name)
{
	int i;

    FILE *ct_fp = fopen(ct_file_name, "w");
    if (!ct_fp) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

	for (i = 0; i < codes_size; i++) {
		if (codes[i].data.frequency) {
			fprintf(ct_fp, "%c;%s;%d\n", codes[i].data.character, codes[i].code, codes[i].data.frequency);
		}
	}
    
	fclose(ct_fp);
}

huff_code *read_codes_from_code_table_file(char *file_name, int *codes_size)
{
	*codes_size = get_file_line_count(file_name);
	huff_code *codes = malloc(*codes_size * sizeof(*codes));

	FILE *ct_fp = fopen(file_name, "r");
    if (!ct_fp) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

	int i = 0;
	char line[ASCII_CHARACTER_COUNT];
    while (fgets(line, ASCII_CHARACTER_COUNT, ct_fp))
    {
        char* tmp = strdup(line);
		codes[i].data.character = strtok(tmp, ";")[0];
		codes[i].code = strdup(strtok(NULL, ";"));
		codes[i].data.frequency = atoi(strtok(NULL, ";"));
        free(tmp);
		i++;
    }

	return codes;
}

void huffman_write_decoded_file(huff_code *codes, int codes_size, char *en_file_name, char *text_file_name)
{
    FILE *en_fp = fopen(en_file_name, "r");
    if (!en_fp) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    FILE *text_fp = fopen(text_file_name, "w");
    if (!text_fp) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

	char c;
	int i, found, gen = 0;
	char code[ASCII_CHARACTER_COUNT];
    while((c = fgetc(en_fp)) != EOF) {
		found = 0;
		code[gen] = c;
		code[gen+1] = '\0';
		for (i = 0; i < codes_size; i++) {
			if (!strcmp(code, codes[i].code)) {
				fprintf(text_fp, "%c", codes[i].data.character);
				gen = 0;
				found = 1;
				break;
			}
		}
		
		if (!found)
			gen++;
	}
    
	fclose(en_fp);
	fclose(text_fp);
}

#ifdef DEBUG
	void print_tree(tree *root)
	{
		if (root)
			printf("char: %c freq: %d\n", root->data.character, root->data.frequency);
		if (root->left)
			print_tree(root->left);
		if (root->right)
			print_tree(root->right);
	}

	void print_huffman_codes(huff_code *codes, int size) {
        for (int i = 0; i < size; i++)
            if (codes[i].data.frequency)
                printf("code: %s char: %c freq: %d\n", codes[i].code,
                       codes[i].data.character,
                       codes[i].data.frequency);
	}
#endif /* DEBUG */
