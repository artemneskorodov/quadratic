#include "stack.h"
#include "custom_assert.h"
#include <stdlib.h>

struct stack_elem_t {
    stack_elem_t *next;
    void *data;
};

stack_state_t stack_push(stack_elem_t **head, void *data) {
    C_ASSERT(data != NULL, STACK_ERROR);
    C_ASSERT(head != NULL, STACK_ERROR);

    stack_elem_t *new_elem = (stack_elem_t *)calloc(1, sizeof(stack_elem_t));
    if(new_elem == NULL)
        return STACK_ERROR;

    new_elem->next = *head;
    new_elem->data = data;
    *head = new_elem;
    return STACK_SUCCESS;
}

void *stack_pop(stack_elem_t **head) {
    C_ASSERT(head != NULL, NULL);
    void *data = (*head)->data;
    stack_elem_t *next = (*head)->next;
    free(*head);
    *head = next;
    return data;
}

void free_stack(stack_elem_t *head) {
    while(head != NULL) {
        stack_elem_t *next = head->next;
        free(head);
        head = next;
    }
}
