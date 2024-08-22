#include "../include/quadratic.h"
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "../include/compare_doubles.h"


#define MAX_INPUT_LENGTH 32


//---------------------------------
//      FUNCTIONS PROTOTYPES
//---------------------------------
static getting_coeffs_state_t get_number(char symbol, double *out);
static solving_state_t solve_linear(quadratic_equation_t *equation);
static void go_to_end_console(void);
static bool try_get_double(double *out);
static bool check_if_exit(void);



getting_coeffs_state_t get_coefficients(quadratic_equation_t *equation) {
    assert(equation != NULL);

    equation->number = NOT_SOLVED;

    equation->x1 = equation->x2 = 0;

    printf("(\"exit\" to leave)\n"
           "Type in coefficients for equation ax^2 + bx + c == 0:\n");

    if(get_number('a', &equation->a) == GETTING_EXIT)
        return GETTING_EXIT;

    if(get_number('b', &equation->b) == GETTING_EXIT)
        return GETTING_EXIT;

    if(get_number('c', &equation->c) == GETTING_EXIT)
        return GETTING_EXIT;

    return GETTING_SUCCESS;
}



solving_state_t solve_quadratic(quadratic_equation_t *equation) {
    assert(equation != NULL);

    if(!isfinite(equation->a)) return INVALID_COEFFICIENTS;
    if(!isfinite(equation->b)) return INVALID_COEFFICIENTS;
    if(!isfinite(equation->c)) return INVALID_COEFFICIENTS;

    //equation is linear if a == 0
    if(is_zero(equation->a))
        return solve_linear(equation);


    double discriminant = equation->b * equation->b - 4 * equation->a * equation->c;

    switch(compare_with_zero(discriminant)) {
        case BIGGER: {
            double discriminant_root = sqrt(discriminant);
            equation->number = TWO_ROOTS;
            equation->x1 = (-equation->b - discriminant_root) / (2 * equation->a);
            equation->x2 = (-equation->b + discriminant_root) / (2 * equation->a);
            return SOLVING_SUCCESS;
        }
        case EQUALS: {
            equation->number = ONE_ROOT;
            equation->x1 = equation->x2 = (-equation->b) / (2 * equation->a);
            return SOLVING_SUCCESS;
        }
        case LESS: {
            equation->number = NO_ROOTS;
            return SOLVING_SUCCESS;
        }
        default: {
            equation->number = NOT_SOLVED;
            return SOLVING_ERROR;
        }
    }
}



void print_quadratic_result(const quadratic_equation_t *equation) {
    assert(equation != NULL);
    printf("Equation %lgx^2 + %lgx + %lg:\n",
           equation->a, equation->b, equation->c);
    switch(equation->number) {
        case NOT_SOLVED: {
            printf("Not solved yet, try to run solve_equation(...)\n");
            return ;
        }
        case NO_ROOTS: {
            printf("Does not have real roots\n");
            return ;
        }
        case ONE_ROOT: {
            printf("Has one root: x = %lg\n", equation->x1);
            return ;
        }
        case TWO_ROOTS: {
            printf("Has two roots: x1 = %lg, x2 = %lg\n", equation->x1, equation->x2);
            return ;
        }
        case INF_ROOTS: {
            printf("Has infinitely many roots\n");
            return ;
        }
        default: {
            printf("Something went wrong while trying to print out result\n");
            return ;
        }
    }
}



static getting_coeffs_state_t get_number(char symbol, double *out) {
    while(true) {
        printf("%c = ", symbol);

        if(try_get_double(out) == true)
            return GETTING_SUCCESS;

        if(check_if_exit() == true)
            return GETTING_EXIT;

        printf("Invalid input\n");
    }
}



static solving_state_t solve_linear(quadratic_equation_t *equation) {
    if(is_zero(equation->b)){
        if(is_zero(equation->c)){
            equation->number = INF_ROOTS;
        }
        else{
            equation->number = NO_ROOTS;
        }
    }
    else{
        equation->number = ONE_ROOT;
        equation->x1 = equation->x2 = -equation->c / equation->b;
    }
    return SOLVING_SUCCESS;
}



static void go_to_end_console(void) {
    int c = getchar();
    while(c != EOF && c != '\n') c = getchar();
}



static bool try_get_double(double *out) {
    if(scanf("%lg", out) != 1) return false;
    return true;
}



static bool check_if_exit(void) {
    char string[MAX_INPUT_LENGTH] = {};
    scanf("%s", string);
    go_to_end_console();
    if(strcmp(string, "exit") == 0)
        return true;

    else
        return false;
}
