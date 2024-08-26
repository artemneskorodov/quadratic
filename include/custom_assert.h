/**
===============================================================================================================================
    @file    custom_assert.h
    @brief   header of custom assert library.
    @date    23.08.2024
    @author  Artem Neskorodov
    @link    https://vk.com/neskorodovartem

===============================================================================================================================
*/

#ifndef CUSTOM_ASSERT_H
#define CUSTOM_ASSERT_H

/**
===============================================================================================================================
    @brief   - If NDEBUG is defined, asserts will be ommited.

    @details - C_ASSERT in case of error will print out file name,
            line number and wrond expression and then termenate program.

===============================================================================================================================
*/
#ifndef NDEBUG
// TODO be able to handle return value
#define C_ASSERT(expression, return_value) if(expression) {(void)0;} else {print_assert_error(#expression, __LINE__, __FILE__);return return_value;}
#else
#define C_ASSERT(expression, return_value) ((void)0);
#endif

/**
===============================================================================================================================
    @brief   - Prints line number, file name and expression on which C_ASSERT macro gained error.

    @details - Terminates program after use.

    @param   [in]  string             Expression that is FALSE.
    @param   [in]  line_number        Number of line in source code on which C_ASSERT gained error.
    @param   [in]  filename           String with name of file in which C_ASSERT gained error.

===============================================================================================================================
*/
void print_assert_error(const char *string, int line_number, const char *filename);

#endif
