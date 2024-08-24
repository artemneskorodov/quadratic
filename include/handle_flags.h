/**
===============================================================================================================================
    @file    handle_flags.h
    @brief   Header of library, allowing to handle flags typed in by user in console
    @date    23.08.2024
    @author  Artem Neskorodov
    @link    https://vk.com/neskorodovartem

===============================================================================================================================
*/

#ifndef HANDLE_FLAGS_H
#define HANDLE_FLAGS_H

/**
================================================================================================================================
    @brief   - Defines the mode in which program will run

    @details - Allows to use --help, --solve and --test flags\n
             - Starting program without a flag is considered as solving mode

    @param   [in]  argc               Number of strings in argv
    @param   [in]  argv               Array of strings with flags

    @return  0 in case of success

===============================================================================================================================
*/
int handle_user(int argc, const char *argv[]);

#endif
