/**
===============================================================================================================================
    @file    custom_assert.cpp
    @brief   Assert macro
    @date    23.08.2024
    @author  Artem Neskorodov
    @link    https://vk.com/neskorodovartem

===============================================================================================================================
*/
#include "custom_assert.h"
#include "colors.h"
#include <stdlib.h>

void print_assert_error(const char *string, int line_number, const char *filename) {
    C_ASSERT(string != NULL);
    C_ASSERT(filename != NULL);

    color_printf(RED, "-<<CUSTOM ASSERT>>-\n"
                     "Caught error on line %d of file \"%s\"\n"
                     "Expression: %s\n", line_number, filename, string);
    exit(EXIT_FAILURE);
}
