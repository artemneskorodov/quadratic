/**
===============================================================================================================================
    @file    handle_flags.cpp
    @brief   Handling flags pushed from console.
    @date    23.08.2024
    @author  Artem Neskorodov
    @link    https://vk.com/neskorodovartem

===============================================================================================================================
*/
#include "handle_flags.h"
#include <stdbool.h>
#include "quadratic.h"
#include "quadratic_tests.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "colors.h"
#include "custom_assert.h"

/**
===============================================================================================================================
    @brief Structure to store names and handle functions of supported modes.

===============================================================================================================================
*/
struct solving_mode_t {
    const char *long_name;
    const char *short_name;
    exit_code_t (*handle_function)(void);
};

static exit_code_t handle_unsupported_amount_of_flags(void);
static exit_code_t handle_unknown_flag(const char *flag);
static exit_code_t handle_help(void);
static exit_code_t handle_solve(void);
static exit_code_t handle_test(void);

/**
===============================================================================================================================
    @brief   - List of modes supported by program.
    @details - Contains short name and long name(for example '-h' and '--help').\n
             - Contains function to handle user.

===============================================================================================================================
*/
static const solving_mode_t modes[] = {
    {.long_name = "--help",  .short_name = "-h", .handle_function = handle_help  },
    {.long_name = "--solve", .short_name = "-s", .handle_function = handle_solve },
    {.long_name = "--test",  .short_name = "-t", .handle_function = handle_test  }
};

exit_code_t handle_user(int argc, const char *argv[]){
    C_ASSERT(argv != NULL);
    if(argc != 1 && argc != 2) return handle_unsupported_amount_of_flags();
    if(argc == 1) return handle_solve();
    for(size_t mode = 0; mode < sizeof(modes) / sizeof(solving_mode_t); mode++)
        if(strcmp(modes[mode].long_name, argv[argc - 1]) == 0 || strcmp(modes[mode].short_name, argv[argc - 1]) == 0)
            return modes[mode].handle_function();
    return handle_unknown_flag(argv[argc - 1]);
}

/**
===============================================================================================================================
    @brief   - Handles the case when user typed in more then one flag.

===============================================================================================================================
*/
exit_code_t handle_unsupported_amount_of_flags(void){
    color_printf(RED, "Unsupported amount of flags, you can use only 0 or 1\n");
    color_printf(DEFAULT, "Use flag ");
    color_printf(PURPLE, "'--help'\n");
    return EXIT_CODE_SUCCESS;
}

/**
===============================================================================================================================
    @brief   - Handles the case when user typed in unknown flag.

    @param   [in]  arg                String with flag, typed in by user.

===============================================================================================================================
*/
exit_code_t handle_unknown_flag(const char *flag){
    C_ASSERT(flag != NULL);
    color_printf(RED, "Unknown flag '%s'\n", flag);
    color_printf(DEFAULT, "Use flag ");
    color_printf(PURPLE, "'--help'\n");
    return EXIT_CODE_SUCCESS;
}

/**
===============================================================================================================================
    @brief   - Handles help request.

===============================================================================================================================
*/
exit_code_t handle_help(void){
    color_printf(PURPLE, "\t'--help'");
    color_printf(DEFAULT, " for help\n");
    color_printf(PURPLE, "\t'--solve'");
    color_printf(DEFAULT, " to type in and solve equation\n");
    color_printf(PURPLE, "\t'--test'");
    color_printf(DEFAULT, " to run tests\n");
    color_printf(PURPLE, "\t''");
    color_printf(DEFAULT, " is considered as ");
    color_printf(PURPLE, "'--solve'\n");
    return EXIT_CODE_SUCCESS;
}

/**
===============================================================================================================================
    @brief   - Solve mode.

    @details - Asks user to type in coefficients for quadratic equation.\n
             - Solves equation.\n
             - Prints resutlts in console.\n

===============================================================================================================================
*/
exit_code_t handle_solve(void){
    quadratic_equation_t equation = {.number = NOT_SOLVED};

    //Getting coefficients from user
    switch(get_coefficients(&equation)){
        case GETTING_EXIT: {
            color_printf(CYAN, "Stop using Vietta\n");
            return EXIT_CODE_SUCCESS;
        }
        case GETTING_SUCCESS: {
            break;
        }
        case GETTING_ERROR: {
            return EXIT_CODE_FAILURE;
        }
        default: {
            return EXIT_CODE_FAILURE;
        }
    }

    //Solving quadratic
    switch(solve_quadratic(&equation)) {
        case INVALID_COEFFICIENTS: {
            color_printf(RED, "Your input was invalid, unable to solve equation :(\n");
            return EXIT_CODE_FAILURE;
        }
        case SOLVING_ERROR: {
            color_printf(RED, "Caught unexpected error while solving\n");
            return EXIT_CODE_FAILURE;
        }
        case SOLVING_SUCCESS: {
            print_quadratic_result(&equation);
            return EXIT_CODE_SUCCESS;
        }
        default: {
            color_printf(RED, "solve_quadratic() returned unexpected result\n");
            return EXIT_CODE_FAILURE;
        }
    }
}

/**
===============================================================================================================================
    @brief   - Test mode.

    @details - Starts tests.\n
             - Prints total number of tests from file "tests.txt" and errors.

===============================================================================================================================
*/
exit_code_t handle_test(void) {
    int total = 0, errors = 0;
    switch(test_solving_quadratic(&total, &errors)) {
        case NO_SUCH_FILE: {
            color_printf(RED, "There is no file \"%s\"\n", TESTS_FILE_NAME);
            return EXIT_CODE_FAILURE;
        }
        case INVALID_LINES:{
            color_printf(RED, "Tests file is invalid\n");
            return EXIT_CODE_FAILURE;
        }
        case SUCCESS_TEST: {
            color_printf(YELLOW, "All test have been carried out\n");
            color_printf(GREEN, "Total: %d, Errors: %d", total, errors);
            return EXIT_CODE_SUCCESS;
        }
        default: {
            color_printf(RED, "Unexpected return value from test function\n");
            return EXIT_CODE_FAILURE;
        }
    }
}
