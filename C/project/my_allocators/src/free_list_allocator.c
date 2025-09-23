#include "free_list_allocator.h"
#include<assert.h>
#include<string.h>

// This is the head of the free list
BlockHeader *freeListHead = NULL;
size_t G_ALIGNMENT = 0;
size_t ALIGNED_HEADER_SIZE = 0;

bool is_power_of_two(uintptr_t ptr){
		return (ptr>0)	&& ((ptr & (ptr-1)) == 0);
}

// Aligns the given size to the next multiple of the specified alignment (which must be a power of two).
size_t align_size_forward(size_t size) {
		assert((G_ALIGNMENT & (G_ALIGNMENT - 1)) == 0); // must be power of two
		size_t modulo = size % G_ALIGNMENT;
		if (modulo == 0) {
				return size;
		}
		return size + (G_ALIGNMENT - modulo);
}

uintptr_t align_forward_uintptr(uintptr_t initial_start){
		uintptr_t p, a, modulo, padding;

		assert(is_power_of_two(G_ALIGNMENT));

		p = initial_start;
		a = (uintptr_t)G_ALIGNMENT;

		modulo = p % a;

		if(modulo == 0)
				return p;

		padding = a - modulo;

		return p + padding;

}

void initializeAllocator(void *backing_buffer, size_t backing_buffer_length, size_t alignment){

		G_ALIGNMENT = alignment;

		//Ensure the header size is a multiple of the alignment
		ALIGNED_HEADER_SIZE = align_size_forward(sizeof(BlockHeader));

		uintptr_t initial_start = (uintptr_t) backing_buffer;
		uintptr_t aligned_start = align_forward_uintptr(initial_start);	
		backing_buffer_length -= (size_t) (aligned_start - initial_start);


		//the initial block must be large enough to hold atleast a blockheader
		assert(backing_buffer_length > ALIGNED_HEADER_SIZE);

		//initially whole memory is single block
		freeListHead = (BlockHeader *) aligned_start;
		freeListHead->size = backing_buffer_length;
		freeListHead->next = NULL;

		return;
}

void* allocator(size_t size){

		assert(freeListHead != NULL && "Please initialize allocator with backing buffer");

		size_t required_block_size = size + ALIGNED_HEADER_SIZE;
		size_t aligned_block_size = align_size_forward(required_block_size);

		BlockHeader *curr = freeListHead;
		BlockHeader *prev = NULL;


		// Loop through the free list to find a suitable block
		while (curr != NULL) {
				if (curr->size >= aligned_block_size) {
						// We found a block that's big enough

						// Calculate the remaining size
						size_t remaining_size = curr->size - aligned_block_size;

						// Check if the remaining size is large enough to form a new free block
						if (remaining_size > sizeof(BlockHeader)) {
								// Split the block
								BlockHeader* newBlock = (BlockHeader*)((char*)curr + aligned_block_size);
								newBlock->size = remaining_size;
								newBlock->next = curr->next;

								curr->size = aligned_block_size; // Update the current block's size

								// Unlink curr and link in newBlock
								if (prev == NULL) {
										freeListHead = newBlock;
								} else {
										prev->next = newBlock;
								}

						} else {
								// No split, just unlink the block
								if (prev == NULL) {
										freeListHead = curr->next;
								} else {
										prev->next = curr->next;
								}


						}
						void *ptr = (char*)curr + ALIGNED_HEADER_SIZE;
						memset(ptr, 0, size);

						// Return the pointer to the user's data
						return ptr;
				}

				// Move to the next block if the current one isn't big enough
				prev = curr;
				curr = curr->next;
		}

		// No suitable block found
		return NULL;

}

void printBlockHeader(BlockHeader *header){

		printf("\n-----------------------------------\n");
		printf("header = %lu\n", (uintptr_t)header);
		printf("header->size = %lu\n",header->size);
		printf("header->next = %lu\n",(uintptr_t)header->next);
		printf("-----------------------------------\n");
}


void free_memory(void* ptr) {
		if (ptr == NULL) {
				return; // Nothing to free
		}



		// Get the block header by moving back from the user's pointer
		BlockHeader* blockToFree = (BlockHeader*)((char*)ptr - ALIGNED_HEADER_SIZE);

		printBlockHeader(blockToFree);

		// Find the correct insertion point in the address-sorted free list
		BlockHeader* currentBlock = freeListHead;
		BlockHeader* prevBlock = NULL;

		while (currentBlock != NULL && currentBlock < blockToFree) {
				prevBlock = currentBlock;
				currentBlock = currentBlock->next;
		}

		// --- Coalescing with the previous block ---
		if (prevBlock != NULL && (char*)prevBlock + prevBlock->size == (char*)blockToFree) {
				// Merge with the previous block
				prevBlock->size += blockToFree->size;
				blockToFree = prevBlock;
				// The block is now merged, so we don't need to insert it
		} else {
				// Insert the new block into the list
				if (prevBlock == NULL) {
						blockToFree->next = freeListHead;
						freeListHead = blockToFree;
				} else {
						blockToFree->next = prevBlock->next;
						prevBlock->next = blockToFree;
				}
		}

		// --- Coalescing with the next block ---
		// The check is slightly different because blockToFree might have merged with prevBlock
		//currentBlock = blockToFree->next; 
		if (currentBlock != NULL && (char*)blockToFree + blockToFree->size == (char*)currentBlock) {
				// Merge with the next block
				blockToFree->size += currentBlock->size;
				blockToFree->next = currentBlock->next;
		}

		printBlockHeader(blockToFree);
}






















