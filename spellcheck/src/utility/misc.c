#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utility/misc.h"

int round_to_multiple_of(int multiple, int num) {
    int remainder = num % multiple;

    if (!multiple || !remainder)
        return num;

    return num + multiple - remainder;
}

unsigned int get_file_line_count(char *file_name)
{
    char c;
    unsigned int wc_l = 0;
    FILE *fp = fopen(file_name, "r");
    if (!fp) {
        perror("check: Could not open file:");
        exit(EXIT_FAILURE);
    }

    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n')
            wc_l++;
    return wc_l;  
}

/*
 * Determine a strings score by summing characters and dividing
 * 	by its length. Score is multiplied by a
 *  constant to widen the distribution.
 */
float string_score(const char *word)
{
	const char *c;
    float score;
	int len = 0, sum = 0;

	for (c = word; *c; c++) {
			sum += (int)*c - 96;
            len++;
    }
	
	score = (float)sum / (float)len;
	return score;
}

char* swap_chars(char *str, int pos)
{
    int i, len = strlen(str);
    char *p = malloc(len+1);

    for (i = 0; i < pos; i++)
        p[i] = str[i];
    
    if (pos != len-1) {
        p[i] = str[i+1];
        p[i+1] = str[i];
        i += 2; // Skip i and i+1 in the loop.

        for (; i < len+1; i++)
            p[i] = str[i];
    }

    return p;
}

char* remove_char(char *str, int pos)
{
    int i, len = strlen(str);
    char *p = malloc(len); // removing char so this does account for null char.

    for (i = 0; i < pos; i++)
        p[i] = str[i];
    
    i++;
    
    for (; i < len; i++)
        p[i] = str[i+1];
    
    return p;
}

char* insert_char(char *str, char c, int pos)
{
    int i, len = strlen(str);
    char *p = malloc(len + 2);

    for (i = 0; i < pos; i++)
        p[i] = str[i];
    
    p[i] = c;
    i++;
    
    for (; i < len+2; i++)
        p[i] = str[i-1];
    
    return p;
}

char* replace_char(char *str, char c, int pos)
{
    int i, len = strlen(str);
    char *p = malloc(len + 1);

    for (i = 0; i < pos; i++)
        p[i] = str[i];
    
    p[i] = c;
    i++;
    
    for (; i < len+1; i++)
        p[i] = str[i];
    
    return p;
}
