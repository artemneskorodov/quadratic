/**
===============================================================================================================================
    @file    quadratic.h
    @brief   Header of library to enter, solve and print quadratic equations.
    @date    23.08.2024
    @author  Artem Neskorodov
    @link    https://vk.com/neskorodovartem

===============================================================================================================================
*/

#ifndef QUADRATIC_H
#define QUADRATIC_H

/**
===============================================================================================================================
    @brief Enum represanting amount of roots that quadratic equation has.
===============================================================================================================================
*/
enum roots_number_t {
    NOT_SOLVED = -1,
    NO_ROOTS = 0,
    ONE_ROOT = 1,
    TWO_ROOTS = 2,
    INF_ROOTS = -2
};

/**
===============================================================================================================================
    @brief @brief Structure of quadratic equation.
===============================================================================================================================
*/
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
    GETTING_EXIT,
    GETTING_ERROR
};

/**
===============================================================================================================================
    @brief   - Writes coefficients, that user typed, in in equation struct.

    @details - Function asks user to type in coefficients of quadratic equation.\n
             - It will ask user until he/she type in valid double number or word "exit".\n
             - Function returns:\n
                + GETTING_SUCCESS (in case of successful writing users inputs to equation fields).\n
                + GETTING_EXIT (in case of typed in word "exit" by user).\n
                + There are no other return values.

    @param   [out] equation           Pointer to quadratic equation struct.

    @return  Error (or success) code.

===============================================================================================================================
*/
getting_coeffs_state_t get_coefficients(quadratic_equation_t *equation);

/**
===============================================================================================================================
    @brief   - Solves quadratic equation in form ax^2 + bx + c == 0.

    @details - Function gets coefficients from fields a, b and c of equation struct.\n
             - It solves linear equation if a == 0.\n
             - Function returns:\n
                + SOLVING_SUCCESS (if solved equation successfully).\n
                + SOLVING_ERROR (in case of unexpected error).\n
                + INVALID_COEFFICIENTS (if one of coefficients is not finite number).\n
                + There are no other return values.\n
             - Function writes roots in fields 'x1' and 'x2' of equation structure.\n
             - Number of roots is written in field 'number':\n
                + NOT_SOLVED can occure there only before structure was sent to solve_quadratic() or in case of error.\n
                + NO_ROOTS if equation has no real roots.\n
                + ONE_ROOT if equation has one real root.\n
                + TWO_ROOTS if equation has two real roots.\n
                + INF_ROOTS if equation has infinitely many roots.

    @param   [out] equation           Point to equation structure, containing coefficients of quadratic equation.

    @return  Error (or success) code.

===============================================================================================================================
*/
solving_state_t solve_quadratic(quadratic_equation_t *equation);

/**
===============================================================================================================================
    @brief   - Prints roots of quadratic equation in console.

    @param   [in]  equation           Pointer to equation struct, that is already solved.

===============================================================================================================================
*/
void print_quadratic_result(const quadratic_equation_t *equation);

#endif
