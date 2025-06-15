#include<stdio.h>
#include "stack.h"
#include<stdlib.h>

// Initialze the stack
Stack* createStack(int capacity){
	// Allocate memory for the stack
	Stack *stack = (Stack *)malloc(sizeof(Stack));

	//set the capacity
	stack->capacity = capacity;

	// Initialize top to -1
	stack->top = -1;

	// Allocate memory for the array
	stack->array = (int *)malloc(capacity * sizeof(int));

	// return the pointer to the stack
	return stack;
}

void push(Stack* st, int item){
	if(++st->top < st->capacity){
		st->array[st->top] = item;
	}else{
		printf("Stack overflow\n");
	}
		
	return;
}

int pop(Stack* st){
	if(st->top == -1){
		printf("Stack is empty \n");
		return -1;
	}

	return st->array[st->top--];
}

int peek(Stack* st){
	if(st->top == -1){
		printf("Stack is empty\n");
		return -1;
	}

	return st->array[st->top];
}

int isEmpty(Stack *st){
	if(st->top == -1)
		return 1;
	
	return 0;
}

void freeStack(Stack* st){
	free(st->array);
	free(st);
}
