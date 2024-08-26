/**
===============================================================================================================================
    @file    handlers.h
    @brief   Prototypes of function to handle users.
    @date    23.08.2024
    @author  Artem Neskorodov
    @link    https://vk.com/neskorodovartem

===============================================================================================================================
*/

#ifndef HANDLERS_H
#define HANDLERS_H

/**
===============================================================================================================================
    @brief   - Handles help request.

===============================================================================================================================
*/
exit_code_t handle_help(const int argc, const char *argv[]);

/**
===============================================================================================================================
    @brief   - Solve mode.

    @details - Asks user to type in coefficients for quadratic equation.\n
             - Solves equation.\n
             - Prints resutlts in console.\n

===============================================================================================================================
*/
exit_code_t handle_solve(const int argc, const char *argv[]);

/**
===============================================================================================================================
    @brief   - Test mode.

    @details - Starts tests.\n
             - Prints total number of tests from file "tests.txt" and errors.

===============================================================================================================================
*/
exit_code_t handle_test(const int argc, const char *argv[]);

#endif
