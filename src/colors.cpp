#include "colors.h"
#include <stdarg.h>
#include <stdio.h>

static const char *red_code = "\033[1;31m";
static const char *green_code = "\033[1;32m";
static const char *yellow_code = "\033[1;33m";
static const char *blue_code = "\033[1;34m";
static const char *purple_code = "\033[1;35m";
static const char *cyan_code = "\033[1;36m";
static const char *white_code = "\033[1;37m";

static void start_color(color_t color);
static void reset_color(void);

int color_printf(color_t color, const char *string, ...) {
    va_list args;
    va_start(args, string);
    start_color(color);
    vprintf(string, args);
    reset_color();
    va_end(args);
    return 0;
}

void start_color(color_t color) {
    switch(color){
        case RED: {
            printf("%s", red_code);
            return ;
        }
        case GREEN: {
            printf("%s", green_code);
            return ;
        }
        case YELLOW: {
            printf("%s", yellow_code);
            return ;
        }
        case BLUE: {
            printf("%s", blue_code);
            return ;
        }
        case PURPLE: {
            printf("%s", purple_code);
            return ;
        }
        case CYAN: {
            printf("%s", cyan_code);
            return ;
        }
        case WHITE: {
            printf("%s", white_code);
            return ;
        }
        case DEFAULT: {
            return ;
        }
        default: {
            return ;
        }
    }
}

void reset_color(void){
    printf("\033[0m");
}
