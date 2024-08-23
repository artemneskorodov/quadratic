#ifndef COMPARE_DOUBLES_H
#define COMPARE_DOUBLES_H

#include <stdbool.h>

//Constant for comparing doubles
const double EPSILON = 1e-6;

enum compare_state_t {
    BIGGER,
    LESS,
    EQUALS
};

bool is_zero(double num);
compare_state_t compare_with_zero(double a);

#endif
