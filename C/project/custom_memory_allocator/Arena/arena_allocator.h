// arena_allocator.h

#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#include<stdbool.h>

typedef struct Arena Arena;
struct Arena {
	unsigned char *buf;
	size_t buf_len;
	size_t prev_offset;
	size_t curr_offset;
};

bool is_power_of_two(uintptr_t x);
uintptr_t align_forward(uintptr_t ptr, size_t align);
void *arena_alloc_align(Arena *a, size_t size, size_t align);
void *arena_alloc(Arena *a, size_t size);

#endif
