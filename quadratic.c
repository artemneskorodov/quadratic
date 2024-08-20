#include <stdio.h>
#include <math.h>



const int INVALID_COEFFITIENTS_FOR_QUADRATIC = -1;
const int INFINITLY_MANY_ROOTS = -2;
const int UNEXPECTED_ERROR_WHILE_SOLVING_QUADRATIC = -3;


int solve_quadratic(double a, double b, double c, double *x1, double *x2);
int compare_doubles(double a, double b);
void print_quadratic_roots(int number, double x1, double x2);



int main(void){
    //Asking user to enter values
    double a = 0, b = 0, c = 0;
    printf("Enter coeffitients of equation in form \"a b c\" (for ax^2 + bx + c == 0): ");
    if(scanf("%lf %lf %lf", &a, &b, &c) != 3){
        printf("Error in values\n");
        exit(-1);
    }

    //Solving equation
    double x1 = 0, x2 = 0;
    int number = solve_quadratic(a, b, c, &x1, &x2);

    //Printing answers in console
    print_quadratic_roots(number, x1, x2);

    return 0;
}




//solves quadratic equation in form ax^2 + bx + c
//writes roots in x1 and x2
//returns number of roots
int solve_quadratic(double a, double b, double c, double *x1, double *x2){
    int compare_with_zero_a = compare_doubles(a, 0.0);
    int compare_with_zero_b = compare_doubles(b, 0.0);
    int compare_with_zero_c = compare_doubles(c, 0.0);
    //in case a == 0, b == 0 and c == 0 equation has infinitely many roots
    if(compare_with_zero_a == 0 && compare_with_zero_b == 0 && compare_with_zero_c == 0) return INFINITLY_MANY_ROOTS;
    //if a == 0 at least one of b and c is nonzero, it is not quadratic equation
    if(compare_with_zero_a == 0) return INVALID_COEFFITIENTS_FOR_QUADRATIC;

    //discriminant of quadratic equation
    double discriminant = b * b - 4.0 * a * c;

    //comparison of discriminant and 0.0
    //1 -> discriminant > 0, -1 -> discriminant < 0, 0 -> discriminant == 0
    int compare_state = compare_doubles(discriminant, 0.0);

    switch(compare_state){
        case 0: {
            //1 real root
            *x1 = (-b) / (2 * a);
            return 1;
        }
        case 1: {
            //2 real roots
            double discriminant_square = sqrt(discriminant);
            *x1 = (-b - discriminant_square) / (2 * a);
            *x2 = (-b + discriminant_square) / (2 * a);
            return 2;
        }
        case -1: {
            //no real roots
            return 0;
        }
        default: {
            return UNEXPECTED_ERROR_WHILE_SOLVING_QUADRATIC;
        }
    }
}



//compares doubles
//if difference between a and b is less then epsilon(==1e-6) then they are equal, returns 0
//if a < b returns -1
//and if a > b returns 1
int compare_doubles(double a, double b){
    const double EPSILON = 1e-6;
    if(fabs(a - b) < EPSILON) return 0;
    if(a < b) return -1;
    return 1;
}



//prints roots of quadratic equation in console
void print_quadratic_roots(int number, double x1, double x2){
    switch(number){
        //2 roots
        case 2: {
            printf("x1 = %f, x2 = %f\n", x1, x2);
            break;
        }
        //1 root
        case 1: {
            printf("x = %f", x1);
            break;
        }
        //no roots
        case 0: {
            printf("No real roots\n");
            break;
        }
        //invalid coeffitients(a == 0)
        case INVALID_COEFFITIENTS_FOR_QUADRATIC: {
            printf("Invalid coeffitients\n");
            break;
        }
        //infinitly many roots(a == b == c == 0)
        case INFINITLY_MANY_ROOTS: {
            printf("Every real number\n");
            break;
        }
        default: {
            printf("Error\n");
        }
    }
}
