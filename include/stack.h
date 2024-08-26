#ifndef STACK_H
#define STACK_H

struct stack_elem_t;

enum stack_state_t {
    STACK_ERROR,
    STACK_SUCCESS
};

/**
================================================================================================================================
    @brief   - Inserts data to stack

    @details - If data pointer is NULL (no elements addded yet) function allocates memory for first elem.\n

    @param   [in]  head               Pointer to pointer to stack structure.
    @param   [in]  data               Pointer to data which stack_push(...) will add to stack.

    @return  Error (or success) code.

    @note    Create a pointer to stack_elem_t in your code ( stack_elem_t *name = NULL; ).

===============================================================================================================================
*/
stack_state_t stack_push(stack_elem_t **head, void *data);

/**
================================================================================================================================
    @brief   - Pops element from stack

    @details - Returns pointer to data.\n
             - If data was dinamically allocated, don't forget to free it after use.\n
             - Always use free_stack(...).

    @param   [in]  head               Pointer to pointer to stack structure.

    @return  Pointer to stroing data.

===============================================================================================================================
*/
void *stack_pop(stack_elem_t **head);

/**
================================================================================================================================
    @brief   - Frees stack

    @param   [in]  head               Pointer to stack structure.

===============================================================================================================================
*/
void free_stack(stack_elem_t *head);

#endif
