/**
===============================================================================================================================
    @file    quadratic_tests.cpp
    @brief   Testing solve_quadratic(...).
    @date    23.08.2024
    @author  Artem Neskorodov
    @link    https://vk.com/neskorodovartem

===============================================================================================================================
*/
#include "quadratic_tests.h"
#include "quadratic.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "utils.h"
#include "colors.h"
#include "custom_assert.h"

/**
===============================================================================================================================
    @brief   - The maximum length of roots number as string.

===============================================================================================================================
*/
static const int MAX_ROOTS_NUMBER_LENGTH = 32;

const char *DEFAULT_TEST_FILE_NAME = "tests.txt";

enum test_result_t {
    OK,
    UNEXPECTED_SOLVING_ERROR,
    DIFFERENT_AMOUNT_OF_ROOTS,
    DIFFERENT_ROOTS,
    TEST_FAILURE
};

static test_result_t run_test(const quadratic_equation_t *expected, quadratic_equation_t *actual);
static void print_test_result(test_result_t test_result, const quadratic_equation_t *expected, const quadratic_equation_t *actual);
static bool compare_roots(const quadratic_equation_t *first, const quadratic_equation_t *second);
static void print_different_amount(const quadratic_equation_t *expected, const quadratic_equation_t *actual);
static void print_different_roots(const quadratic_equation_t *expected, const quadratic_equation_t *actual);
static void roots_number_to_string(char *out, roots_number_t number);

test_state_t test_solving_quadratic(int *tests_number, int *errors_number, const char *filename) {
    C_ASSERT(tests_number  != NULL, TEST_ERROR);
    C_ASSERT(errors_number != NULL, TEST_ERROR);
    C_ASSERT(filename      != NULL, TEST_ERROR);

    *errors_number = 0;
    *tests_number = 0;

    FILE *tests = fopen(filename, "r");

    if(tests == NULL)
        return NO_SUCH_FILE;

    quadratic_equation_t expected = {};
    reading_state_t reading_state = read_expected_line(tests, &expected);

    while(reading_state == READING_SUCCESS) {
        quadratic_equation_t actual = {};
        test_result_t test_result = run_test(&expected, &actual);

        if(test_result != OK)
            *errors_number += 1;

        print_test_result(test_result, &expected, &actual);
        reading_state = read_expected_line(tests, &expected);
        *tests_number += 1;
    }

    if(reading_state == READING_ERROR)
        return INVALID_LINES;

    return SUCCESS_TEST;
}

/**
===============================================================================================================================
    @brief   - Runs one equation from file "tests.txt" and checks answer.

    @details - Pushes fields a, b and c from expected to actual and then runs solve_quadratic(actual)\.n
             - Compares expected answers and actual answers:\n
                + Firstly checks the equality of amount of roots.\n
                + Secondly checks roots depending on amount of them:\n
                    + Skips comparison if equation has zero or infinitly many roots.\n
                    + Checks equality of x1 if equation has one root.\n
                    + Checks equality of x1 and x2 if eqaution has two roots.\n
             - Function returns:\n
                + UNEXPECTED_SOLVING_ERROR if solve_quadratic did not return SOLVING_SUCCESS.\n
                + DIFFETENT_AMOUT_OF_ROOTS if there is different amount of roots.\n
                + DIFFERENT_ROOTS if roots are different.\n
                + OK if test is passed.\n
             - The roots are considired to be equal if iz_zero(x_actual - x_expected) == true.

    @param   [in]  expected           Pointer to expected equation structure
    @param   [in]  actual             Pointer to quadratic structure where solve_quadratic(...) will work.

    @return  Error(or success) code.

===============================================================================================================================
*/
test_result_t run_test(const quadratic_equation_t *expected, quadratic_equation_t *actual) {
    C_ASSERT(expected != NULL, TEST_FAILURE);
    C_ASSERT(actual   != NULL, TEST_FAILURE);

    actual->number = NOT_SOLVED;
    actual->a      = expected->a;
    actual->b      = expected->b;
    actual->c      = expected->c;

    if(solve_quadratic(actual) != SOLVING_SUCCESS)
        return UNEXPECTED_SOLVING_ERROR;

    if(expected->number != actual->number)
        return DIFFERENT_AMOUNT_OF_ROOTS;

    if(compare_roots(expected, actual) != true)
        return DIFFERENT_ROOTS;

    return OK;
}

