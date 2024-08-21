#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>


#define TEST_MODE


const double EPSILON = 1e-6;


typedef enum roots_number_t{
    NOT_SOLVED = -2,
    NO_ROOTS = 0,
    ONE_ROOT = 1,
    TWO_ROOTS = 2,
    INFINITELY_MANY_ROOTS = -1
} roots_number_t;



typedef struct quadratic_equation_t{
    double a, b, c;
    double x1, x2;
    roots_number_t roots_number;
} quadratic_equation_t;



typedef enum getting_coefs_state_t{
    SUCCESSFULLY_GOT,
    EXIT
} getting_coefs_state_t;



typedef enum solving_state_t{
    SUCCESSFULLY_SOLVED,
    INVALID_INPUT,
    SOLVING_ERROR
} solving_state_t;



typedef enum test_result_t{
    OK,
    DIFFERENT_ROOTS,
    NOT_SAME_ROOTS_NUMBER,
    UNEXPECTED_SOLVING_ERROR
} test_result_t;



typedef enum compare_result_t{
    EQUALS,
    BIGGER,
    LESS
} compare_result_t;



getting_coefs_state_t get_coefficients(quadratic_equation_t *equation);
solving_state_t solve_quadratic(quadratic_equation_t *equation);
void print_quadratic_roots(quadratic_equation_t *equation);
compare_result_t compare_doubles(double a, double b);
solving_state_t solve_linear(quadratic_equation_t *equation);
getting_coefs_state_t get_number(char symbol, double *out);
test_result_t test_solve_quadratic(const quadratic_equation_t *expected, quadratic_equation_t *actual);
int test_solving(void);
void go_to_end_in_console(void);
bool check_if_exit(void);
bool try_get_double(double *out);
bool same_roots(const quadratic_equation_t *first, const quadratic_equation_t *second);
void roots_number_to_string(char *output, roots_number_t number);
void print_different_roots_error(const quadratic_equation_t *expected, const quadratic_equation_t *actual);
void print_test_results(const quadratic_equation_t *expected, const quadratic_equation_t *actual, test_result_t result);
bool is_zero(double num);



int main(void) {

    //Test mode (define in top of source code)
    #ifdef TEST_MODE
    printf("Errors: %d\n", test_solving());
    #else
    quadratic_equation_t equation = {.roots_number = NOT_SOLVED};

    //Getting coefficients from user
    if(get_coefficients(&equation) == EXIT){
        printf("Stop using Vietta\n");
        return 0;
    }

    //Solving equation
    switch(solve_quadratic(&equation)) {
        case SOLVING_ERROR: {
            printf("Caught unexpected error while solving\n");
            return -1;
        }
        case INVALID_INPUT: {
            printf("Invalid input\n");
            return -2;
        }
        case SUCCESSFULLY_SOLVED: {
            print_quadratic_roots(&equation);
            return 0;
        }
        default: {
            printf("solve_quadratic() returned unexpected value\n");
            return -3;
        }
    }
    #endif
}



//!-----------------------------
//!@param symbol [in] letter, representing needed coefficient
//!@param out [out] pointer to result
//!@return state(SUCCESS when got number and EXIT if user typed in "exit")
//!@note it is nessesary to check the result, because in case of exit, out can't be considered as valid
//!----------------------------
getting_coefs_state_t get_number(char symbol, double *out) {
    assert(out != NULL);
    while(true) {
        printf("%c = ", symbol);

        if(try_get_double(out) == true) return SUCCESSFULLY_GOT;

        if(check_if_exit() == true) return EXIT;

        printf("Invalid input\n");
    }
    return EXIT;
}



//!---
//!@
//!---
bool try_get_double(double *out) {
    if(scanf("%lf", out) == 1){
        go_to_end_in_console();
        return true;
    }
    return false;
}



bool check_if_exit(void) {
    char str[128] = {};
    scanf("%s", str);
    go_to_end_in_console();
    if(strcmp(str, "exit") == 0) return true;
    return false;
}



void go_to_end_in_console(void) {
    int c = getchar();
    while(c != EOF && c != '\n') c = getchar();
}



getting_coefs_state_t get_coefficients(quadratic_equation_t *equation) {
    assert(equation != NULL);
    printf("Type in coefficients for equation ax^2 + bx + c == 0: \n");
    printf("(Type exit to leave)\n");
    if(get_number('a', &equation->a) == EXIT)
        return EXIT;
    if(get_number('b', &equation->b) == EXIT)
        return EXIT;
    if(get_number('c', &equation->c) == EXIT)
        return EXIT;
    return SUCCESSFULLY_GOT;
}



