CC = gcc
CFLAGS = -Wall -Wextra -g -I includes/

all: final

final : test_pool.o poolAllocator.o
	$(CC) $(CFLAGS) test_pool.o poolAllocator.o -o final

# Rules for sources in src/
%.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rules for test case
%.o : tests/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm test_pool.o poolAllocator.o
