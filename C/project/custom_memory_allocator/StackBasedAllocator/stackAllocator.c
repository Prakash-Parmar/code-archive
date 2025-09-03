#include "stackAllocator.h"
#include <assert.h>
#include<math.h>

bool is_power_of_two(uintptr_t ptr){
	return (ptr>0)	&& ((ptr & (ptr-1)) == 0);
}


void stack_init(Stack *s, void *backing_buffer, size_t backing_buffer_length){
	s->buf = (uint8_t *) backing_buffer;
	s->buf_len = backing_buffer_length;
	s->offset = 0;
}

size_t calculate_padding_with_header(uintptr_t ptr, uintptr_t alignment, size_t header_size){
	uintptr_t p, a, modulo, padding, needed_space;

	assert(is_power_of_two(alignment));

	p = ptr;
	a = alignment;
	modulo = (p % a);

	padding = 0;
	needed_space = 0;
	
	if(modulo != 0){
		padding = a - modulo;
	}

	needed_space = (uintptr_t) header_size;

	if(padding < needed_space){
		needed_space -= padding;

		padding += a * ceil(needed_space / a);

	}

	return (size_t)padding;
}
