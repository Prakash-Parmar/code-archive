// arena_allocator.h

#ifndef ARENA_ALLOCATOR_H
#define ARENA_ALLOCATOR_H

#include<stdbool.h>
#include<stdint.h>
#include<stdio.h>
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
void arena_init(Arena *a, void *backing_buffer, size_t backing_buffer_len);
void arena_free(Arena *a, void *ptr);
void *arena_resize_align(Arena *a, void *current_block, size_t curr_size, size_t new_size, size_t align);
void *arena_resize(Arena *a, void *curr_block, size_t curr_size, size_t new_size);
void arena_free_all(Arena *a);

void arena_view(Arena *a);

#endif
