#include "poolAllocator.h"
#include <assert.h>
#include<string.h>


bool is_power_of_two(uintptr_t ptr){
	return (ptr>0)	&& ((ptr & (ptr-1)) == 0);
}



uintptr_t align_forward_uintptr(uintptr_t initial_start, uintptr_t chunk_alignment){
	uintptr_t p, a, modulo, padding;
	
	assert(is_power_of_two(chunk_alignment));

	p = initial_start;
	a = chunk_alignment;

	modulo = p % a;

	if(modulo == 0)
			return p;

	padding = a - modulo;
	
	return p + padding;

}

size_t align_forward_size(size_t size, size_t align) {
    assert((align & (align - 1)) == 0); // must be power of two
    size_t modulo = size % align;
    if (modulo == 0) {
        return size;
    }
    return size + (align - modulo);
}


void pool_init(Pool *p, void *backing_buffer, size_t backing_buffer_len, size_t chunk_size, size_t chunk_alignment){
	
	uintptr_t initial_start = (uintptr_t) backing_buffer;
	uintptr_t start = align_forward_uintptr(initial_start, chunk_alignment);
	backing_buffer_len -= (size_t) (start - initial_start);

	chunk_size = align_forward_size(chunk_size, chunk_alignment);

	assert(chunk_size >= sizeof(Pool_Free_Node) &&
	       "Chunk size is too small");
	assert(backing_buffer_len >= chunk_size &&
	       "Backing buffer length is smaller than the chunk size");

	p->buf = (unsigned char *) backing_buffer;
	p->buf_len = backing_buffer_len;
	p->chunk_size = chunk_size;
	p->head = NULL;

	pool_free_all(p);
}

void pool_free_all(Pool *p){
	size_t chunk_count = p->buf_len / p->chunk_size;
	size_t i;

	for(i=0; i<chunk_count; i++){
		void *ptr = &p->buf[i * p->chunk_size];
		Pool_Free_Node *node = (Pool_Free_Node *)ptr;

		node->next = p->head;
		p->head = node;
	}
}


void *pool_alloc(Pool *p) {
	// Get latest free node
	Pool_Free_Node *node = p->head;

	if (node == NULL) {
		assert(0 && "Pool allocator has no free memory");
		return NULL;
	}

	// Pop free node
	p->head = p->head->next;

	// Zero memory by default
	return memset(node, 0, p->chunk_size);
}


void pool_free(Pool *p, void *ptr) {
	Pool_Free_Node *node;

	void *start = p->buf;
	void *end = &p->buf[p->buf_len];

	if (ptr == NULL) {
		// Ignore NULL pointers
		return;
	}

	if (!(start <= ptr && ptr < end)) {
		assert(0 && "Memory is out of bounds of the buffer in this pool");
		return;
	}

	// Push free node
	node = (Pool_Free_Node *)ptr;
	node->next = p->head;
	p->head = node;
}
