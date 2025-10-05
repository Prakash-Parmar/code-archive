#define UNITY_OUTPUT_COLOR
#include "unity.h"
#include "free_list_allocator.h"

#define BUFFER_SIZE 1024
static char buffer[BUFFER_SIZE];  // global test buffer
static size_t alignment;          // shared test alignment value


void setUp()
{

	alignment = 8; 
	initializeAllocator(buffer, BUFFER_SIZE, alignment);


}

void tearDown()
{
	freeListHead = NULL;
}

void test_initializeAllocator(void){
	
	TEST_ASSERT_EQUAL(alignment, G_ALIGNMENT);
	TEST_ASSERT_TRUE(ALIGNED_HEADER_SIZE % alignment == 0);
	TEST_ASSERT_NOT_NULL(freeListHead);

	uintptr_t addr = (uintptr_t) freeListHead;
	TEST_ASSERT_TRUE(addr % alignment == 0);

	TEST_ASSERT_EQUAL_PTR(NULL, freeListHead->next);
	
	// verify usable size
	uintptr_t initial_start = (uintptr_t)buffer;
	uintptr_t aligned_start = (initial_start % alignment == 0) ? initial_start : initial_start + (alignment - (initial_start % alignment)) ;

	size_t expected_size = BUFFER_SIZE - (aligned_start - initial_start);
	TEST_ASSERT_EQUAL(expected_size, freeListHead->size);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_initializeAllocator); 

    UNITY_END();

    return 0;
}

