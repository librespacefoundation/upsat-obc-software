typedef struct node {
    void *data;
    struct node *lower;
} stack_node;

typedef struct stack {
    size_t elem_size;
    size_t stack_size;
    stack_node *top;
} STACK;

enum status stack_init(STACK *stack, size_t elem_size)
{
    stack->elem_size = elem_size;
    stack->stack_size = 0;
    stack->top = NULL;
    return STACK_SUCCESS;
}

stack_node * stack_node_create(STACK *stack)
{
    stack_node *node = malloc(sizeof(stack_node));
    if (node == NULL) {
        return NULL;
    }
    node->data = malloc(stack->elem_size);
    if (node->data == NULL) {
        free(node);
        return NULL;
    }
    return node;
}

enum status stack_push(STACK *stack, void *data)
{
    stack_node *node = stack_node_create(stack);
    if (node == NULL) {
        return STACK_FAILURE;
    }

    memcpy(node->data, data, stack->elem_size);

    node->lower = stack->top;
    stack->top = node;
    stack->stack_size += 1;
    return STACK_SUCCESS;
}

enum status stack_pop(STACK *stack, void *data)
{
    if (stack->top == NULL) {
        return STACK_FAILURE;
    }
    stack_node *node = stack->top;
    memcpy(data, node->data, stack->elem_size);
    stack->top = node->lower;

    free(node->data);
    free(node);

    stack->stack_size -= 1;
    return STACK_SUCCESS;
}

enum status stack_peek(STACK *stack, void *data)
{
    if (stack->top == NULL) {
        return STACK_FAILURE;
    }
    memcpy(data, stack->top->data, stack->elem_size);
    return STACK_SUCCESS;
}

void stack_cleanup(STACK *stack)
{
    while (stack->top != NULL) {
        stack_node *node = stack->top;
        stack->top = stack->top->lower;
        free(node->data);
        free(node);
    }
}