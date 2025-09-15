#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H


#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>


typedef struct Pool_Free_Node Pool_Free_Node;
struct Pool_Free_Node {
	Pool_Free_Node *next;
};

typedef struct Pool Pool;
struct Pool {
	unsigned char *buf;
	size_t buf_len;
	size_t chunk_size;

	Pool_Free_Node *head;
};


void pool_free_all(Pool *p);
void pool_init_align(Pool *p, void *backing_buffer, size_t backing_buffer_length, size_t chunk_size, size_t chunk_alignment);
void pool_init(Pool *p, void *backing_buffer, size_t backing_buffer_length, size_t chunk_size);
void *pool_alloc(Pool *p);
void pool_free(Pool *p, void *ptr);

uintptr_t align_forward_uintptr(uintptr_t initial_start, uintptr_t chunk_alignment);
size_t align_forward_size(size_t size, size_t align);
bool is_power_of_two(uintptr_t ptr);

#endif
