#include "free_list_allocator.h"
#include<assert.h>


// This is the head of the free list
BlockHeader *freeListHead = NULL;


bool is_power_of_two(uintptr_t ptr){
	return (ptr>0)	&& ((ptr & (ptr-1)) == 0);
}

// Aligns the given size to the next multiple of the specified alignment (which must be a power of two).
size_t align_header_size(size_t size, size_t align) {
    assert((align & (align - 1)) == 0); // must be power of two
    size_t modulo = size % align;
    if (modulo == 0) {
        return size;
    }
    return size + (align - modulo);
}

uintptr_t align_forward_uintptr(uintptr_t initial_start, uintptr_t alignment){
	uintptr_t p, a, modulo, padding;

	assert(is_power_of_two(alignment));

	p = initial_start;
	a = alignment;

	modulo = p % a;

	if(modulo == 0)
			return p;

	padding = a - modulo;

	return p + padding;

}

void initializeAllocator(void *backing_buffer, size_t backing_buffer_length, size_t alignment){
	
	//Ensure the header size is a multiple of the alignment
	size_t aligned_header_size = align_header_size(sizeof(BlockHeader), alignment);

	uintptr_t initial_start = (uintptr_t) backing_buffer;
	uintptr_t aligned_start = align_forward_uintptr(initial_start, alignment);	
	backing_buffer_length -= (size_t) (aligned_start - initial_start);


	//the initial block must be large enough to hold atleast a blockheader
	assert(backing_buffer_length > aligned_header_size);

	//initially whole memory is single block
	freeListHead = (BlockHeader *) aligned_start;
	freeListHead->size = backing_buffer_length;
	freeListHead->next = NULL;

	return;
}
