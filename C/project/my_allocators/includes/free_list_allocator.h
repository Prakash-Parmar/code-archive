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
extern size_t G_ALIGNMENT;
extern size_t ALIGNED_HEADER_SIZE;
extern void* g_backing_buffer_start;
extern size_t g_backing_buffer_length;

bool is_power_of_two(uintptr_t ptr);
size_t align_header_size(size_t size);
uintptr_t align_forward_uintptr(uintptr_t initial_start);


void initializeAllocator(void *backing_buffer, size_t backing_buffer_length, size_t alignment);
void* allocator(size_t size);

void free_memory(void *ptr);

void free_all();

#endif
