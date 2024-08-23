#include "quadratic_tests.h"
#include "quadratic.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include "utils.h"
#include "colors.h"

#define MAX_ROOTS_NUMBER_LENGTH 32

enum test_result_t {
    OK,
    UNEXPECTED_SOLVING_ERROR,
    DIFFERENT_AMOUNT_OF_ROOTS,
    DIFFERENT_ROOTS
};

//---------------------------------
//      FUNCTIONS PROTOTYPES
//---------------------------------
static test_result_t test_solving_one(const quadratic_equation_t *expected, quadratic_equation_t *actual);
static void print_test_result(test_result_t test_result, const quadratic_equation_t *expected, const quadratic_equation_t *actual);
static bool are_roots_same(const quadratic_equation_t *first, const quadratic_equation_t *second);
static void print_different_amount(const quadratic_equation_t *expected, const quadratic_equation_t *actual);
static void print_different_roots(const quadratic_equation_t *expected, const quadratic_equation_t *actual);
static void roots_number_to_string(char *out, roots_number_t number);

int test_solving_quadratic(void) {
    int errors_counter = 0;

    FILE *tests = fopen("tests.txt", "r");

    if(tests == NULL){
        color_printf(RED, "There is no file \"tests.txt\"\n");
        return -1;
    }

    int lines_number = 0;
    if(read_lines_number(tests, &lines_number) == READING_ERROR) {
        color_printf(RED, "Caught error while trying to get lines number\n");
        fclose(tests);
        return -1;
    }

    for(int line = 0; line < lines_number; line++){
        quadratic_equation_t expected = {};

        if(read_line(tests, &expected) == READING_ERROR) {
            color_printf(RED, "Caught error while reading line %d", line);
            fclose(tests);
            return -1;
        }

        quadratic_equation_t actual = {};

        test_result_t result = test_solving_one(&expected, &actual);

        print_test_result(result, &expected, &actual);

    }
    return errors_counter;
}

static test_result_t test_solving_one(const quadratic_equation_t *expected, quadratic_equation_t *actual) {
    assert(expected != NULL);
    assert(actual != NULL);

    actual->number = NOT_SOLVED;
    actual->a = expected->a;
    actual->b = expected->b;
    actual->c = expected->c;

    if(solve_quadratic(actual) != SOLVING_SUCCESS)
        return UNEXPECTED_SOLVING_ERROR;

    if(expected->number != actual->number)
        return DIFFERENT_AMOUNT_OF_ROOTS;

    if(are_roots_same(expected, actual) != true)
        return DIFFERENT_ROOTS;

    return OK;
}

static void print_test_result(test_result_t test_result,
                              const quadratic_equation_t *expected,
                              const quadratic_equation_t *actual) {

    color_printf(DEFAULT, "For equation ");
    color_printf(YELLOW, "%lgx^2 + %lgx + %lg",
        expected->a, expected->b, expected->c);
    color_printf(DEFAULT, ":\n");

    switch(test_result) {
        case OK: {
            color_printf(GREEN, "Test went successfully\n");
            break;
        }
        case UNEXPECTED_SOLVING_ERROR: {
            color_printf(RED, "Caught unexpected solving error");
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
        default: {
            color_printf(RED, "Test function returned unexpected exit status\n");
        }
    }
    color_printf(DEFAULT, "------------------------\n");
}

static bool are_roots_same(const quadratic_equation_t *first, const quadratic_equation_t *second) {
    assert(first != NULL);
    assert(second != NULL);

    if(first->number != second->number)
        return false;

    if(first->number == ONE_ROOT){
        if(is_zero(first->x1 - second->x1)) return true;
        return false;
    }

    if(first->number == TWO_ROOTS){
        if(is_zero(first->x1 - second->x1) && is_zero(first->x2 - second->x2)) return true;
        if(is_zero(first->x1 - second->x2) && is_zero(first->x2 - second->x1)) return true;
        return false;
    }
    return true;
}

static void print_different_amount(const quadratic_equation_t *expected, const quadratic_equation_t *actual){
    char string_number_expected[MAX_ROOTS_NUMBER_LENGTH] = {};
    char string_number_actual[MAX_ROOTS_NUMBER_LENGTH] = {};

    roots_number_to_string(string_number_expected, expected->number);
    roots_number_to_string(string_number_actual, actual->number);
    color_printf(RED, "Got different amount of roots\n");
    color_printf(YELLOW, "Expected: %s, actual: %s\n",
           string_number_expected, string_number_actual);
}

static void print_different_roots(const quadratic_equation_t *expected, const quadratic_equation_t *actual) {
    switch(expected->number){
        case NOT_SOLVED: {
            color_printf(RED, "expected is not solved\n");
            return ;
        }
        case NO_ROOTS: {
            color_printf(RED, "expected has no roots\n");
            return ;
        }
        case ONE_ROOT: {
            color_printf(RED, "Got different roots\n");
            color_printf(YELLOW, "Expected: x = ");
            color_printf(PURPLE, "%lg", expected->x1);
            color_printf(YELLOW, ", actual: x = ");
            color_printf(PURPLE, "%lg\n", actual->x1);
            return ;
        }
        case TWO_ROOTS: {
            color_printf(RED, "Got diggerent roots\n");
            color_printf(YELLOW, "Expected: x1 = ");
            color_printf(PURPLE, "%lg", expected->x1);
            color_printf(YELLOW, ", x2 = ");
            color_printf(PURPLE, "%lg,\n", expected->x2);
            color_printf(YELLOW, "Actual: x1 = ");
            color_printf(PURPLE, "%lg", actual->x1);
            color_printf(YELLOW, ", x2 = ");
            color_printf(PURPLE, "%lg\n", actual->x2);
            return ;
        }
        case INF_ROOTS: {
            color_printf(RED, "expected has inf roots\n");
            return ;
        }
        default: {
            color_printf(RED, "Unexpected roots number\n");
        }
    }
}

static void roots_number_to_string(char *out, roots_number_t number) {
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
        }
    }
}
