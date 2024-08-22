#include <handle_flags.h>
#include <stdbool.h>
#include <quadratic.h>
#include <quadratic_tests.h>
#include <stdio.h>
#include <string.h>
#include <colors.h>



enum flag_t {
    PROHIBITED_AMOUNT_FLAGS,
    HELP_FLAG,
    TEST_FLAG,
    SOLVE_FLAG,
    UNKNOWN_FLAG
};



static flag_t get_flag(int argc, char *argv[]);
static int handle_amount_flag(void);
static int handle_unknown_flag(char *flag);
static int handle_help_flag(void);
static int handle_solve_flag(void);
static int handle_test_flag(void);
static int handle_error(void);



flag_t get_flag(int argc, char *argv[]) {
    if(argc < 1) return PROHIBITED_AMOUNT_FLAGS;
    if(argc == 1)
        return SOLVE_FLAG;

    if(argc > 2)
        return PROHIBITED_AMOUNT_FLAGS;

    if(!strcmp("--help", argv[argc - 1])) return HELP_FLAG;
    if(!strcmp("--solve", argv[argc - 1])) return SOLVE_FLAG;
    if(!strcmp("--test", argv[argc - 1])) return TEST_FLAG;

    return UNKNOWN_FLAG;
}



int handle_user(int argc, char *argv[]){
    switch(get_flag(argc, argv)) {
        case PROHIBITED_AMOUNT_FLAGS: return handle_amount_flag();
        case UNKNOWN_FLAG: return handle_unknown_flag(argv[argc - 1]);
        case HELP_FLAG: return handle_help_flag();
        case TEST_FLAG: return handle_test_flag();
        case SOLVE_FLAG: return handle_solve_flag();
        default: return handle_error();
    }
}



int handle_amount_flag(void){
    printf(RED("Unexpected amount of flags, you can use only 0 or 1\n")
           "Use flag " PURPLE("'--help'\n"));
    return 0;
}



int handle_unknown_flag(char *arg){
    printf(RED("Unknown flag '%s'\n")
           "Use flag " PURPLE("'--help'\n"), arg);
    return 0;
}



int handle_help_flag(void){
    printf(PURPLE("\t'--help'") " for help\n"
           PURPLE("\t'--solve'") " to type in and solve equation\n"
           PURPLE("\t'--test'") " to run tests\n"
           PURPLE("\t''") " is considered as " PURPLE("'--solve'\n"));
    return 0;
}



int handle_solve_flag(void){
    quadratic_equation_t equation = {.number = NOT_SOLVED};

    //Getting coefficients from user
    if(get_coefficients(&equation) == GETTING_EXIT){
        printf(CYAN("Stop using Vietta\n"));
        return 0;
    }

    //Solving quadratic
    switch(solve_quadratic(&equation)) {
        case INVALID_COEFFICIENTS: {
            printf(RED("Your input was invalid, unable to solve equation :(\n"));
            return -1;
        }
        case SOLVING_ERROR: {
            printf(RED("Caught unexpected error while solving\n"));
            return -2;
        }
        case SOLVING_SUCCESS: {
            print_quadratic_result(&equation);
            return 0;
        }
        default: {
            printf(RED("solve_quadratic() returned unexpected result\n"));
            return -3;
        }
    }
}



int handle_test_flag(void) {
    printf(YELLOW("Total errors: %d"), test_solving_quadratic());
    return 0;
}



int handle_error(void) {
    printf(RED("Something went wrong\n"));
    return -1;
}
