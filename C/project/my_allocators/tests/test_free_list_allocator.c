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

void test_allocator_splites_block_when_large_enough(void){

	void *ptr = allocator(100);

	TEST_ASSERT_NOT_NULL(ptr);

	TEST_ASSERT_EQUAL_UINT(0, ((uintptr_t)ptr) % G_ALIGNMENT);

	TEST_ASSERT_NOT_NULL(freeListHead);

	TEST_ASSERT_LESS_THAN(1024, freeListHead->size);
    TEST_ASSERT_GREATER_THAN(sizeof(BlockHeader), freeListHead->size);

	unsigned char *data = (unsigned char *)ptr;
    for (size_t i = 0; i < 100; i++) {
        TEST_ASSERT_EQUAL_UINT8(0, data[i]);
    }
}

void test_allocator_uses_whole_block_when_too_small_to_split(void){

	void *ptr = allocator(1008);

	TEST_ASSERT_NOT_NULL(ptr);

	TEST_ASSERT_EQUAL_UINT(0, ((uintptr_t)ptr) % G_ALIGNMENT);
	

	TEST_ASSERT_EQUAL_PTR(NULL, freeListHead);

	BlockHeader* header = (BlockHeader*) ((uintptr_t)ptr - ALIGNED_HEADER_SIZE);
	TEST_ASSERT_EQUAL_UINT(1024, header->size);

}

void test_allocator_returns_null_when_no_block_fits(void){

	void *ptr = allocator(BUFFER_SIZE+1);

	TEST_ASSERT_EQUAL_PTR(NULL, ptr);
}


void test_free_memory_null_pointer(void){
	
	freeListHead = NULL;
	free_memory(NULL);

	TEST_ASSERT_NULL(freeListHead);
}

void test_free_memory_free_into_empty_free_list(void){
	
	void *ptr = allocator(1008);
	
	free_memory(ptr);
	TEST_ASSERT_EQUAL_UINT((uintptr_t)freeListHead + ALIGNED_HEADER_SIZE ,(uintptr_t) ptr);
}

void test_free_memory_free_before_head(void){
	
	void *ptr = allocator(100);
	free_memory(ptr);

	TEST_ASSERT_EQUAL_UINT((uintptr_t)freeListHead+ALIGNED_HEADER_SIZE , (uintptr_t)ptr);
}

void test_free_memory_free_after_head_no_coalescing(void){

	void *ptr1 = allocator(16);
	void *ptr2 = allocator(16);
	void *ptr3 = allocator(16);

	free_memory(ptr1);

	TEST_ASSERT_EQUAL_UINT((uintptr_t) freeListHead + ALIGNED_HEADER_SIZE, (uintptr_t)ptr1);

	free_memory(ptr3);

	TEST_ASSERT_EQUAL_UINT((uintptr_t) freeListHead->next + ALIGNED_HEADER_SIZE, (uintptr_t)ptr3);

}

void test_free_memory_free_after_prev_block(void){

    void *ptr1 = allocator(16);
    void *ptr2 = allocator(16);
    void *ptr3 = allocator(16);


	BlockHeader *header1 = (BlockHeader*) ((char*)ptr1 - ALIGNED_HEADER_SIZE);
	BlockHeader *header2 = (BlockHeader*) ((char*) ptr2 - ALIGNED_HEADER_SIZE);
	BlockHeader *header3 = (BlockHeader*) ((char*)ptr3 - ALIGNED_HEADER_SIZE);

	int size1 = header1->size;
	int size2 = header2->size;

    free_memory(ptr1);

    TEST_ASSERT_EQUAL_PTR((char*)freeListHead, (char*)header1);

    free_memory(ptr2);
	TEST_ASSERT_EQUAL_UINT(freeListHead->size, size1+size2);


}

void test_free_memory_free_before_next_block(void){

	void *ptr1 = allocator(16);
    void *ptr2 = allocator(16);
    void *ptr3 = allocator(16);
	void *ptr4 = allocator(16);

	BlockHeader *header1 = (BlockHeader*) ((char*)ptr1 - ALIGNED_HEADER_SIZE);
    BlockHeader *header2 = (BlockHeader*) ((char*) ptr2 - ALIGNED_HEADER_SIZE);
    BlockHeader *header3 = (BlockHeader*) ((char*)ptr3 - ALIGNED_HEADER_SIZE);


	int size3 = header3->size;
	int size2 = header2->size;
	free_memory(ptr3);

	free_memory(ptr2);

	TEST_ASSERT_EQUAL_PTR(header3->next, header2->next);
	TEST_ASSERT_EQUAL_UINT(header2->size, size2+size3);
	
}


void test_free_memory_free_between_two_adj_free_blocks(void){

	void *ptr1 = allocator(16);
    void *ptr2 = allocator(16);
    void *ptr3 = allocator(16);
	void *ptr4 = allocator(16);

    BlockHeader *header1 = (BlockHeader*) ((char*)ptr1 - ALIGNED_HEADER_SIZE);
    BlockHeader *header2 = (BlockHeader*) ((char*) ptr2 - ALIGNED_HEADER_SIZE);
    BlockHeader *header3 = (BlockHeader*) ((char*)ptr3 - ALIGNED_HEADER_SIZE);

	int size1 = header1->size;
	int size2 = header2->size;
	int size3 = header3->size;

	free_memory(ptr1);
	free_memory(ptr3);

	free_memory(ptr2);

	TEST_ASSERT_EQUAL(header1->next, header3->next);
	TEST_ASSERT_EQUAL_UINT(header1->size, size1+size2+size3);

}


void test_free_all(void){

	void *ptr1 = allocator(16);
	void *ptr2 = allocator(16);

	free_all();
	
	TEST_ASSERT_NOT_NULL(g_backing_buffer_start);

	TEST_ASSERT_EQUAL_PTR((uintptr_t) g_backing_buffer_start, (uintptr_t) freeListHead);
	TEST_ASSERT_EQUAL_UINT(g_backing_buffer_length, freeListHead->size);
	TEST_ASSERT_NULL(freeListHead->next);

}

int main(void)
{
    UNITY_BEGIN();

    //RUN_TEST(test_initializeAllocator); 

	//RUN_TEST(test_allocator_splites_block_when_large_enough);
	
    //	RUN_TEST(test_allocator_uses_whole_block_when_too_small_to_split);

	//RUN_TEST(test_allocator_returns_null_when_no_block_fits);


	// test free memory
	//	RUN_TEST(test_free_memory_null_pointer);
    //RUN_TEST(test_free_memory_free_into_empty_free_list);
	//RUN_TEST(test_free_memory_free_before_head);
	//RUN_TEST(test_free_memory_free_after_head_no_coalescing);
	//RUN_TEST(test_free_memory_free_after_prev_block);
	//RUN_TEST(test_free_memory_free_before_next_block);
	//RUN_TEST(test_free_memory_free_between_two_adj_free_blocks);


	// free all memory
	RUN_TEST(test_free_all);	

	UNITY_END();

    return 0;
}

