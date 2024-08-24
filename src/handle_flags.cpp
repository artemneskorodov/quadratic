/**
===============================================================================================================================
    @file    handle_flags.cpp
    @brief   Handling flags pushed from console
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

enum flag_t {
    PROHIBITED_AMOUNT_FLAGS,
    HELP_FLAG,
    TEST_FLAG,
    SOLVE_FLAG,
    UNKNOWN_FLAG
};

static flag_t get_flag(int argc, const char *argv[]);
static int handle_amount_flag(void);
static int handle_unknown_flag(const char *flag);
static int handle_help_flag(void);
static int handle_solve_flag(void);
static int handle_test_flag(void);
static int handle_error(void);

/**
===============================================================================================================================
    @brief   - Compares flag with supported.

    @details - Supported flags are:\n
                + --help\n
                + --test\n
                + --solve\n
             - Typing in more than one flag is not supported.

    @param   [in]  argc               Number of strings in argv[].
    @param   [in]  argv               Array of strings with flags and additional system information

    @return  Enumerator representing flag
===============================================================================================================================
*/
flag_t get_flag(int argc, const char *argv[]) {
    C_ASSERT(argv != NULL);
    if(argc < 1) return PROHIBITED_AMOUNT_FLAGS;
    if(argc == 1)
        return SOLVE_FLAG;

    if(argc > 2)
        return PROHIBITED_AMOUNT_FLAGS;

    if(!strcmp("--help", argv[argc - 1])) return HELP_FLAG;
    if(!strcmp("--solve", argv[argc - 1])) return SOLVE_FLAG;
    if(!strcmp("--test", argv[argc - 1])) return TEST_FLAG;

    return UNKNOWN_FLAG;
}

int handle_user(int argc, const char *argv[]){
    C_ASSERT(argv != NULL);
    switch(get_flag(argc, argv)) {
        case PROHIBITED_AMOUNT_FLAGS: return handle_amount_flag();
        case UNKNOWN_FLAG: return handle_unknown_flag(argv[argc - 1]);
        case HELP_FLAG: return handle_help_flag();
        case TEST_FLAG: return handle_test_flag();
        case SOLVE_FLAG: return handle_solve_flag();
        default: return handle_error();
    }
}

/**
===============================================================================================================================
    @brief   - Handles the case when user typed in more then one flag.

===============================================================================================================================
*/
int handle_amount_flag(void){
    color_printf(RED, "Unexpected amount of flags, you can use only 0 or 1\n");
    color_printf(DEFAULT, "Use flag ");
    color_printf(PURPLE, "'--help'\n");
    return 0;
}

/**
===============================================================================================================================
    @brief   - Handles the case when user typed in unknown flag.

    @param   [in]  arg                String with flag, typed in by user.

===============================================================================================================================
*/
int handle_unknown_flag(const char *arg){
    C_ASSERT(arg != NULL);
    color_printf(RED, "Unknown flag '%s'\n");
    color_printf(DEFAULT, "Use flag ");
    color_printf(PURPLE, "'--help'\n", arg);
    return 0;
}

/**
===============================================================================================================================
    @brief   - Handles help request.

===============================================================================================================================
*/
int handle_help_flag(void){
    color_printf(PURPLE, "\t'--help'");
    color_printf(DEFAULT, " for help\n");
    color_printf(PURPLE, "\t'--solve'");
    color_printf(DEFAULT, " to type in and solve equation\n");
    color_printf(PURPLE, "\t'--test'");
    color_printf(DEFAULT, " to run tests\n");
    color_printf(PURPLE, "\t''");
    color_printf(DEFAULT, " is considered as ");
    color_printf(PURPLE, "'--solve'\n");
    return 0;
}

/**
===============================================================================================================================
    @brief   - Solve mode.

    @details - Asks user to type in coefficients for quadratic equation.\n
             - Solves equation.\n
             - Prints resutlts in console.\n

===============================================================================================================================
*/
int handle_solve_flag(void){
    quadratic_equation_t equation = {.number = NOT_SOLVED};

    //Getting coefficients from user
    if(get_coefficients(&equation) == GETTING_EXIT){
        color_printf(CYAN, "Stop using Vietta\n");
        return 0;
    }

    //Solving quadratic
    switch(solve_quadratic(&equation)) {
        case INVALID_COEFFICIENTS: {
            color_printf(RED, "Your input was invalid, unable to solve equation :(\n");
            return -1;
        }
        case SOLVING_ERROR: {
            color_printf(RED, "Caught unexpected error while solving\n");
            return -2;
        }
        case SOLVING_SUCCESS: {
            print_quadratic_result(&equation);
            return 0;
        }
        default: {
            color_printf(RED, "solve_quadratic() returned unexpected result\n");
            return -3;
        }
    }
}

/**
===============================================================================================================================
    @brief   - Test mode

    @details - Starts tests.\n
             - Prints total number of tests from file "tests.txt" and errors

===============================================================================================================================
*/
int handle_test_flag(void) {
    int total = 0, errors = 0;
    switch(test_solving_quadratic(&total, &errors)) {
        case NO_SUCH_FILE: {
            color_printf(RED, "There is no file \"tests.txt\"\n");
            return 0;
        }
        case INVALID_LINES:{
            color_printf(RED, "Tests file is invalid\n");
            return 0;
        }
        case SUCCESS_TEST: {
            color_printf(YELLOW, "All test have been carried out\n");
            color_printf(GREEN, "Total: %d, Errors: %d", total, errors);
            return 0;
        }
        default: {
            color_printf(RED, "Unexpected return value from test function\n");
            return -1;
        }
    }
}

/**
===============================================================================================================================
    @brief   - Handles error after get_flag() return

===============================================================================================================================
*/
int handle_error(void) {
    color_printf(RED, "Something went wrong\n");
    return EXIT_FAILURE;
}
