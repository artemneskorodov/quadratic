#include "utils.h"
#include <math.h>
#include "quadratic.h"

bool is_zero(double num) {
    if(fabs(num) < EPSILON) return true;
    return false;
}

compare_state_t compare_with_zero(double a) {
    if(is_zero(a) == true) return EQUALS;
    if(a > 0.0) return BIGGER;
    return LESS;
}
