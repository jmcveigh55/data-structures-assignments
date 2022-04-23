#include <stdio.h>
#include <stdlib.h>

#include "utility/input.h"
#include "spell_check.h"

int main (int argc, char *argv[])
{
    struct input_s input;
    parse_input(argc, argv, &input);
    
    if (spell_check(&input) != EXIT_SUCCESS) {
        perror("check: An error occured during execution: ");
        return EXIT_FAILURE;
    }

    destroy_input(&input);
    return EXIT_SUCCESS;
}
