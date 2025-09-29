# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iunity/src/ -Isrc -g

# Source files
SRC = src/calc.c
TEST = test/Testcalc.c
UNITY = unity/src/unity.c

# Output binary
OUT = build/test_runner

# Default target
all: $(OUT)
	@./$(OUT)

# Build the test binary
$(OUT): $(SRC) $(TEST) $(UNITY)
	$(CC) $(CFLAGS) $^ -o $@

# Clean build artifacts
clean:
	rm -f $(OUT)

