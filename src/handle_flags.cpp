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

/**
===============================================================================================================================
    @brief Structure to store names and handle functions of supported modes.

===============================================================================================================================
*/
struct solving_mode_t {
    const char *long_name;
    const char *short_name;
    exit_code_t (*handle_function)(void);
};

static unsigned int modes_number = 0;
static exit_code_t (*default_handler)(void) = NULL;
static solving_mode_t *modes = NULL;

static exit_code_t handle_unsupported_amount_of_flags(void);
static exit_code_t handle_unknown_flag(const char *flag);

/**
===============================================================================================================================
    @brief   - List of modes supported by program.
    @details - Contains short name and long name(for example '-h' and '--help').\n
             - Contains function to handle user.

===============================================================================================================================
*/
exit_code_t parse_flags(int argc, const char *argv[]){
    C_ASSERT(argv != NULL, EXIT_CODE_FAILURE);
    if(modes == NULL || modes_number == 0)
        return EXIT_CODE_FAILURE;

    if(argc != 1 && argc != 2) return handle_unsupported_amount_of_flags();
    if(argc == 1) return default_handler();

    for(size_t mode = 0; mode < modes_number; mode++)
        if(strcmp(modes[mode].long_name,  argv[argc - 1]) == 0 ||
           strcmp(modes[mode].short_name, argv[argc - 1]) == 0)
            return modes[mode].handle_function();

    return handle_unknown_flag(argv[argc - 1]);
}

/**
===============================================================================================================================
    @brief   - Handles the case when user typed in more then one flag.

===============================================================================================================================
*/
exit_code_t handle_unsupported_amount_of_flags(void){
    color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "Unsupported amount of flags, you can use only 0 or 1\n");
    color_printf(DEFAULT_TEXT, false, DEFAULT_BACKGROUND, "Use flag ");
    color_printf(PURPLE_TEXT, false, DEFAULT_BACKGROUND, "'--help'\n");
    return EXIT_CODE_SUCCESS;
}

/**
===============================================================================================================================
    @brief   - Handles the case when user typed in unknown flag.

    @param   [in]  arg                String with flag, typed in by user.

===============================================================================================================================
*/
exit_code_t handle_unknown_flag(const char *flag){
    C_ASSERT(flag != NULL, EXIT_CODE_FAILURE);
    color_printf(RED_TEXT, false, DEFAULT_BACKGROUND, "Unknown flag '%s'\n", flag);
    color_printf(DEFAULT_TEXT, false, DEFAULT_BACKGROUND, "Use flag ");
    color_printf(PURPLE_TEXT, false, DEFAULT_BACKGROUND, "'--help'\n");
    return EXIT_CODE_SUCCESS;
}

exit_code_t modes_init(unsigned int number) {
    if(modes_number != 0)
        return EXIT_CODE_FAILURE;
    C_ASSERT(modes == NULL, EXIT_CODE_FAILURE);
    C_ASSERT(number != 0, EXIT_CODE_FAILURE);
    modes_number = number;
    modes = (solving_mode_t *)calloc(number, sizeof(solving_mode_t));
    return EXIT_CODE_SUCCESS;
}

exit_code_t register_mode(const char *long_name, const char *short_name, exit_code_t (*handler)(void)) {
    static size_t counter = 0;
    modes[counter].long_name = long_name;
    modes[counter].short_name = short_name;
    modes[counter].handle_function = handler;
    counter++;
    if(counter > modes_number)
        return EXIT_CODE_FAILURE;
    return EXIT_CODE_SUCCESS;
}

exit_code_t choose_default_mode(const char *short_name) {
    for(size_t i = 0; i < modes_number; i++) {
        if(strcmp(modes[i].short_name, short_name) == 0) {
            default_handler = modes[i].handle_function;
            return EXIT_CODE_SUCCESS;
        }
    }
    return EXIT_CODE_FAILURE;
}

exit_code_t free_modes(void) {
    free(modes);
    return EXIT_CODE_SUCCESS;
}
