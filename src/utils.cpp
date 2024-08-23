#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "quadratic.h"
#include "utils.h"

static roots_number_t int_to_roots_number(int n);

bool is_zero(double num) {
    if(fabs(num) < EPSILON) return true;
    return false;
}

compare_state_t compare_with_zero(double a) {
    if(is_zero(a) == true) return EQUALS;
    if(a > 0.0) return BIGGER;
    return LESS;
}

reading_state_t read_lines_number(FILE *file, int *out) {
    if(file == NULL)
        return READING_ERROR;
    if(ftell(file) != 0)
        return READING_ERROR;

    if(fscanf(file, "%d", out) != 1)
        return READING_ERROR;

    return READING_SUCCESS;
}

reading_state_t read_line(FILE *file, quadratic_equation_t *equation) {
    if(file == NULL)
        return READING_ERROR;
    if(ftell(file) == 0)
        return READING_ERROR;

    int roots_number_numeric = 0;

    if(fscanf(file, "%lg %lg %lg %lg %lg %d\n",
    &equation->a, &equation->b, &equation->c,
    &equation->x1, &equation->x2, &roots_number_numeric) != 6)
        return READING_ERROR;

    equation->number = int_to_roots_number(roots_number_numeric);
    if(equation->number == NOT_SOLVED)
        return READING_ERROR;

    return READING_SUCCESS;
}

static roots_number_t int_to_roots_number(int n){
    switch(n) {
        case -1: return NOT_SOLVED;
        case -2: return INF_ROOTS;
        case 0: return NO_ROOTS;
        case 1: return ONE_ROOT;
        case 2: return TWO_ROOTS;
        default: return NOT_SOLVED;
    }
}