/**
===============================================================================================================================
    @brief   - Prints result of test to console.

    @param   [in]  test_result        The test result returned by run_test(...).
    @param   [in]  expected           Pointer to structure of expected values.
    @param   [in]  actual             Pointer to structure with actual values.

===============================================================================================================================
*/
void print_test_result(test_result_t test_result,
                       const quadratic_equation_t *expected,
                       const quadratic_equation_t *actual) {

    C_ASSERT(expected != NULL, );
    C_ASSERT(actual   != NULL, );

    color_printf(DEFAULT_TEXT, false, DEFAULT_BACKGROUND, "For equation ");
    color_printf(YELLOW_TEXT, false, DEFAULT_BACKGROUND, "%lgx^2 + %lgx + %lg",
                 expected->a, expected->b, expected->c);
    color_printf(DEFAULT_TEXT, false, DEFAULT_BACKGROUND, ":\n");

    switch(test_result) {
        case OK: {
            color_printf(GREEN_TEXT, false, DEFAULT_BACKGROUND, "Test went successfully\n");
            break;
        }
        case UNEXPECTED_SOLVING_ERROR: {
            color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "Caught unexpected solving error");
            break;
        }
        case DIFFERENT_AMOUNT_OF_ROOTS: {
            print_different_amount(expected, actual);
            break;
        }
        case DIFFERENT_ROOTS: {
            print_different_roots(expected, actual);
            break;
        }
        case TEST_FAILURE: {
            break;
        }
        default: {
            color_printf(RED_TEXT, false, DEFAULT_BACKGROUND,  "Test function returned unexpected exit status\n");
            break;
        }
    }
    color_printf(DEFAULT_TEXT, false, DEFAULT_BACKGROUND, "------------------------\n");
}

/**
===============================================================================================================================
    @brief   - Compares roots depending on their amount.

    @details - If amounts of roots are different roots are NOT the same.\n
             - If there is zero or infinitely many roots they are the same.\n
             - Checks only x1 if there is one root.\n
             - Checks x1 and x2 if there is two roots.\n

    @param   [in]  first              Pointer to first equation structure to be compared.
    @param   [in]  second             Pointer to second equation structure to be compared.

    @return  True if roots are the same and false if not.

===============================================================================================================================
*/
bool compare_roots(const quadratic_equation_t *first, const quadratic_equation_t *second) {
    C_ASSERT(first  != NULL, false);
    C_ASSERT(second != NULL, false);

    if(first->number != second->number)
        return false;

    if(first->number == ONE_ROOT){
        if(is_equal(first->x1, second->x1)) return true;
        return false;
    }

    if(first->number == TWO_ROOTS){
        if(is_equal(first->x1, second->x1) && is_equal(first->x2, second->x2))
            return true;
        if(is_equal(first->x1, second->x2) && is_equal(first->x2, second->x1))
            return true;

        return false;
    }
    return true;
}

/**
===============================================================================================================================
    @brief   - Functions to print in console that there is different amount of roots in structures.

    @param   [in]  expected           Pointer to expected quadratic structure.
    @param   [in]  actual             Pointer to actual quadratic structure.

===============================================================================================================================
*/
void print_different_amount(const quadratic_equation_t *expected, const quadratic_equation_t *actual){
    C_ASSERT(expected != NULL, );
    C_ASSERT(actual   != NULL, );

    char string_number_expected[MAX_ROOTS_NUMBER_LENGTH] = {};
    char string_number_actual[MAX_ROOTS_NUMBER_LENGTH]   = {};

    roots_number_to_string(string_number_expected, expected->number);
    roots_number_to_string(string_number_actual, actual->number);
    color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "Got different amount of roots\n");
    color_printf(YELLOW_TEXT, false, DEFAULT_BACKGROUND, "Expected: %s, actual: %s\n",
                 string_number_expected, string_number_actual);
}

