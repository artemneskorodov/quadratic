#include "handle_flags.h"
#include "custom_assert.h"
#include <stdio.h>

/**
===============================================================================================================================
    @mainpage Quadratic.exe
    @brief    Allowes to solve quadratic equations
    @details  Has different modes:\n
                + Solving (--solve)\n
                + Testing (--test)\n
              --help to get help
    @author   Artem Neskorodov
    @link     https://vk.com/neskorodovartem
    @date     24.08.2024

===============================================================================================================================
*/

int main(int argc, const char *argv[]) {
    C_ASSERT(argv != NULL);
    return (int)handle_user(argc, argv);
}
