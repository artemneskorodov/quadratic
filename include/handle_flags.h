/**
===============================================================================================================================
    @file    handle_flags.h
    @brief   Header of library, allowing to handle flags typed in by user in console.
    @date    23.08.2024
    @author  Artem Neskorodov
    @link    https://vk.com/neskorodovartem

===============================================================================================================================
*/

#ifndef HANDLE_FLAGS_H
#define HANDLE_FLAGS_H

enum exit_code_t {
    EXIT_CODE_SUCCESS = 0,
    EXIT_CODE_FAILURE = 1
};

struct program_modes_t;

/**
================================================================================================================================
    @brief   - Defines the mode in which program will run.

    @details - Allows to use --help, --solve and --test flags.\n
             - Starting program without a flag is considered as solving mode.

    @param   [in]  argc               Number of strings in argv.
    @param   [in]  argv               Array of strings with flags.

    @return  Exit code

===============================================================================================================================
*/
exit_code_t parse_flags(int argc, const char *argv[], program_modes_t *list);

/**
===============================================================================================================================
    @brief   - Adds mode to modes list.

    @param   [in]  long_name          Long flag for mode.
    @param   [in]  short_name         Short flag for mode.
    @param   [in]  handler            Function, that handles user for particullar mode.
    @param   [in]  list               Pointer to list with modes

    @return  Error (or success) code.
===============================================================================================================================
*/
exit_code_t register_mode(program_modes_t *list, const char *short_name, const char *long_name, exit_code_t (*handler)(void));

/**
===============================================================================================================================
    @brief   - Choose the default run mode.

    @details - This function will run if user does not type in any flags.

    @param   [in]  short_name         Short name of particullar mode (registered with register_mode(...) before).

    @return  Error (or success) code.

===============================================================================================================================
*/
exit_code_t choose_default_mode(program_modes_t *modes, exit_code_t (*handler)(void));

/**
===============================================================================================================================
    @brief   - Frees modes array.

    @return  Error (or success) code.

===============================================================================================================================
*/
exit_code_t free_modes(program_modes_t *modes);

program_modes_t *modes_list_init(void);
#endif
