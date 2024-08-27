/**
===============================================================================================================================
    @file    handle_flags.cpp
    @brief   Handling flags pushed from console.
    @date    23.08.2024
    @author  Artem Neskorodov
    @link    https://vk.com/neskorodovartem

===============================================================================================================================
*/
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "colors.h"
#include "custom_assert.h"
#include "handle_flags.h"
#include "quadratic.h"
#include "quadratic_tests.h"
#include "handlers.h"

/**
===============================================================================================================================
    @brief Structure to store names and handle functions of supported modes.

===============================================================================================================================
*/
struct solving_mode_t {
    const char *long_name;
    const char *short_name;
    exit_code_t (*handle_function)(const int, const char **);
};

const solving_mode_t modes[] =
    {{"--test" , "-t", handle_test },
     {"--solve", "-s", handle_solve},
     {"--help" , "-h", handle_help }};
exit_code_t (* const default_handler)(const int, const char **) = handle_solve;

/**
===============================================================================================================================
    @brief   - List of modes supported by program.
    @details - Contains short name and long name(for example '-h' and '--help').\n
             - Contains function to handle user.

===============================================================================================================================
*/
exit_code_t parse_flags(const int argc, const char *argv[]){
    C_ASSERT(argv != NULL, EXIT_CODE_FAILURE);

    if(argc == 1) return default_handler(argc, argv);

    for(size_t mode = 0; mode < sizeof(modes) / sizeof(solving_mode_t); mode++)
        if(strcmp(modes[mode].long_name,  argv[1]) == 0 ||
           strcmp(modes[mode].short_name, argv[1]) == 0)
            return modes[mode].handle_function(argc, argv);

    return handle_unknown_flag(argv[1]);
}

exit_code_t handle_unknown_flag(const char *flag){
    C_ASSERT(flag != NULL, EXIT_CODE_FAILURE);
    color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "Unknown flag '%s'\n", flag);
    color_printf(DEFAULT_TEXT, false, DEFAULT_BACKGROUND, "Use flag ");
    color_printf(PURPLE_TEXT, false, DEFAULT_BACKGROUND, "'--help'\n");
    return EXIT_CODE_SUCCESS;
}
