#include <stdlib.h>
#include <stdio.h>

#include "utilities.h"
#include "counter.h"
#include "huffman.h"

int main (int argc, char *argv[])
{
    int i, original_size, compressed_size, compression_ratio;
    struct input_s input;
    parse_input(argc, argv, &input);

    if (!input.mode) { // encode
        /* Generate Counter */
        counterCollection *file_cc = counterCollection_from_file(input.text_file);
        counterCollection_selectionSort(file_cc);
        original_size = 0;
        for (i = 0; i < file_cc->size; i++)
            original_size += 8 * file_cc->entries[i].frequency;

#ifdef DEBUG
        printf("*** DEBUG: counter collection ***\n");
        print_counterCollection(file_cc);
        printf("*********************************\n\n");
#endif /* DEBUG */

        /* Compress via Huffman Encoding */
        tree *huffman_heap = huffman_tree_from_counterCollection(file_cc);

#ifdef DEBUG
        printf("****** DEBUG: huffman tree ******\n");
        print_tree(huffman_heap);
        printf("*********************************\n\n");
#endif /* DEBUG */

        huff_code *huffman_codes = huffman_compile_codes(huffman_heap, file_cc->size);

#ifdef DEBUG
        printf("***** DEBUG: huffman codes ******\n");
        print_huffman_codes(huffman_codes, file_cc->size);
        printf("*********************************\n\n");
#endif /* DEBUG */
        
        compressed_size = huffman_write_encoded_file(huffman_heap, huffman_codes, file_cc->size, input.text_file, input.encoded_file);
        huffman_write_code_table_file(huffman_codes, file_cc->size, input.table_file);
        compression_ratio = 100 - (int)(((float)(original_size - compressed_size)/original_size) * 100);

        printf("Size of Original File, \"%s\": %d bits\n", input.text_file, original_size);
        printf("Size of Encoded File, \"%s\": %d bits\n", input.encoded_file, compressed_size);
        printf("Compression Ratio: %d%%\n", compression_ratio);
        destroy_counterCollection(file_cc);
    }
    else { // decode
        int codes_size;
        huff_code *huffman_codes = read_codes_from_code_table_file(input.table_file, &codes_size);

#ifdef DEBUG
        for (i = 0; i < codes_size; i++)
            printf("char : %c code : %s freq : %d\n", huffman_codes[i].data.character, huffman_codes[i].code, huffman_codes[i].data.frequency);
        printf("\n");
#endif /* DEBUG */
    
        huffman_write_decoded_file(huffman_codes, codes_size, input.encoded_file, input.text_file);

        for (i = 0; i < codes_size; i++)
            free(huffman_codes[i].code);
    }

    return 0;
}
