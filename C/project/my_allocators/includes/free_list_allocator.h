#ifndef FREE_LIST_ALLOCATOR_H
#define FREE_LIST_ALLOCATOR_H

#include<stdio.h>
#include<stdbool.h>

typedef struct BlockHeader BlockHeader;
struct BlockHeader {
	size_t size;
	BlockHeader *next;
};

extern BlockHeader *freeListHead;

bool is_power_of_two(uintptr_t ptr);
size_t align_header_size(size_t size, size_t align);
uintptr_t align_forward_uintptr(uintptr_t initial_start, uintptr_t alignment);


void initializeAllocator(void *backing_buffer, size_t backing_buffer_length, size_t alignment);

#endif
