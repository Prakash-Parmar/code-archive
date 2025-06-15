#include<stdio.h>
#include "stack.h"
#include<ctype.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	Stack *st = createStack(10);
	while(--argc > 0){
		char *token = *++argv;

		if(isdigit(token[0])){
			push(st, atoi(token));	
		}else{
			int item2 = pop(st);
			int item1 = pop(st);
	
			printf("%d %d \n", item1, item2);	
		
			switch(token[0]){
				case '+':
					push(st, item1 + item2);
					break;
                                case '-':
                                        push(st, item1 - item2);
                                        break;
                                case '*':
                                        push(st, item1 * item2);
                                        break;
                                case '/':
                                        if(item2 != 0)
						push(st, item1 / item2);
					else{
						printf("Error: unknown operator %c\n", token[0]);
						return 1;
					}
                                        break;
                                default:
                                        printf("Error, Unknown operator %c\n", token[0]);
                                        break;
				
			}
		}

	}

	if(!isEmpty(st))
		printf("Result: %d\n", pop(st));
	else
		printf("Error: Invalid expression\n");


	freeStack(st);

	return 0;
}
