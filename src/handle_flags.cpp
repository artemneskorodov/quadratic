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
    exit_code_t (*handle_function)(const int argc, const char *argv[]);
    solving_mode_t *next;
};

struct program_modes_t {
    solving_mode_t *modes_list;
    exit_code_t (*default_handler)(const int argc, const char *argv[]);
};

enum push_state_t {
    PUSHED_SUCCESSFULLY,
    PUSHING_ERROR
};

static exit_code_t handle_unknown_flag(const char *flag);
static solving_mode_t *mode_pop(solving_mode_t **head);
static push_state_t mode_push(solving_mode_t **head, solving_mode_t *mode);

exit_code_t parse_flags(const int argc, const char *argv[], program_modes_t *modes_stack){
    C_ASSERT(modes_stack                  != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(argv                         != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(modes_stack->default_handler != NULL, EXIT_CODE_FAILURE);

    if(argc == 1)
        return modes_stack->default_handler(argc, argv);

    for(solving_mode_t *current = mode_pop(&(modes_stack->modes_list));
        current != NULL;
        free(current),  current = mode_pop(&(modes_stack->modes_list))) {
        if(strcmp(current->long_name,  argv[1]) == 0 ||
           strcmp(current->short_name, argv[1]) == 0) {
            exit_code_t (*function)(const int, const char *[]) = current->handle_function;
            free(current);
            return function(argc, argv);
        }
    }

    return handle_unknown_flag(argv[1]);
}

/**
===============================================================================================================================
    @brief   - Handles the case when user typed in unknown flag.

    @param   [in]  flag               String with flag, typed in by user.

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
    C_ASSERT(short_name  != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(long_name   != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(handler     != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(modes_stack != NULL, EXIT_CODE_FAILURE);

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

    if(mode_push(&((*modes_stack)->modes_list), mode) != PUSHED_SUCCESSFULLY) {
        free(mode);
        return EXIT_CODE_FAILURE;
    }

    return EXIT_CODE_SUCCESS;
}

exit_code_t choose_default_mode(program_modes_t **modes_stack, exit_code_t (*handler)(const int argc, const char *argv[])) {
    C_ASSERT(handler     != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(modes_stack != NULL, EXIT_CODE_FAILURE);

    if(*modes_stack == NULL) {
        *modes_stack = (program_modes_t *)calloc(1, sizeof(program_modes_t));
        if(*modes_stack == NULL)
            return EXIT_CODE_FAILURE;
    }

    (*modes_stack)->default_handler = handler;
    return EXIT_CODE_SUCCESS;
}

void free_modes(program_modes_t *modes_stack) {
    C_ASSERT(modes_stack != NULL, );
    solving_mode_t *current = modes_stack->modes_list;

    while(current != NULL) {
        solving_mode_t *next = current->next;
        free(current);
        current = next;
    }

    free(modes_stack);
}

/**
================================================================================================================================
    @brief   - Gets one element from stack.

    @details - Returns NULL, when there is no other elements in stack.\n
             - Do not forget to free memory, allocated to mode.

    @param   [in]  head               Pointer to pointer to head in program_modes_structure.

    @return  Pointer to solving mode structure.

===============================================================================================================================
*/
solving_mode_t *mode_pop(solving_mode_t **head) {
    C_ASSERT(head != NULL, NULL);

    if(*head == NULL)
        return NULL;

    solving_mode_t *result_mode = *head;
    *head = (*head)->next;
    return result_mode;
}

/**
================================================================================================================================
    @brief   - Pushes element in stack.

    @details - Allows to use --help, --solve and --test flags.\n
             - Starting program without a flag is considered as solving mode.

    @param   [in]  head               Pointer to pointer to head in program_modes_structure.
    @param   [in]  mode               Pointer to structure, containing solving mode.

    @return  Error (or success) code.

===============================================================================================================================
*/
push_state_t mode_push(solving_mode_t **head, solving_mode_t *mode) {
    C_ASSERT(head != NULL, PUSHING_ERROR);

    mode->next = *head;
    *head = mode;
    return PUSHED_SUCCESSFULLY;
}
