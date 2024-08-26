/**
===============================================================================================================================
    @file    handlers.h
    @brief   Function to handle users.
    @date    23.08.2024
    @author  Artem Neskorodov
    @link    https://vk.com/neskorodovartem

===============================================================================================================================
*/

#include <stdio.h>
#include "colors.h"
#include "handle_flags.h"
#include "handlers.h"
#include "quadratic.h"
#include "quadratic_tests.h"
#include "custom_assert.h"

exit_code_t handle_help(const int /*argc*/, const char */*argv*/[]) {
    color_printf(PURPLE_TEXT, false, DEFAULT_BACKGROUND, "\t'--help'");
    color_printf(DEFAULT_TEXT, false, DEFAULT_BACKGROUND, " for help\n");
    color_printf(PURPLE_TEXT, false, DEFAULT_BACKGROUND, "\t'--solve'");
    color_printf(DEFAULT_TEXT, false, DEFAULT_BACKGROUND, " to type in and solve equation\n");
    color_printf(PURPLE_TEXT, false, DEFAULT_BACKGROUND, "\t'--test (filename)'");
    color_printf(DEFAULT_TEXT, false, DEFAULT_BACKGROUND, " to run tests\n");
    color_printf(PURPLE_TEXT, false, DEFAULT_BACKGROUND, "\t''");
    color_printf(DEFAULT_TEXT, false, DEFAULT_BACKGROUND, " is considered as ");
    color_printf(PURPLE_TEXT, false, DEFAULT_BACKGROUND, "'--solve'\n");
    return EXIT_CODE_SUCCESS;
}

exit_code_t handle_solve(const int /*argc*/, const char */*argv*/[]) {
    quadratic_equation_t equation = {.number = NOT_SOLVED};

    //Getting coefficients from user
    switch(get_coefficients(&equation)){
        case GETTING_EXIT: {
            color_printf(CYAN_TEXT, false, DEFAULT_BACKGROUND, "Stop using Vietta\n");
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
            color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "Your input was invalid, unable to solve equation :(\n");
            return EXIT_CODE_FAILURE;
        }
        case SOLVING_ERROR: {
            color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "Caught unexpected error while solving\n");
            return EXIT_CODE_FAILURE;
        }
        case SOLVING_SUCCESS: {
            print_quadratic_result(&equation);
            return EXIT_CODE_SUCCESS;
        }
        default: {
            color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "solve_quadratic() returned unexpected result\n");
            return EXIT_CODE_FAILURE;
        }
    }
}

exit_code_t handle_test(const int argc, const char *argv[]) {
    C_ASSERT(argv != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(argc >= 0,    EXIT_CODE_FAILURE);
    int total = 0, errors = 0;
    const char *filename = DEFAULT_TEST_FILE_NAME;
    if(argc >= 3) {
        filename = argv[2];
    }
    switch(test_solving_quadratic(&total, &errors, filename)) {
        case NO_SUCH_FILE: {
            color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "There is no file \"%s\"\n", filename);
            return EXIT_CODE_FAILURE;
        }
        case INVALID_LINES:{
            color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "Tests file is invalid\n");
            return EXIT_CODE_FAILURE;
        }
        case SUCCESS_TEST: {
            color_printf(YELLOW_TEXT, false, DEFAULT_BACKGROUND, "All test have been carried out\n");
            color_printf(GREEN_TEXT, false, DEFAULT_BACKGROUND, "Total: %d, Errors: %d", total, errors);
            return EXIT_CODE_SUCCESS;
        }
        case TEST_ERROR: {
            color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "Unexpected return value from test function\n");
            return EXIT_CODE_FAILURE;
        }
        default: {
            color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "Unexpected return value from test function\n");
            return EXIT_CODE_FAILURE;
        }
    }
}
