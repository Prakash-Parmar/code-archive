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
	

	{
		Temp_Arena_Memory temp = temp_arena_memory_begin(&a);

		int *nums = arena_alloc(&a, 10 * sizeof(int));
		arena_view(&a);

		for(int i=0; i<10; i++){
			nums[i] = i*10;
		}

		printf("Allocated numbers: \n");
		for(int i=0; i<10; i++){
			printf("%d ", nums[i]);
		}
		printf("\n");

		temp_arena_memory_end(temp);

		arena_view(&a);
	}

	arena_view(&a);

	printf("\n%zu\n", sizeof(uintptr_t));
	printf("\n%zu\n", sizeof(size_t));

	return 0;
}