/**
===============================================================================================================================
    @brief   - Function to print in console that the roots are different.

    @param   [in]  expected           Pointer to expected quadratic structure.
    @param   [in]  actual             Pointer to actual quadratic structure.

===============================================================================================================================
*/
void print_different_roots(const quadratic_equation_t *expected, const quadratic_equation_t *actual) {
    C_ASSERT(expected != NULL, );
    C_ASSERT(actual   != NULL, );

    switch(expected->number){
        case NOT_SOLVED: {
            color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "expected is not solved\n");
            return ;
        }
        case NO_ROOTS: {
            color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "expected has no roots\n");
            return ;
        }
        case ONE_ROOT: {
            color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "Got different roots\n");
            color_printf(YELLOW_TEXT, false, DEFAULT_BACKGROUND, "Expected: x = ");
            color_printf(PURPLE_TEXT, false, DEFAULT_BACKGROUND, "%lg", expected->x1);
            color_printf(YELLOW_TEXT, false, DEFAULT_BACKGROUND, ", actual: x = ");
            color_printf(PURPLE_TEXT, false, DEFAULT_BACKGROUND, "%lg\n", actual->x1);
            return ;
        }
        case TWO_ROOTS: {
            color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "Got diggerent roots\n");
            color_printf(YELLOW_TEXT, false, DEFAULT_BACKGROUND, "Expected: x1 = ");
            color_printf(PURPLE_TEXT, false, DEFAULT_BACKGROUND, "%lg", expected->x1);
            color_printf(YELLOW_TEXT, false, DEFAULT_BACKGROUND, ", x2 = ");
            color_printf(PURPLE_TEXT, false, DEFAULT_BACKGROUND, "%lg,\n", expected->x2);
            color_printf(YELLOW_TEXT, false, DEFAULT_BACKGROUND, "Actual: x1 = ");
            color_printf(PURPLE_TEXT, false, DEFAULT_BACKGROUND, "%lg", actual->x1);
            color_printf(YELLOW_TEXT, false, DEFAULT_BACKGROUND, ", x2 = ");
            color_printf(PURPLE_TEXT, false, DEFAULT_BACKGROUND, "%lg\n", actual->x2);
            return ;
        }
        case INF_ROOTS: {
            color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "expected has inf roots\n");
            return ;
        }
        default: {
            color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "Unexpected roots number\n");
            return ;
        }
    }
}

/**
===============================================================================================================================
    @brief   - Converts roots number to string.

    @details - If number is 0, 1 or 2, string will be numerical.\n
             - If number is NOT_SOLVED (error occured) string will be "NOT SOLVED".\n
             - If number is INF_ROOTS string will be "INF".\n
             - The minimal length of out is 11, but to avoid errors use constant MAX_ROOTS_NUMBER_LENGTH.

    @param   [out] out                Pointer to string which will contain output.
    @param   [in]  number             Number of roots.

===============================================================================================================================
*/
void roots_number_to_string(char *out, roots_number_t number) {
    C_ASSERT(number != NOT_SOLVED, );
    C_ASSERT(out    != NULL,       );

    switch(number){
        case NOT_SOLVED: {
            sprintf(out, "NOT SOLVED");
            return ;
        }
        case NO_ROOTS: {
            sprintf(out, "0");
            return ;
        }
        case ONE_ROOT: {
            sprintf(out, "1");
            return ;
        }
        case TWO_ROOTS: {
            sprintf(out, "2");
            return ;
        }
        case INF_ROOTS: {
            sprintf(out, "INF");
            return ;
        }
        default: {
            sprintf(out, "ERROR");
            return ;
        }
    }
}
