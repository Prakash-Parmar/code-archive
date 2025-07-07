#include<stdio.h>
# include "arena_allocator.h"
#include<stdbool.h>
#include<stdint.h>
#include <string.h>

bool is_power_of_two(uintptr_t x){
	return (x && (x-1)) == 0;
}

uintptr_t align_forward(uintptr_t ptr, size_t align){
	uintptr_t p, a, modulo;

	if(!is_power_of_two(align))
		printf("align itself is not power of 2.\n");
	
	p = ptr;
	a = (uintptr_t) align;
	modulo = ptr % align;
	if(modulo != 0){
		ptr += a - modulo;

	}
	return p;
}

void *arena_alloc_align(Arena *a, size_t size, size_t align){
	// Align curr_offset to the specified allignment
	uintptr_t curr_ptr = (uintptr_t)a->buf + (uintptr_t)a->curr_offset;
   	uintptr_t offset = align_forward(curr_ptr, align);
	offset -= (uintptr_t)a->buf;

	// Check if Arena has enough memory to hold size bytes
	if(a->buf_len - offset >= size){
		void *ptr = &a->buf[offset];
		a->prev_offset = offset;
		a->curr_offset = offset + size;

		memset(ptr, 0, size);
		return ptr;
	}

	return NULL;
}

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT (2 * sizeof(void *))
#endif

//malloc of arena
void *arena_alloc(Arena *a, size_t size){
	return arena_alloc_align(a, size, DEFAULT_ALIGNMENT);
}