solving_state_t solve_quadratic(quadratic_equation_t *equation) {
    assert(equation != NULL);

    if(!isfinite(equation->a)) return INVALID_INPUT;
    if(!isfinite(equation->b)) return INVALID_INPUT;
    if(!isfinite(equation->c)) return INVALID_INPUT;

    //Linear if a == 0
    if(is_zero(equation->a))
        return solve_linear(equation);

    double discriminant = equation->b * equation->b - 4 * equation->a * equation->c;

    switch(compare_doubles(discriminant, 0.0)) {
        //Two roots when D > 0
        case BIGGER: {
            equation->roots_number = TWO_ROOTS;
            double discriminant_root = sqrt(discriminant);
            equation->x1 = (-equation->b - discriminant_root) / (2 * equation->a);
            equation->x2 = (-equation->b + discriminant_root) / (2 * equation->a);
            break;
        }
        //One root when D == 0
        case EQUALS: {
            equation->roots_number = ONE_ROOT;
            equation->x1 = equation->x2 = (-equation->b) / (2 * equation->a);
            break;
        }
        //No roots when D < 0
        case LESS: {
            equation->roots_number = NO_ROOTS;
            break;
        }
        //Something went wrong while comparing doubles
        default: {
            equation->roots_number = NOT_SOLVED;
            return SOLVING_ERROR;
        }
    }
    return SUCCESSFULLY_SOLVED;
}



void print_quadratic_roots(quadratic_equation_t *equation) {
    assert(equation != NULL);

    printf("Equation %lgx^2 + %lgx + %lg == 0 ", equation->a, equation->b, equation->c);
    switch(equation->roots_number){
        case NOT_SOLVED: {
            printf("is not solved yet.\n");
            break;
        }
        case NO_ROOTS: {
            printf("has no roots.\n");
            break;
        }
        case ONE_ROOT: {
            printf("has one roots: x = %lg.\n", equation->x1);
            break;
        }
        case TWO_ROOTS: {
            printf("has two roots: x1 = %lg, x2 = %lg.\n", equation->x1, equation->x2);
            break;
        }
        case INFINITELY_MANY_ROOTS: {
            printf("has infinilely many roots.\n");
            break;
        }
        default: {
            printf("had some unexpected troubles while solving\nCheck if it was initialized before using.\n");
        }
    }
}



compare_result_t compare_doubles(double a, double b) {
    if(fabs(a - b) < EPSILON) return EQUALS;
    if(a < b) return LESS;
    return BIGGER;
}



solving_state_t solve_linear(quadratic_equation_t *equation) {
    assert(equation != NULL);
    if(is_zero(equation->b)){
        if(is_zero(equation->c)){
            //0x + 0 == 0 -> x is real
            equation->roots_number = INFINITELY_MANY_ROOTS;
        }
        else{
            //0x + c == 0 (c != 0) -> no roots
            equation->roots_number = NO_ROOTS;
        }
    }
    else{
        //ax + b == 0 (a != 0) -> x = -b / a
        equation->roots_number = ONE_ROOT;
        equation->x1 = equation->x2 = (-equation->c) / equation->b;
    }
    return SUCCESSFULLY_SOLVED;
}



test_result_t test_solve_quadratic(const quadratic_equation_t *expected, quadratic_equation_t *actual) {
    actual->a = expected->a;
    actual->b = expected->b;
    actual->c = expected->c;

    if(solve_quadratic(actual) != SUCCESSFULLY_SOLVED)
        return UNEXPECTED_SOLVING_ERROR;

    if(expected->roots_number != actual->roots_number)
        return NOT_SAME_ROOTS_NUMBER;

    if(same_roots(expected, actual) == false)
        return DIFFERENT_ROOTS;

    return OK;
}



bool same_roots(const quadratic_equation_t *first, const quadratic_equation_t *second) {
    if(first->roots_number != second->roots_number)
        return false;

    if(first->roots_number == ONE_ROOT && !is_zero(first->x1 - second->x1))
        return true;

    if(first->roots_number == TWO_ROOTS){
        if(is_zero(first->x1 - second->x1) && is_zero(first->x2 - second->x2)) return true;
        if(is_zero(first->x1 - second->x2) && is_zero(first->x2 - second->x1)) return true;
        return false;
    }
    return true;
}



