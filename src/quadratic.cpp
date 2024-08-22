/**
===============================================================================================================================
    @file    quadratic.cpp
    @brief   Quadratic equations library, allows to input coefficients, solve equation and print out roots
    @author  Artem Neskorodov
    @date    22.08.2024
===============================================================================================================================
*/



#include <quadratic.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <utils.h>
#include <colors.h>


/**
===============================================================================================================================
    @brief   Maximum length of user input
===============================================================================================================================
*/
const int MAX_INPUT_LENGTH = 32;


//---------------------------------
//      FUNCTIONS PROTOTYPES
//---------------------------------
static getting_coeffs_state_t get_number(char symbol, double *out);
static solving_state_t solve_linear(quadratic_equation_t *equation);
static void go_to_end_console(void);
static bool try_get_double(double *out);
static bool check_if_exit(void);



/**
===============================================================================================================================
    @brief   - Writes coefficients, that user typed, in in equation struct

    @details - Function asks user to type in coefficients of quadratic equation.\n
             - It will ask user until he/she type in valid double number or word "exit".\n
             - Function returns:\n
                + GETTING_SUCCESS (in case of successful writing users inputs to equation fields).\n
                + GETTING_EXIT (in case of typed in word "exit" by user).\n
                + There are no other return values.

    @param   [out] equation pointer to quadratic equation struct

    @return  Enum, represanting the bihavior of function run
===============================================================================================================================
*/
getting_coeffs_state_t get_coefficients(quadratic_equation_t *equation) {
    assert(equation != NULL);

    equation->number = NOT_SOLVED;

    equation->x1 = equation->x2 = 0;

    printf(CYAN("(\"exit\" to leave)\n")
           "Type in coefficients for equation " YELLOW("ax^2 + bx + c == 0:\n"));

    if(get_number('a', &equation->a) == GETTING_EXIT)
        return GETTING_EXIT;

    if(get_number('b', &equation->b) == GETTING_EXIT)
        return GETTING_EXIT;

    if(get_number('c', &equation->c) == GETTING_EXIT)
        return GETTING_EXIT;

    return GETTING_SUCCESS;
}



/**
===============================================================================================================================
    @brief   - Solves quadratic equation in form ax^2 + bx + c == 0

    @details - Function gets coefficients from fields a, b and c of equation struct.\n
             - It solves linear equation if a == 0.\n
             - Function returns:\n
                + SOLVING_SUCCESS (if solved equation successfully)\n
                + SOLVING_ERROR (in case of unexpected error)\n
                + INVALID_COEFFICIENTS (if one of coefficients is not finite number)\n
                + There are no other return values\n
             - Function writes roots in fields 'x1' and 'x2' of equation structure.\n
             - Number of roots is written in field 'number':\n
                + NOT_SOLVED can occure there only before structure was sent to solve_quadratic() or in case of error\n
                + NO_ROOTS if equation has no real roots\n
                + ONE_ROOT if equation has one real root\n
                + TWO_ROOTS if equation has two real roots\n
                + INF_ROOTS if equation has infinitely many roots

    @param   [out] equation Point to equation structure, containing coefficients of quadratic equation

    @return  Enum, representing the behavior of function run
===============================================================================================================================
*/
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



/**
===============================================================================================================================
    @brief   - Prints roots of quadratic equation in console

    @param   [in]  equation Pointer to equation struct, that is already solved
===============================================================================================================================
*/
void print_quadratic_result(const quadratic_equation_t *equation) {
    assert(equation != NULL);
    printf("Equation " YELLOW("%lgx^2 + %lgx + %lg") ":\n",
           equation->a, equation->b, equation->c);
    switch(equation->number) {
        case NOT_SOLVED: {
            printf(RED("Not solved yet, try to run solve_equation(...)\n"));
            return ;
        }
        case NO_ROOTS: {
            printf(PURPLE("Does not have real roots\n"));
            return ;
        }
        case ONE_ROOT: {
            printf("Has one root: " PURPLE("x = %lg\n"), equation->x1);
            return ;
        }
        case TWO_ROOTS: {
            printf("Has two roots: " PURPLE("x1 = %lg, x2 = %lg\n"), equation->x1, equation->x2);
            return ;
        }
        case INF_ROOTS: {
            printf(PURPLE("Has infinitely many roots\n"));
            return ;
        }
        default: {
            printf(RED("Something went wrong while trying to print out result\n"));
            return ;
        }
    }
}



/**
===============================================================================================================================
    @brief   - Function tryes to get one number from user.

    @details - Tries to get number until one of cases:\n
                + User types in valid double value that can be understood by scanf.\n
                + User types in word "exit".\n
             - Function returns:\n
                + GETTING_SUCCESS (in case of typing in number)\n
                + GETTING_EXIT (in case of typing in "exit")\n
                + There are no other return values\n

    @param   [in]  symbol Letter naming coefficient in quadratic equation('a' for x^2, b for x^1 and c for x^0).
    @param   [out] out Pointer to double, which will contain user coefficient.

    @return  Enum represanting meaning of user input
===============================================================================================================================
*/
getting_coeffs_state_t get_number(char symbol, double *out) {
    while(true) {
        printf("%c = ", symbol);

        if(try_get_double(out) == true)
            return GETTING_SUCCESS;

        if(check_if_exit() == true)
            return GETTING_EXIT;

        printf("Invalid input\n");
    }
}



/**
===============================================================================================================================
    @brief   - Function solves linear equation bx + c == 0, where b and c are fields of equation struct

    @details - Function returns:
                + SOLVING_SUCCESS (if solved equation successfully)\n
                + SOLVING_ERROR (in case of unexpected error)\n
                + INVALID_COEFFICIENTS (if one of coefficients is not finite number)\n
                + There are no other return values\n
             - Function write root to fields 'x1' and 'x2' of equation struct.\n
             - Function writes number of roots in field 'number':\n
                + NOT_SOLVED can occure there only before structure was sent to solve_quadratic() or in case of error\n
                + NO_ROOTS if equation has no real roots\n
                + ONE_ROOT if equation has one real root\n
                + INF_ROOTS if equation has infinitely many roots\n
                + TWO_ROOTS can't occure in case of linear equation

    @param   [out] equation Pointer to equation struct, that is already solved

    @return  Enum, representing the behavior of function run
===============================================================================================================================
*/
solving_state_t solve_linear(quadratic_equation_t *equation) {
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



/**
===============================================================================================================================
    @brief   - Function moves pointer in console to last character

    @details - Moves pointer until meets:\n
                + New line character\n
                + End Of File\n

===============================================================================================================================
*/
void go_to_end_console(void) {
    int c = getchar();
    while(c != EOF && c != '\n') c = getchar();
}



/**
===============================================================================================================================
    @brief   - Tries to scanf number that user types in

    @details - Uses scanf() with '%lg' format

    @param   [out] out Pointer to double, which will contain user coefficient.

    @return  TRUE in case of success and FALSE in other
===============================================================================================================================
*/
bool try_get_double(double *out) {
    if(scanf("%lg", out) != 1) return false;
    go_to_end_console();
    return true;
}



/**
===============================================================================================================================
    @brief   - Checks if user typed in word "exit"

    @details - Scanf's input with '%s' format.\n
             - After run moves pointer in console to last character

    @return  TRUE if there is word "exit" in console and FALSE if not
===============================================================================================================================
*/
bool check_if_exit(void) {
    char string[MAX_INPUT_LENGTH] = {};
    scanf("%s", string);
    go_to_end_console();
    if(strcmp(string, "exit") == 0)
        return true;

    else
        return false;
}
