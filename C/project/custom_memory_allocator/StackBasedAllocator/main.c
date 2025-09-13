#include "stackAllocator.h"
#include<string.h>
#define STACK_SIZE 1024

int main(){
	
	Stack s;
	
	unsigned char buffer[STACK_SIZE];

	stack_init(&s, buffer, STACK_SIZE);
    
	printf("%d \n", sizeof(Stack_Allocation_Header));
	printf("%lu \n\n", (uintptr_t)s.buf);

	void *name = stack_alloc(&s, 8);
	
	printf("%lu \n", (uintptr_t)name);

	strcpy((char*)name, "Prakash");
	
	printf("%s \n", name);

	void *surname = stack_alloc(&s, 7);

	printf("%lu \n", (uintptr_t)surname);

	strcpy((char*)surname, "Parmar");

	printf("%s \n", surname);	
		
	
	stack_free(&s, surname);
	

	void *fathername = stack_alloc(&s, 11);
		
	printf("%lu \n", (uintptr_t)fathername);

	strcpy((char*)fathername, "Rameshbhai");

	 printf("%s \n", fathername);
 
	 return 0;

}
