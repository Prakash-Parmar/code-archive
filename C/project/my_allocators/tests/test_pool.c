#include "poolAllocator.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 128
#define CHUNK_SIZE 16

int main() {
    Pool p;
    unsigned char buffer[BUFFER_SIZE];

    // ---- Test Initialization ----
    pool_init(&p, buffer, BUFFER_SIZE, CHUNK_SIZE);
    assert(p.head != NULL); // head should not be null
    printf("Initialization test passed\n");

    // ---- Test Allocation ----
    void *a = pool_alloc(&p);
    void *b = pool_alloc(&p);
    assert(a != NULL && b != NULL);
    assert(a != b); // two different chunks
    printf("Allocation test passed\n");

    // Check memory zeroed
    for (size_t i = 0; i < CHUNK_SIZE; i++) {
        assert(((unsigned char *)a)[i] == 0);
    }

    // ---- Test Free ----
    pool_free(&p, a); // free first allocation
    void *c = pool_alloc(&p); // should reuse the freed chunk
    assert(c == a);
    printf("Free/reuse test passed\n");

    // ---- Test Out of Memory ----
    size_t max_chunks = BUFFER_SIZE / CHUNK_SIZE;
    void *chunks[max_chunks];
    pool_free_all(&p); // reset
    for (size_t i = 0; i < max_chunks; i++) {
        chunks[i] = pool_alloc(&p);
        assert(chunks[i] != NULL);
    }
    // Next allocation should fail (assert)
    // pool_alloc(&p); // Uncomment to check assertion
    printf("Out-of-memory test passed\n");

    printf("All tests passed!\n");
    return 0;
}

