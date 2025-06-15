 #include<stdio.h>
 #include<stdint.h>
 #include<stdatomic.h>
 #include<string.h>

 #define HEAP_SIZE (1024 * 1024 * 150)
 
 uint8_t _heap[HEAP_SIZE] = { 0 };
 
 typedef struct BlockHeader {
 	size_t size;
	int is_free;
	struct BlockHeader *next;
	struct BlockHeader *prev;
 } BlockHeader;
 
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
	printf("%zu \n", metaBlock->size);
	printf("%d \n", metaBlock->is_free);
	printf("%p \n", metaBlock->next);
	printf("%p \n", metaBlock->prev);
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
				size_t org_curr_block_size = curr->size;
				curr->size = total_required_size;
				curr->is_free = 0;

				// creat new free block for the remainder space
				BlockHeader *new_free_block = (BlockHeader *) ((uint8_t *) curr + curr->size);
				new_free_block->size = org_curr_block_size - curr->size;
				new_free_block->is_free = 1;
				
				// relink new free block into ll
				new_free_block->next = curr->next;
				new_free_block->prev = curr;
				if(curr->next != NULL){
					curr->next->prev = new_free_block;
				}
				curr->next = new_free_block;
				
				return (void *) ((uint8_t *) curr + sizeof(BlockHeader));

			}
		}else{
			curr = curr->next;
		}
	}
 	
	return NULL;
 }

 

 int main(int arc, char* const* arv) {
    init_allocator();

    char* first_name = (char *)my_malloc(8);  // 7 chars + null terminator
    strcpy(first_name, "prakash");

    char* second_name = (char *)my_malloc(7);  // 6 chars + null terminator
    strcpy(second_name, "parmar");

    printf("starting add is %p and value is %s\n", first_name, first_name);
    printf("starting add is %p and value is %s\n", second_name, second_name);

    printf("byte diff = %zu\n", second_name - first_name);
	
	my_free(first_name);
	my_free(second_name);


    return 0;
 }
