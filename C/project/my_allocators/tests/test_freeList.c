#include "free_list_allocator.h"
#include <stdio.h>
#include <assert.h>


uintptr_t calculate_expected_start(uintptr_t initial_start, size_t alignment) {
    uintptr_t modulo = initial_start % alignment;
    if (modulo == 0) {
        return initial_start;
    }
    return initial_start + (alignment - modulo);
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
    printf("Running Test Case 2: Buffer Too Small...\n");
    char small_buffer[sizeof(BlockHeader) / 2];

    // Re-initialize the allocator with the small buffer
    initializeAllocator(small_buffer, sizeof(small_buffer), alignment);

    // Perform checks
    assert(freeListHead == NULL);

    printf("Test Case 2 Passed!\n");

    return 0;
}
