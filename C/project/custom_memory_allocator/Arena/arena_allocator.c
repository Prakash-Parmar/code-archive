#include<stdio.h>
# include "arena_allocator.h"
#include<stdbool.h>
#include<stdint.h>
#include <string.h>

bool is_power_of_two(uintptr_t x){
	return (x != 0) && ((x && (x-1)) == 0);
}

uintptr_t align_forward(uintptr_t ptr, size_t align){
	uintptr_t p, a, modulo;

	if(!is_power_of_two(align)){
		printf("\n%d\n", align);
		printf("align itself is not power of 2.\n");
	}
	p = ptr;
	a = (uintptr_t) align;
	modulo = ptr % align;
	if(modulo != 0){
		p += a - modulo;

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

void arena_init(Arena *a, void *backing_buffer, size_t backing_buffer_len){
	a->buf = (unsigned char *)backing_buffer;
	a->buf_len = backing_buffer_len;
	a->prev_offset = 0;
	a->curr_offset = 0;
}

void arena_free(Arena *a, void *ptr){
	//Do Nothing
}

void *arena_resize_align(Arena *a, void *current_block, size_t curr_size, size_t new_size, size_t align){
	
	unsigned char *curr_block = (unsigned char *)current_block;
	if(!is_power_of_two(align)){
		printf("align is not power of two\n");
		return NULL;
	}

	if(curr_block == NULL || curr_size==0)
			return arena_alloc_align(a, new_size, align);
	else if(a->buf <= curr_block && curr_block <= a->buf + a->buf_len){
		if(a->buf + a->prev_offset == curr_block){
			a->curr_offset = a->prev_offset + new_size;
			if(new_size > curr_size){
				memset(&a->buf[a->curr_offset], 0, new_size-curr_size);
			}
			return curr_block;
		}else{
			void *new_block = arena_alloc_align(a, new_size, align);
			size_t copy_size = curr_size > new_size ? new_size : curr_size;
			memmove(new_block, curr_block, copy_size);
			return new_block;
		}
	}else{
		printf("the memory block you provided is not in the scope of this Arena\n");
		return NULL;
	}
}


void *arena_resize(Arena *a, void *curr_block, size_t curr_size, size_t new_size) {
	return arena_resize_align(a, curr_block, curr_size, new_size, DEFAULT_ALIGNMENT);
}

void arena_free_all(Arena *a){
	a->curr_offset = 0;
	a->prev_offset = 0;
}

void arena_view(Arena *a){
	printf("Arena->buf: %p\n", a->buf);
	printf("Arena->buf_len: %zu\n", a->buf_len);
	printf("Arena->curr_offset: %zu\n", a->curr_offset);
	printf("Arena->prev_offset: %zu\n", a->prev_offset);
}
