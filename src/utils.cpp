/**
===============================================================================================================================
    @file    utils.cpp
    @brief   Comparison of doubles with zero and reading test cases from file.
    @date    23.08.2024
    @author  Artem Neskorodov
    @link    https://vk.com/neskorodovartem

===============================================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "quadratic.h"
#include "utils.h"
#include "custom_assert.h"

static const int FILE_LINE_NUMBERS = 6;

bool is_zero(double num) {
    if(fabs(num) < EPSILON)
        return true;

    return false;
}

compare_state_t compare_with_zero(double a) {
    if(is_zero(a) == true)
        return EQUALS;
    if(a > 0.0)
        return BIGGER;
    return LESS;
}

reading_state_t read_expected_line(FILE *file, quadratic_equation_t *equation) {
    C_ASSERT(file != NULL, READING_ERROR);
    C_ASSERT(equation != NULL, READING_ERROR);
    if(file == NULL)
        return READING_ERROR;

    int read_state = fscanf(file, "%lg %lg %lg %lg %lg %d\n",
                            &equation->a, &equation->b, &equation->c,
                            &equation->x1, &equation->x2, (int*)&equation->number);

    if(read_state == EOF)
        return READING_END;
    if(read_state != FILE_LINE_NUMBERS)
        return READING_ERROR;
    if(equation->number == NOT_SOLVED)
        return READING_ERROR;

    return READING_SUCCESS;
}

bool is_minus_zero(double number) {
    const uint64_t minus_zero = 0x8000000000000000;
    uint64_t bits = *(uint64_t *)&number;
    if(minus_zero == bits)
        return true;
    return false;
}

bool is_equal(double a, double b) {
    if(is_zero(a - b))
        return true;
    return false;
}
