#include "./include/quadratic.h"
#include "./include/quadratic_tests.h"
#include <stdio.h>





int main(void) {

    #ifdef TEST
    printf("Total errors: %d", test_solving_quadratic());

    #else
    quadratic_equation_t equation = {.number = NOT_SOLVED};

    //Getting coefficients from user
    if(get_coefficients(&equation) == GETTING_EXIT){
        printf("Stop using Vietta\n");
        return 0;
    }

    //Solving quadratic
    switch(solve_quadratic(&equation)) {
        case INVALID_COEFFICIENTS: {
            printf("Your input was invalid, unable to solve equation :(\n");
            return -1;
        }
        case SOLVING_ERROR: {
            printf("Caught unexpected error while solving\n");
            return -2;
        }
        case SOLVING_SUCCESS: {
            print_quadratic_result(&equation);
            return 0;
        }
        default: {
            printf("solve_quadratic() returned unexpected result\n");
            return -3;
        }
    }
    #endif
}
