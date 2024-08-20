#include <stdio.h>
#include <math.h>
#include <cassert>
#include <string.h>

typedef enum getting_coefficients_state_t{
    SUCCESS,
    EXIT
}getting_coefficients_state_t;


typedef struct quadratic_coefficients_t{
    double a, b, c;
    getting_coefficients_state_t state;
}quadratic_coefficients_t;


typedef enum roots_nuber_t{
    ERROR_WITH_SOLVING_QUADRATIC,
    INFINITELY_MANY_ROOTS,
    NO_ROOTS,
    ONE_ROOT,
    TWO_ROOTS
}roots_number_t;


typedef struct quadratic_roots_t{
    roots_number_t number;
    double x1, x2;
}quadratic_roots_t;

typedef enum compare_result_t{
    EQUALS,
    BIGGER,
    LESS
}compare_result_t;



//Function prototypes
quadratic_coefficients_t get_coefficients(void);
quadratic_roots_t solve_quadratic(quadratic_coefficients_t equation);
compare_result_t compare_doubles(double a, double b);
void print_quadratic_roots(quadratic_roots_t roots);
getting_coefficients_state_t get_number(char symbol, double *out);



int main(void){
    //Getting coefficients from user
    quadratic_coefficients_t equation = get_coefficients();
    if(equation.state == EXIT)
        return 0;

    //Solving equation
    quadratic_roots_t roots = solve_quadratic(equation);

    //Printing answers in console
    print_quadratic_roots(roots);

    return 0;
}



getting_coefficients_state_t get_number(char symbol, double *out){
    printf("%c = ", symbol);
    if(scanf("%lf", out) == 1)
        return SUCCESS;
    char string[128] = {0, };
    scanf("%s", string);
    if(strcmp(string, "exit") == 0) return EXIT;
    printf("Invalid input\n");
    return get_number(symbol, out);
}



quadratic_coefficients_t get_coefficients(void){
    quadratic_coefficients_t equation;
    equation.state = SUCCESS;
    printf("Enter coefficients for ax^2 + bx + c == 0: \n");
    if(get_number('a', &equation.a) == EXIT){
        equation.state = EXIT;
        return equation;
    }
    if(get_number('b', &equation.b) == EXIT){
        equation.state = EXIT;
        return equation;
    }
    if(get_number('c', &equation.c) == EXIT){
        equation.state = EXIT;
        return equation;
    }

    return equation;
}




//solves quadratic equation in form ax^2 + bx + c
//writes roots in x1 and x2
//returns number of roots
quadratic_roots_t solve_quadratic(quadratic_coefficients_t equation){
    quadratic_roots_t roots;


    //checkig for invalid inputs
    assert(isfinite(equation.a));
    assert(isfinite(equation.b));
    assert(isfinite(equation.c));

    //comparing coefficients with 0.0
    int compare_with_zero_a = compare_doubles(equation.a, 0.0);
    int compare_with_zero_b = compare_doubles(equation.b, 0.0);
    int compare_with_zero_c = compare_doubles(equation.c, 0.0);

    //in case a == 0, b == 0 and c == 0 equation has infinitely many roots
    if(compare_with_zero_a == EQUALS && compare_with_zero_b == EQUALS && compare_with_zero_c == EQUALS){
        roots.number = INFINITELY_MANY_ROOTS;
        return roots;
    }

    //if a == 0 equation is linear
    else if(compare_with_zero_a == EQUALS){
        if(compare_with_zero_b == 0 && compare_with_zero_c != 0){
            //c == 0(where c != 0)
            roots.number = NO_ROOTS;
        }
        else{
            //cx + k = 0 (c != 0) => x = -k / c
            roots.number = ONE_ROOT;
            roots.x1 = (-equation.c) / equation.b;
        }
        return roots;
    }


    //discriminant of quadratic equation
    double discriminant = equation.b * equation.b - 4.0 * equation.a * equation.c;


    //comparison of discriminant and 0.0
    compare_result_t compare_state = compare_doubles(discriminant, 0.0);


    switch(compare_state){
        case EQUALS: {
            //1 real root
            roots.number = ONE_ROOT;
            roots.x1 = (-equation.b) / (2.0 * equation.a);
            break;
        }
        case BIGGER: {
            //2 real roots
            roots.number = TWO_ROOTS;
            double discriminant_square = sqrt(discriminant);
            roots.x1 = (-equation.b - discriminant_square) / (2 * equation.a);
            roots.x2 = (-equation.b + discriminant_square) / (2 * equation.a);
            break;
        }
        case LESS: {
            roots.number = NO_ROOTS;
            break;
        }
        default: {
            roots.number = ERROR_WITH_SOLVING_QUADRATIC;
        }
    }
    return roots;
}



//compares doubles
//if difference between a and b is less then epsilon(==1e-6) then they are equal, returns 0
//if a < b returns -1
//and if a > b returns 1
compare_result_t compare_doubles(double a, double b){
    const double EPSILON = 1e-6;
    if(fabs(a - b) < EPSILON) return EQUALS;
    if(a < b) return LESS;
    return BIGGER;
}



//prints roots of quadratic equation in console
void print_quadratic_roots(quadratic_roots_t roots){
    switch(roots.number){
        case TWO_ROOTS: {
            printf("x1 = %g, x2 = %g\n", roots.x1, roots.x2);
            return ;
        }

        case ONE_ROOT: {
            printf("x = %g\n", roots.x1);
            return ;
        }

        case NO_ROOTS: {
            printf("Equation has no real roots\n");
            return ;
        }

        case INFINITELY_MANY_ROOTS: {
            printf("Equation has infinitely many roots\n");
            return ;
        }

        case ERROR_WITH_SOLVING_QUADRATIC: {
            printf("Error with solving quadratioc equation\n");
            return ;
        }
        default: {
            printf("unexpected error\n");
            return ;
        }
    }
}