int test_solving(void) {
    int counter = 0;


    const quadratic_equation_t tests[] = {
        {.a = 1, .b = 2, .c = 1, .x1 = -1, .x2 = -1, .roots_number = ONE_ROOT},
        {.a = 0.5, .b = 0.5, .c = 0.5, .x1 = 0, .x2 = 0, .roots_number = NO_ROOTS},
        {.a = 3.0, .b = 5.0, .c = 2.0, .x1 = -1, .x2 = -0.66666666667, .roots_number = TWO_ROOTS},
        {.a = 2, .b = -12, .c = 13.5, .x1 = 1.5, .x2 = 4.5, .roots_number = TWO_ROOTS},
        {.a = 0, .b = 0, .c = 0, .x1 = 0, .x2 = 0, .roots_number = INFINITELY_MANY_ROOTS},
        {.a = 1, .b = -19.96, .c = -105.462, .x1 = 24.3, .x2 = -4.34, .roots_number = TWO_ROOTS},
        {.a = 1, .b = -19.96, .c = -105.462, .x1 = -4.34, .x2 = 24.3, .roots_number = TWO_ROOTS},
    };


    for(size_t i = 0; i < sizeof(tests) / sizeof(quadratic_equation_t); i++) {
        quadratic_equation_t actual = {};
        test_result_t result = test_solve_quadratic(tests + i, &actual);
        print_test_results(&tests[i], &actual, result);
        if(result != OK) counter++;
    }


    return counter;
}



void print_test_results(const quadratic_equation_t *expected, const quadratic_equation_t *actual, test_result_t result) {
    switch(result) {
        case OK: {
            printf("Successfully solved equation %lgx^2 + %lgx + %lg\n", expected->a, expected->b, expected->c);
            break ;
        }
        case UNEXPECTED_SOLVING_ERROR: {
            printf("Caught unexpected error while solving %lgx^2 + %lgx + %lg\n", expected->a, expected->b, expected->c);
            break ;
        }
        case NOT_SAME_ROOTS_NUMBER: {
            char string_number_expected[32] = {};
            char string_number_actual[32] = {};
            roots_number_to_string(string_number_expected, expected->roots_number);
            roots_number_to_string(string_number_actual, actual->roots_number);

            printf("Caught error while solving %lgx^2 + %lgx + %lg\n"
                   "Different roots number:\n"
                   "Expected: %s, actual: %s\n",
                   expected->a, expected->b, expected->c,
                   string_number_expected, string_number_actual);
            break;
        }
        case DIFFERENT_ROOTS: {
            print_different_roots_error(expected, actual);
            break;
        }
        default: {
            printf("Unexpected return from test function\n");
        }
    }

    printf("-----------------------------------\n");
}




void print_different_roots_error(const quadratic_equation_t *expected, const quadratic_equation_t *actual) {
    printf("Caught error while solving %lgx^2 + %lgx + %lg\n", expected->a, expected->b, expected->c);
    switch(expected->roots_number){
        case ONE_ROOT: {
            printf("Expected: x = %lg, actual: x = %lg\n", expected->x1, actual->x1);
            return ;
        }
        case TWO_ROOTS: {
            printf("Expected: x1 = %lg, x2 = %lg\n"
                   "Actual: x1 = %lg, x2 = %lg\n",
                   expected->x1, expected->x2, actual->x1, actual->x2);
            return ;
        }
        case NOT_SOLVED: {
            printf("There are no roots, equation is not solved\n");
            return ;
        }
        case NO_ROOTS: {
            printf("There are 0 roots\n");
            return ;
        }
        case INFINITELY_MANY_ROOTS: {
            printf("Error, every x is root of equation\n");
            return ;
        }
        default: {
            printf("Errorneus result of test function\n");
            return ;
        }
    }
}



void roots_number_to_string(char *output, roots_number_t number) {
    switch(number) {
        case NOT_SOLVED: {
            sprintf(output, "NOT_SOLVED");
            return ;
        }

        case NO_ROOTS: {
            sprintf(output, "0");
            return ;
        }

        case ONE_ROOT: {
            sprintf(output, "1");
            return ;
        }

        case TWO_ROOTS: {
            sprintf(output, "2");
            return ;
        }

        case INFINITELY_MANY_ROOTS: {
            sprintf(output, "INF");
            return ;
        }

        default: {
            sprintf(output, "ERROR");
        }
    }
}



bool is_zero(double num) {
    if(fabs(num) < EPSILON) return true;
    return false;
}
