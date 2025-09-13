#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>

typedef struct Stack Stack;

struct Stack {
	unsigned char *buf;
	size_t buf_len;
	size_t offset;

};

typedef struct Stack_Allocation_Header Stack_Allocation_Header;

struct Stack_Allocation_Header{
	uint8_t padding;
};

bool is_power_of_two(uintptr_t ptr);

void stack_init(Stack *s, void *backing_buffer, size_t backing_buffer_length);
size_t calculate_padding_with_header(uintptr_t ptr, uintptr_t alignment, size_t header_size);

void *stack_alloc_align(Stack* s, size_t size, size_t alignment);
void *stack_alloc(Stack* s, size_t size);

void stack_free(Stack* s, void *ptr);
void *stack_resize_align(Stack *s, void *ptr, size_t old_size, size_t new_size, size_t alignment);
void *stack_resize(Stack *s, void *ptr, size_t old_size, size_t new_size);
void stack_free_all(Stack *s);

#endif

