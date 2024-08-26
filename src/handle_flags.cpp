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
#include "stack.h"
#include "quadratic_tests.h"

/**
===============================================================================================================================
    @brief Structure to store names and handle functions of supported modes.

===============================================================================================================================
*/

struct solving_mode_t {
    const char *long_name;
    const char *short_name;
    exit_code_t (*handle_function)(const int argc, const char *argv[]);
};

struct program_modes_t {
    stack_elem_t *modes_list;
    exit_code_t (*default_handler)(const int argc, const char *argv[]);
};

static exit_code_t handle_unknown_flag(const char *flag);

/**
===============================================================================================================================
    @brief   - List of modes supported by program.
    @details - Contains short name and long name(for example '-h' and '--help').\n
             - Contains function to handle user.

===============================================================================================================================
*/
exit_code_t parse_flags(const int argc, const char *argv[], program_modes_t *modes_stack){
    C_ASSERT(modes_stack                  != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(argv                         != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(modes_stack->default_handler != NULL, EXIT_CODE_FAILURE);
    if(argc == 1)
        return modes_stack->default_handler(argc, argv);

    for(solving_mode_t *current = (solving_mode_t *)stack_pop(&(modes_stack->modes_list));
        current != NULL;
        free(current),  current = (solving_mode_t *)stack_pop(&(modes_stack->modes_list))) {
        if(strcmp(current->long_name,  argv[1]) == 0 ||
           strcmp(current->short_name, argv[1]) == 0) {
            return current->handle_function(argc, argv);
        }
    }

    return handle_unknown_flag(argv[1]);
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

exit_code_t register_mode(program_modes_t **modes_stack, const char *short_name, const char *long_name, exit_code_t (*handler)(const int argc, const char *argv[])) {
    C_ASSERT(short_name != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(long_name  != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(handler    != NULL, EXIT_CODE_FAILURE);

    if(*modes_stack == NULL) {
        *modes_stack = (program_modes_t *)calloc(1, sizeof(program_modes_t));
        if(*modes_stack == NULL)
            return EXIT_CODE_FAILURE;
    }

    solving_mode_t *mode = (solving_mode_t *)calloc(1, sizeof(solving_mode_t));
    if(mode == NULL)
        return EXIT_CODE_FAILURE;

    mode->handle_function = handler   ;
    mode->long_name       = long_name ;
    mode->short_name      = short_name;

    if(stack_push(&((*modes_stack)->modes_list), (void *)mode) != STACK_SUCCESS) {
        free(mode);
        return EXIT_CODE_FAILURE;
    }

    return EXIT_CODE_SUCCESS;
}

exit_code_t choose_default_mode(program_modes_t **modes_stack, exit_code_t (*handler)(const int argc, const char *argv[])) {
    C_ASSERT(handler != NULL, EXIT_CODE_FAILURE);

    if(*modes_stack == NULL) {
        *modes_stack = (program_modes_t *)calloc(1, sizeof(program_modes_t));
        if(*modes_stack == NULL)
            return EXIT_CODE_FAILURE;
    }

    (*modes_stack)->default_handler = handler;
    return EXIT_CODE_SUCCESS;
}

void free_modes(program_modes_t *modes_stack) {
    if(modes_stack == NULL)
        return ;

    free_stack(modes_stack->modes_list);
}
