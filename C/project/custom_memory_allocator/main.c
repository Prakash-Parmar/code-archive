
#include "allocator.h"
#include<stddef.h>
#include<stdio.h>
#include<string.h>

 int main(int arc, char* const* arv) {
    init_allocator();

    char* first_name = (char *)my_malloc(100);  // 7 chars + null terminator
    strcpy(first_name, "prakash");

    char* second_name = (char *)my_malloc(7);  // 6 chars + null terminator
    strcpy(second_name, "parmar");

	printf("First checkpoint \n");
	
	first_name = (char *)my_realloc(first_name, 105);
	
	printf("Second Checkpoint \n");
	strcpy(first_name, "Prak");
    printf("starting add is %p and value is %s\n", first_name, first_name);
    printf("starting add is %p and value is %s\n", second_name, second_name);

    //printf("byte diff = %zu\n", second_name - first_name);
	
	//printf("----");

	my_free(first_name);
	my_free(second_name);


    return 0;
 }
