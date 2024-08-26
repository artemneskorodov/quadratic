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

int main(int argc, const char *argv[]) {

    if(modes_init(3) != EXIT_CODE_SUCCESS)
        return EXIT_FAILURE;

    if(register_mode("--solve", "-s", handle_solve) != EXIT_CODE_SUCCESS) {
        free_modes();
        return EXIT_FAILURE;
    }

    if(register_mode("--help",  "-h", handle_help ) != EXIT_CODE_SUCCESS) {
        free_modes();
        return EXIT_FAILURE;
    }

    if(register_mode("--test",  "-t", handle_test ) != EXIT_CODE_SUCCESS) {
        free_modes();
        return EXIT_FAILURE;
    }

    if(choose_default_mode("-s") != EXIT_CODE_SUCCESS) {
        free_modes();
        return EXIT_FAILURE;
    }
    exit_code_t result = parse_flags(argc, argv);
    free_modes();
    return (int)result;
}
