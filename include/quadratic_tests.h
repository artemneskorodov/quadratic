/**
===============================================================================================================================
    @file    quadratic_tests.h
    @brief   Header of library, allowing to test solving quadratic equations.
    @date    23.08.2024
    @author  Artem Neskorodov
    @link    https://vk.com/neskorodovartem

===============================================================================================================================
*/

#ifndef QUADRATIC_TESTS_H
#define QUADRATIC_TESTS_H

enum test_state_t {
    NO_SUCH_FILE,
    INVALID_LINES,
    SUCCESS_TEST
};

/**
===============================================================================================================================
    @brief Name of file where program will take expected cases to test solve_quadratic(...).

===============================================================================================================================
*/
extern const char *TESTS_FILE_NAME;

/**
===============================================================================================================================
    @brief   - Run tests of solve_quadratic(...)

    @details - Expected values are taken from file "tests.txt" in form:\n
             'a b c x1 x2 roots_number'.\n
             - Meaning of values:\n
                + a, b and c are coefficients of equation ax^2 + bx + c == 0.\n
                + x1 and x2 are roots of these equation.\n
                + roots_number is number of roots.\n
             - Don't skip roots if number is less then 2.\n
             - Function does not compare second root if roots_number == 1.\n
             - Function does not compare roots if there are zero or infinitely many roots.\n
             - If equation has infinitely many roots type in -2\n
             - Function returns:\n
                + NO_SUCH_FILE if there is no file "tests.txt".\n
                + INVALID_LINES if there is error in "tests.txt".\n
                + SUCCESS_TEST if all test provided in "tests.txt" were carried out.\n

    @param   [out] tests_number       Pointer to ineteger in which function will put total number of tests.
    @param   [out] errors_number      Pointer to integer in which function will put total number of errors.

    @return  Error (or success) code.

===============================================================================================================================
*/
test_state_t test_solving_quadratic(int *tests_number, int *errors_number);

#endif
