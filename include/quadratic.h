#ifndef QUADRATIC_H
#define QUADRATIC_H

enum roots_number_t {
    NOT_SOLVED = -1,
    NO_ROOTS = 0,
    ONE_ROOT = 1,
    TWO_ROOTS = 2,
    INF_ROOTS = -2
};

struct quadratic_equation_t {
    double a, b, c;
    double x1, x2;
    roots_number_t number;
};

enum solving_state_t {
    SOLVING_SUCCESS,
    SOLVING_ERROR,
    INVALID_COEFFICIENTS
};

enum getting_coeffs_state_t {
    GETTING_SUCCESS,
    GETTING_EXIT
};

getting_coeffs_state_t get_coefficients(quadratic_equation_t *equation);
solving_state_t solve_quadratic(quadratic_equation_t *equation);
void print_quadratic_result(const quadratic_equation_t *equation);

#endif
