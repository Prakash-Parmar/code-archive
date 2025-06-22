// allocator.h

#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include<stddef.h>

#define HEAP_SIZE (1024 * 1024 * 150)

 typedef struct BlockHeader {
     size_t size;
     int is_free;
     struct BlockHeader *next;
     struct BlockHeader *prev;
  } BlockHeader;

void init_allocator();
void print_block_metadata(BlockHeader* metaBlock);
void my_free(void * ptr);
void split_block(BlockHeader *curr, size_t bytes);
void *my_malloc(size_t bytes);
void *my_realloc(void* ptr, size_t bytes);



#endif
