#ifndef COMPARE_DOUBLES_H
#define COMPARE_DOUBLES_H

#include <stdbool.h>
#include "quadratic.h"

//Constant for comparing doubles
const double EPSILON = 1e-6;

enum compare_state_t {
    BIGGER,
    LESS,
    EQUALS
};

enum reading_state_t {
    READING_SUCCESS,
    READING_ERROR
};

bool is_zero(double num);
compare_state_t compare_with_zero(double a);
reading_state_t read_lines_number(FILE *file, int *out);
reading_state_t read_line(FILE *file, quadratic_equation_t *equation);

#endif
