//allocator.c

#include "allocator.h"
#include<stdio.h>
#include<stdint.h>
#include<math.h>
#include<stddef.h>
#include<string.h>


uint8_t _heap[HEAP_SIZE] = { 0 };

// Global pointer to the start of linked list
static BlockHeader *head_block = NULL;

 void init_allocator(){
    head_block = (BlockHeader *) _heap;
    head_block->size = HEAP_SIZE;
    head_block->is_free = 1;
    head_block->next = NULL;
    head_block->prev = NULL;
 }

void print_block_metadata(BlockHeader* metaBlock){

    printf("\n");
    printf("Address: %p\n", metaBlock);
    printf("size: %zu \n", metaBlock->size);
    printf("is_free: %d \n", metaBlock->is_free);
    printf("next: %p \n", metaBlock->next);
    printf("prev: %p \n", metaBlock->prev);
    printf("\n");

    return;
}

 void my_free(void * ptr){
    if(ptr == NULL) return;

    BlockHeader *block_to_free = (BlockHeader *)((uint8_t *) ptr - sizeof(BlockHeader));

    print_block_metadata(block_to_free);

    block_to_free->is_free = 1;

    //merge adj block to prevent fragmentation

    //merge prev block
    while(block_to_free->prev && block_to_free->prev->is_free){
        block_to_free->prev->size +=  block_to_free->size;

        //relink adj block
        block_to_free->prev->next = block_to_free->next;
        if(block_to_free->next){
            block_to_free->next->prev = block_to_free->prev;
        }

        block_to_free = block_to_free->prev;
    }

    //merge next block
    while(block_to_free->next && block_to_free->next->is_free){
        block_to_free->size += block_to_free->next->size;

        //relink the adj block
        block_to_free->next = block_to_free->next->next;
        if(block_to_free->next){
            block_to_free->next->prev = block_to_free;
        }
    }

    print_block_metadata(block_to_free);

    return;
 }

 void split_block(BlockHeader *curr, size_t bytes){
    if(curr->size <= bytes || curr->size - bytes <= sizeof(BlockHeader))
            return;
    size_t main_block_size = curr->size;
    curr->size = bytes;
    curr->is_free = 0;
    BlockHeader *new_block = (BlockHeader *) ((uint8_t *) curr + curr->size);
    new_block->size = main_block_size - curr->size;
    new_block->is_free = 1;

    // relink new free block into ll
    new_block->next = curr->next;
    new_block->prev = curr;
    if(curr->next != NULL){
        curr->next->prev = new_block;
    }
    curr->next = new_block;

    return;
 }

 void *my_malloc(size_t bytes){
    size_t total_required_size = bytes + sizeof(BlockHeader);

    //iterate through blocks to find the free block
    BlockHeader *curr = head_block;
    while(curr){
        if(curr->is_free && curr->size >= total_required_size){
            if(curr->size - total_required_size <= sizeof(BlockHeader)){
                curr->is_free = 0;
                return (void*)((uint8_t *)curr + sizeof(BlockHeader));
            }else{
                split_block(curr, total_required_size);
                return (void *) ((uint8_t *) curr + sizeof(BlockHeader));

            }
        }else{
            curr = curr->next;
        }
    }

    return NULL;
 }

 void *my_realloc(void* ptr, size_t bytes){
    if(ptr == NULL) return my_malloc(bytes);

    if(bytes==0) {
        my_free(ptr);
        return NULL;
    }

    BlockHeader *block_before_resize = (BlockHeader *)((uint8_t *)ptr - sizeof(BlockHeader));
    size_t new_total_required_size = bytes + sizeof(BlockHeader);

    //shrink the block
    if(new_total_required_size <= block_before_resize->size){
        //split the block if after allcoatin required bytes there is still enough space left
        if(block_before_resize->size - new_total_required_size >= sizeof(BlockHeader)){
            //BlockHeader *new_block = (BlockHeader *)((uint8_t *)block_before_resize + new_total_required_size);
            //new_block->size = block_before_resize->size - new_total_required_size;
            //new_block->is_free = 1;
            //new_block->next = block_before_resize->next;
            //new_block->prev = block_before_resize;

            //block_before_resize->size = new_total_required_size;
            //block_before_resize->next = new_block;
            //if(new_block->next) new_block->next->prev = new_block;
            split_block(block_before_resize, new_total_required_size);
            return ptr;
        }else{
            return ptr;
        }
    }

    // grow the block


    //first check if next block is free and has enough size
    if(block_before_resize->next && block_before_resize->next->is_free && block_before_resize->next->size >= new_total_required_size - block_before_resize->size){
        block_before_resize->size += block_before_resize->next->size;
        block_before_resize->next = block_before_resize->next->next;

        //printf("Checkpoint 3\n");

        if(block_before_resize->next){
            block_before_resize->next->prev = block_before_resize;
        }


        //printf("Checkpoint 4\n");
        return ptr;
    }

    //if the next block is not big enough to satisfy bytes size
    // go through the linked list starting from head and find if there any block exist with sufficient size
    void *new_ptr = (void *)my_malloc(bytes);
    if(new_ptr == NULL)
            printf("Out of memory!");
    else{
        // we found the block that is big enough to store total bytes
        //copy the old block content to new block
        size_t bytes_to_copy = fmin(bytes, block_before_resize->size - sizeof(BlockHeader));
        memcpy(new_ptr, ptr, bytes_to_copy);

        //split the block if it could be
        split_block(new_ptr, new_total_required_size);

        // free old block poinint by ptr
        my_free(ptr);
        return new_ptr;
    }

    return NULL;
 }

