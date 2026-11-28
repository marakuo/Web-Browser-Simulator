
#ifndef WEB_BROWSER_SIMULATOR_STACK_H
#define WEB_BROWSER_SIMULATOR_STACK_H
#define MAX 100
#define max_url_len 256

typedef struct stack {
    char data[MAX][max_url_len];
    int top;
}Stack;

void stack_init(Stack *stack);
int stack_empty(Stack *stack);
int stack_full(Stack *stack);
int stack_size(Stack *stack);
int stack_push(Stack *stack,char *url);
int stack_pop(Stack *stack,char *url);
int stack_peek(Stack *stack,char *url);
void stack_clear(Stack *stack);




#endif //WEB_BROWSER_SIMULATOR_STACK_H
