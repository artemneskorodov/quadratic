#include <stdlib.h>
#include <stdio.h>
#include "handle_flags.h"
#include "custom_assert.h"
#include "handlers.h"
#include "colors.h"

/**
===============================================================================================================================
    @mainpage Quadratic.exe
    @brief    Allowes to solve quadratic equations.
    @details  Has different modes:\n
                + Solving (--solve)\n
                + Testing (--test)\n
              --help to get help
    @author   Artem Neskorodov
    @link     https://vk.com/neskorodovartem
    @date     24.08.2024

===============================================================================================================================
*/

int main(const int argc, const char *argv[]) {
    return (int)parse_flags(argc, argv);
}

