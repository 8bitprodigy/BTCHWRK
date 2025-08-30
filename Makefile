# BTCHWRK Library Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -fPIC
DEBUG_CFLAGS = -Wall -Wextra -std=c99 -g -DDEBUG -fPIC
AR = ar
ARFLAGS = rcs

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
INSTALL_PREFIX = /usr/local
INSTALL_INCLUDE = $(INSTALL_PREFIX)/include/btchwrk
INSTALL_LIB = $(INSTALL_PREFIX)/lib

# Library name
LIB_NAME = libbtchwrk.a
LIB_TARGET = $(BUILD_DIR)/$(LIB_NAME)

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.h)

# Default target
all: $(LIB_TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Create static library
$(LIB_TARGET): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^
	@echo "Static library $(LIB_NAME) created successfully"

# Debug build
debug: CFLAGS = $(DEBUG_CFLAGS)
debug: $(LIB_TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Install library and headers
install: $(LIB_TARGET)
	@echo "Installing BTCHWRK library..."
	install -m 644 $(HEADERS) $(INSTALL_INCLUDE)/
	install -m 644 $(LIB_TARGET) $(INSTALL_LIB)/
	@echo "Installation complete"
	@echo "Headers installed to: $(INSTALL_INCLUDE)"
	@echo "Library installed to: $(INSTALL_LIB)""

# Uninstall library and headers
uninstall:
	@echo "Uninstalling BTCHWRK library..."
	rm -rf $(INSTALL_INCLUDE)
	rm -f $(INSTALL_LIB)/$(LIB_NAME)
	@echo "Uninstall complete"

# Show help
help:
	@echo "BTCHWRK Library Build System"
	@echo ""
	@echo "Targets:"
	@echo "  all       - Build the static library (default)"
	@echo "  debug     - Build with debug flags"
	@echo "  clean     - Remove build artifacts"
	@echo "  install   - Install library and headers to system"
	@echo "  uninstall - Remove library and headers from system"
	@echo "  help      - Show this help message"
	@echo ""
	@echo "Variables:"
	@echo "  CC           = $(CC)"
	@echo "  CFLAGS       = $(CFLAGS)"
	@echo "  INSTALL_PREFIX = $(INSTALL_PREFIX)"

# Declare phony targets
.PHONY: all debug clean install uninstall help
