/**
===============================================================================================================================
    @file    colors.h
    @brief   Header of library, allowing to print colored text in console.
    @date    23.08.2024
    @author  Artem Neskorodov
    @link    https://vk.com/neskorodovartem

===============================================================================================================================
*/
#ifndef COLORS_H
#define COLORS_H

enum color_t {
    RED_TEXT,
    GREEN_TEXT,
    YELLOW_TEXT,
    BLUE_TEXT,
    PURPLE_TEXT,
    CYAN_TEXT,
    WHITE_TEXT,
    DEFAULT_TEXT
};

enum background_t {
    BLACK_BACKGROUND,
    RED_BACKGROUND,
    GREEN_BACKGROUND,
    YELLOW_BACKGROUND,
    BLUE_BACKGROUND,
    PURPLE_BACKGROUND,
    CYAN_BACKGROUND,
    WHITE_BACKGROUND,
    DEFAULT_BACKGROUND
};

/**
===============================================================================================================================
    @brief   - Allows to print colored text.

    @details - Use only supported colors: RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE, DEFAULT\n
             - String can be pointer to constant string, use of formats and arguements are same as in printf():\n
                + '%c' -- character symbol.\n
                + '%d' -- decimal integer.\n
                + '%i' -- decimal integer.\n
                + '%e' -- floating point number in exponential form.\n
                + '%f' -- floating point number in decimal form (length of tail can be controlled by '.num' between '%' and 'f').\n
                + '%g' -- floating point that chooses the smallest way to write between '%e' and '%f'.\n
                + '%o' -- octal integer.\n
                + '%s' -- string.\n
                + '%x' -- hexadecimal integer.\n
                + '%p' -- pointer.\n
                + '%n' -- number of read symbols.\n
                + '%u' -- unsigned integer.\n
                + '%[]' -- scans many symbols.\n
                + '%l...' -- long integer or double.\n

    @param   [in]  color              Enumerator that represants color of text.
    @param   [in]  string             String represanting format of console output.

===============================================================================================================================
*/
void color_printf(color_t color, bool is_bold, background_t background, const char *string, ...);

#endif
