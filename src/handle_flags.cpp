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
    solving_mode_t *next;
};

struct program_modes_t {
    solving_mode_t *list_head;
    exit_code_t (*default_handler)(void);
};

static exit_code_t handle_unsupported_amount_of_flags(void);
static exit_code_t handle_unknown_flag(const char *flag);

/**
===============================================================================================================================
    @brief   - List of modes supported by program.
    @details - Contains short name and long name(for example '-h' and '--help').\n
             - Contains function to handle user.

===============================================================================================================================
*/
exit_code_t parse_flags(int argc, const char *argv[], program_modes_t *modes_list){
    C_ASSERT(modes_list != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(argv != NULL, EXIT_CODE_FAILURE);

    if(argc != 1 && argc != 2)
        return handle_unsupported_amount_of_flags();
    if(argc == 1)
        return modes_list->default_handler();

    solving_mode_t *current_mode = modes_list->list_head;
    while(modes_list != NULL) {
        if(strcmp(current_mode->long_name,  argv[argc - 1]) == 0 ||
           strcmp(current_mode->short_name, argv[argc - 1]) == 0)
            return current_mode->handle_function();
        current_mode = current_mode->next;
    }

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

exit_code_t register_mode(program_modes_t *list, const char *short_name, const char *long_name, exit_code_t (*handler)(void)) {
    C_ASSERT(list != NULL, EXIT_CODE_FAILURE);
    if(list->list_head == NULL) {
        list->list_head = (solving_mode_t *)calloc(1, sizeof(solving_mode_t));
        list->list_head->handle_function = handler;
        list->list_head->long_name = long_name;
        list->list_head->short_name = short_name;
        list->list_head->next = NULL;
        return EXIT_CODE_SUCCESS;
    }

    solving_mode_t *current_elem = list->list_head;
    while(current_elem->next != NULL)
        current_elem = current_elem->next;

    current_elem->next = (solving_mode_t *)calloc(1, sizeof(solving_mode_t));
    current_elem->next->next = NULL;
    current_elem->next->long_name = long_name;
    current_elem->next->short_name = short_name;
    current_elem->next->handle_function = handler;
    return EXIT_CODE_SUCCESS;
}

exit_code_t choose_default_mode(program_modes_t *modes, exit_code_t (*handler)(void)) {
    C_ASSERT(handler != NULL, EXIT_CODE_FAILURE);
    C_ASSERT(modes   != NULL, EXIT_CODE_FAILURE);

    modes->default_handler = handler;
    return EXIT_CODE_SUCCESS;
}

exit_code_t free_modes(program_modes_t *modes) {
    solving_mode_t *current = modes->list_head;
    while(current != NULL) {
        solving_mode_t *next = current->next;
        free(current);
        current = next;
    }
    free(modes);
    return EXIT_CODE_SUCCESS;
}

program_modes_t *modes_list_init(void) {
    program_modes_t *list = (program_modes_t *)calloc(1, sizeof(program_modes_t));
    return list;
}
