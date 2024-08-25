/**
===============================================================================================================================
    @file    utils.h
    @brief   Header of library, allowing to compare doubles with zero and reading test cases for quadratic equation from file.
    @date    23.08.2024
    @author  Artem Neskorodov
    @link    https://vk.com/neskorodovartem

===============================================================================================================================
*/

#ifndef COMPARE_DOUBLES_H
#define COMPARE_DOUBLES_H

#include <stdbool.h>
#include "quadratic.h"

/**
===============================================================================================================================
    @brief   - Constant that is used to compare doubles.

===============================================================================================================================
*/
static const double EPSILON = 1e-9;

enum compare_state_t {
    BIGGER,
    LESS,
    EQUALS
};

enum reading_state_t {
    READING_SUCCESS,
    READING_ERROR,
    READING_END
};

/**
===============================================================================================================================
    @brief   - Compares number to zero.

    @details - Number is considered to be zero if |num| < EPSILON.

    @param   [in] num                 The number to be compared with zero.

    @return  TRUE if |num| < EPSILON and FALSE in other cases.

===============================================================================================================================
*/
bool is_zero(double num);

/**
===============================================================================================================================
    @brief   - Compares double number with zero.

    @details - Number is considered to be zero if is_zero(Number) == true.\n
             - Function returns:\n
                + BIGGER if a > 0.\n
                + EQUALS if is_zero(a) == true.\n
                + LESS if a < 0.\n

    @param   [in] a                   Number to be compared with zero.

    @return  - Enumerator representing the result of somparison.

===============================================================================================================================
*/
compare_state_t compare_with_zero(double a);

/**
===============================================================================================================================
    @brief   - Reads line of tests file.

    @details - Format of reading: "%lg %lg %lg %lg %lg %d".\n
             - Inputs are considered as "a b c x1 x2 n_roots", where.\n
                + a, b and c are coefficients of quadratic equation ax^2 + bx + c == 0.\n
                + x1 and x2 are roots of these equation.\n
                + n_roots is expected number of roots.\n
             - Note that roots are not compared if n_roots == -2 || n_roots == 0\ and only x1 is compared if n_roots == 1.\n
             - Read documentation of test_solve_quadratic().\n
             - Puts result of reading in equation structure.\n
             - Function returns:\n
                + READING_SUCCESS if it read coefficients successfully.\n
                + READING_ERROR if it read coefficients unsiccessfully.\n
                + READING_END if pointer in file reached the EOF.\n

    @param   [in]  file               Pointer to opened file where tests are located.
    @param   [out] equation           Pointer a structure where function puts coefficients, expected roots and roots number.

    @return  Error (or success) code

================================================================================================================================
*/
reading_state_t read_expected_line(FILE *file, quadratic_equation_t *equation);

/**
================================================================================================================================
    @brief   - Checks if double represantation of zero has sign bit set to 1.

    @details - Converts double to integer saving bits and compares it with 0b100000000000000000....

    @param   [in]  number             Number to be checked.

    @return  True if number has binary represantation 1000000..... and false in other cases.

================================================================================================================================
*/
bool is_minus_zero(double number);

#endif
