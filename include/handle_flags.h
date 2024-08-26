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
exit_code_t parse_flags(int argc, const char *argv[]);

exit_code_t modes_init(unsigned int number);

exit_code_t register_mode(const char *long_name, const char *short_name, exit_code_t (*handler)(void));

exit_code_t choose_default_mode(const char *short_name);

exit_code_t free_modes(void);
#endif
