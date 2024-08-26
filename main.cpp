#include <stdlib.h>
#include <stdio.h>
#include "handle_flags.h"
#include "custom_assert.h"
#include "handlers.h"
#include "colors.h"

/**
===============================================================================================================================
    @mainpage Quadratic.exe
    @brief    Allowes to solve quadratic equations.
    @details  Has different modes:\n
                + Solving (--solve)\n
                + Testing (--test)\n
              --help to get help
    @author   Artem Neskorodov
    @link     https://vk.com/neskorodovartem
    @date     24.08.2024

===============================================================================================================================
*/

program_modes_t *register_modes_flags(void);

int main(const int argc, const char *argv[]) {
    program_modes_t *modes = register_modes_flags();

    if(modes == NULL)
        return EXIT_FAILURE;

    exit_code_t result = parse_flags(argc, argv, modes);
    free_modes(modes);
    return (int)result;
}

program_modes_t *register_modes_flags(void) {
    program_modes_t *modes = NULL;
    if(register_mode(&modes, "-s", "--solve", handle_solve) != EXIT_CODE_SUCCESS) {
        free_modes(modes);
        return NULL;
    }

    if(register_mode(&modes, "-t", "--test", handle_test) != EXIT_CODE_SUCCESS) {
        free_modes(modes);
        return NULL;
    }

    if(register_mode(&modes, "-h", "--help", handle_help) != EXIT_CODE_SUCCESS) {
        free_modes(modes);
        return NULL;
    }

    if(choose_default_mode(&modes, handle_solve) != EXIT_CODE_SUCCESS) {
        free_modes(modes);
        return NULL;
    }
    return modes;
}
