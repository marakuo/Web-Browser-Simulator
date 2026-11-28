#include "stack.h"
#include <stdio.h>
#include <string.h>

void stack_init(Stack *stack) {
    if (!stack)return;
    stack->top=-1;
}
int stack_empty(Stack *stack) {
    return (!stack||stack->top==-1);
}

int stack_full(Stack *stack) {
    return (stack&&stack->top==MAX-1);
}
int stack_size(Stack *stack) {
    if (!stack) return 0;
    return stack->top+1;
}
int stack_push(Stack *stack, char *url) {
    if (!stack || !url) return 0;
    if (stack_full(stack)) {
        printf("stack is full\n");
        return 0;
    }
    if (strlen(url) >= max_url_len) {
        printf("URL is too long\n");
        return 0;
    }
    stack->top++;
    strcpy(stack->data[stack->top],url);
    return 1;
}

int stack_pop(Stack *stack,char *url) {
    if (stack_empty(stack)||!url) {
        printf("stack is empty\n");
        return 0;
    }
    strcpy(url, stack->data[stack->top]);
    stack->top--;
    return 1;
}

int stack_peek(Stack *stack,char *url) {
    if (stack_empty(stack)||!url) {
        printf("stack is empty\n");
        return 0;
    }
    strcpy(url, stack->data[stack->top]);
    return 1;

}

void stack_clear(Stack *stack) {
    stack_init(stack);
}


