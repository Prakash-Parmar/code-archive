# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I unity/src/ -I includes -g -DUNITY_USE_COLOR

#source files
SRC = src
TEST = tests
UNITY = unity/src

BUILD = build

free_list: $(BUILD)/test_free_list_allocator
	./$(BUILD)/test_free_list_allocator

# pattern rule to build each alocator 
$(BUILD)/test_%: $(SRC)/%.c $(TEST)/test_%.c $(UNITY)/unity.c
	$(CC) $(CFLAGS) $^ -o $@

# Clean all binaries
clean:
	rm -rf $(BUILD)/*

