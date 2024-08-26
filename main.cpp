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

int main(const int argc, const char *argv[]) {
    program_modes_t *modes = modes_list_init();
    if(modes == NULL)
        return EXIT_FAILURE;

    if(register_mode(modes, "-s", "--solve", handle_solve) != EXIT_CODE_SUCCESS) {
        free_modes(modes);
        return EXIT_FAILURE;
    }

    if(register_mode(modes, "-t", "--test", handle_test) != EXIT_CODE_SUCCESS) {
        free_modes(modes);
        return EXIT_FAILURE;
    }

    if(register_mode(modes, "-h", "--help", handle_help) != EXIT_CODE_SUCCESS) {
        free_modes(modes);
        return EXIT_FAILURE;
    }

    if(choose_default_mode(modes, handle_solve) != EXIT_CODE_SUCCESS) {
        free_modes(modes);
        return EXIT_FAILURE;
    }

    exit_code_t result = parse_flags(argc, argv, modes);
    free_modes(modes);
    return (int)result;
}
