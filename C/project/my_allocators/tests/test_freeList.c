#include "free_list_allocator.h"
#include <stdio.h>
#include <assert.h>
#include<string.h>

uintptr_t calculate_expected_start(uintptr_t initial_start, size_t alignment) {
		uintptr_t modulo = initial_start % alignment;
		if (modulo == 0) {
				return initial_start;
		}
		return initial_start + (alignment - modulo);
}

bool check_alignment(void* ptr, size_t alignment){
		uintptr_t p, a;

		if(ptr == NULL)	return false;

		p = (uintptr_t) ptr;
		a = (uintptr_t) alignment;

		return (p%a == 0);	
}


int main() {

		// --- Test Case 1: Successful Initialization ---
		printf("Running Test Case 1: Successful Initialization...\n");
		char buffer[1024];
		size_t alignment = 8;

		// Call the function
		initializeAllocator(buffer, sizeof(buffer), alignment);

		// Perform checks
		assert(freeListHead != NULL);

		uintptr_t expected_start = calculate_expected_start((uintptr_t)buffer, alignment);
		assert((uintptr_t)freeListHead == expected_start);

		// The size check. Assuming no header size padding is applied yet.
		assert(freeListHead->size == (sizeof(buffer) - (expected_start - (uintptr_t)buffer)));

		assert(freeListHead->next == NULL);

		printf("Test Case 1 Passed!\n\n");

		// --- Test Case 2: Buffer Too Small ---
		// printf("Running Test Case 2: Buffer Too Small...\n");
		// char small_buffer[sizeof(BlockHeader) / 2];

		// Re-initialize the allocator with the small buffer
		// initializeAllocator(small_buffer, sizeof(small_buffer), alignment);

		// Perform checks
		//assert(freeListHead == NULL);

		//printf("Test Case 2 Passed!\n");

		// --- Test Case 3: memory alignment ---
		printf("Running Test Case 3: checking memory alignment...\n");

		void *ptr1 = allocator(1);
		assert(check_alignment(ptr1, alignment) && "ptr1 is not aligned");

		void *ptr2 = allocator(2);
		assert(check_alignment(ptr2, alignment) && "ptr2 is not aligned");

		void *ptr3 = allocator(3);
		assert(check_alignment(ptr3, alignment) && "ptr3 is not aligned");

		void *ptr7 = allocator(7);
		assert(check_alignment(ptr7, alignment) && "ptr7 is not aligned");

		void *ptr8 = allocator(8);
		assert(check_alignment(ptr8, alignment) && "ptr8 is not aligned");

		void *ptr9 = allocator(9);
		assert(check_alignment(ptr9, alignment) && "ptr9 is not aligned");

		printf("Test Case 3 Passed!\n");

		// --- Test Case 4: non-overlap between allocation ---
		printf("Running Test Case 4: checking non-overlap between allocation...\n");
		// Fill each with a unique value
		memset(ptr1, 0x11, 1);
		memset(ptr2, 0x22, 2);
		memset(ptr3, 0x33, 3);
		memset(ptr7, 0x77, 7);
		memset(ptr8, 0x88, 8);
		memset(ptr9, 0x99, 9);

		// Verify each block still contains its pattern
		for (size_t i = 0; i < 1; i++) {
				assert(((unsigned char*)ptr1)[i] == 0x11 && "Overlap detected in ptr1");
		}
		for (size_t i = 0; i < 2; i++) {
				assert(((unsigned char*)ptr2)[i] == 0x22 && "Overlap detected in ptr2");
		}
		for (size_t i = 0; i < 3; i++) {
				assert(((unsigned char*)ptr3)[i] == 0x33 && "Overlap detected in ptr3");
		}
		for (size_t i = 0; i < 7; i++) {
				assert(((unsigned char*)ptr7)[i] == 0x77 && "Overlap detected in ptr7");
		}
		for (size_t i = 0; i < 8; i++) {
				assert(((unsigned char*)ptr8)[i] == 0x88 && "Overlap detected in ptr8");
		}
		for (size_t i = 0; i < 9; i++) {
				assert(((unsigned char*)ptr9)[i] == 0x99 && "Overlap detected in ptr9");
		}

		printf("Test Case 4 Passed!\n");

		return 0;
}
