# BTCHWRK Examples Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
DEBUG_CFLAGS = -Wall -Wextra -std=c99 -g -DDEBUG

# Directories
BTCHWRK_INCLUDE = ../include
BTCHWRK_LIB = ../build
EXAMPLES_DIR = .
BUILD_DIR = build

# Library linking
LDFLAGS = -L$(BTCHWRK_LIB) -lbtchwrk

# Find all example subdirectories (directories containing a Makefile)
EXAMPLE_DIRS = $(shell find $(EXAMPLES_DIR) -mindepth 1 -maxdepth 1 -type d -exec test -f {}/Makefile \; -print | sort)
EXAMPLE_NAMES = $(notdir $(EXAMPLE_DIRS))
EXAMPLE_TARGETS = $(EXAMPLE_NAMES:%=$(BUILD_DIR)/%)

# Default target - build all examples
all: $(BUILD_DIR) $(EXAMPLE_TARGETS)
	@echo "All examples built successfully"

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build individual examples
$(BUILD_DIR)/%: %/Makefile | $(BUILD_DIR)
	@echo "Building example: $*"
	$(MAKE) -C $* BUILD_DIR=../$(BUILD_DIR) \
		CC=$(CC) \
		CFLAGS="$(CFLAGS)"

# Debug build
debug: CFLAGS = $(DEBUG_CFLAGS)
debug: all

# Clean all examples
clean:
	@echo "Cleaning all examples..."
	rm -rf $(BUILD_DIR)
	@for dir in $(EXAMPLE_DIRS); do \
		echo "Cleaning $$dir..."; \
		$(MAKE) -C $$dir clean BUILD_DIR=../$(BUILD_DIR) || true; \
	done

# Run all examples
run: all
	@echo "Running all examples..."
	@for example in $(EXAMPLE_NAMES); do \
		echo ""; \
		echo "=== Running $$example ==="; \
		if [ -x "$(BUILD_DIR)/$$example" ]; then \
			./$(BUILD_DIR)/$$example; \
		else \
			echo "Example $$example not found or not executable"; \
		fi; \
	done

# Run specific example
run-%: $(BUILD_DIR)/%
	@echo "Running example: $*"
	./$(BUILD_DIR)/$*

# List available examples
list:
	@echo "Available examples:"
	@for dir in $(EXAMPLE_NAMES); do \
		echo "  $$dir"; \
	done

# Show help
help:
	@echo "BTCHWRK Examples Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all       - Build all examples (default)"
	@echo "  debug     - Build all examples with debug flags"
	@echo "  clean     - Clean all examples"
	@echo "  run       - Build and run all examples"
	@echo "  run-NAME  - Build and run specific example (e.g., run-test_Sequence)"
	@echo "  list      - List available examples"
	@echo "  help      - Show this help message"
	@echo ""
	@echo "Available examples:"
	@for dir in $(EXAMPLE_NAMES); do \
		echo "  $$dir"; \
	done

# Declare phony targets
.PHONY: all debug clean run list help $(addprefix run-,$(EXAMPLE_NAMES))
