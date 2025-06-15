#ifndef STACK_H
#define STACK_H

typedef struct {
	int *array;
	int top;
	int capacity;
}Stack;

// Function prototypes
Stack* createStack(int capacity);
void push(Stack* stack, int item);
int pop(Stack* stack);
int peek(Stack* stack);
int isEmpty(Stack* stack);
void freeStack(Stack* stack);

#endif
