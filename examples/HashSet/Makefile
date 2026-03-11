# HashSet Example Makefile

# Program name
PROGRAM = HashSet

# Source files
SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)

# Build directory (can be overridden by parent makefile)
BUILD_DIR ?= build

# Compiler and flags (can be overridden by parent makefile)
CC ?= gcc
CFLAGS ?= -Wall -Wextra -std=c17 -O0 -g3

# Auto-detect paths based on where we can find the include directory
INCLUDE_PATHS := ../../include ../../../include ../../../../include
BTCHWRK_INCLUDE := $(firstword $(wildcard $(INCLUDE_PATHS)))

# Set lib path relative to include path
BTCHWRK_LIB := $(dir $(BTCHWRK_INCLUDE))build
LDFLAGS ?= -L$(BTCHWRK_LIB) -lbtchwrk

# Target executable
TARGET = $(BUILD_DIR)/$(PROGRAM)

# Default target
all: $(TARGET)

# Build the program
$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(OBJECTS) -o $@ -I$(BTCHWRK_INCLUDE) $(LDFLAGS)
	@echo "Built $(PROGRAM) successfully"

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -I$(BTCHWRK_INCLUDE) -c $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET)

# Show help
help:
	@echo "HashSet Example Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all   - Build the example (default)"
	@echo "  clean - Clean build artifacts"
	@echo "  run   - Build and run the example"
	@echo "  debug - Build with debug flags"
	@echo "  help  - Show this help message"

# Declare phony targets
.PHONY: all clean run debug help
