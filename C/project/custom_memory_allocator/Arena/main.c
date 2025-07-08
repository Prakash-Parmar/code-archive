#include<stdio.h>
#include "arena_allocator.h"
#include<stdlib.h>

int main(){
	void *mem_pool = (void *)malloc(256);
	Arena a = {0};
	arena_init(&a, mem_pool, 256);

	arena_view(&a);
	char *name = (char *)arena_alloc(&a, 8);
	arena_view(&a);

	printf("%zu\n", sizeof(int));
	return 0;
}
