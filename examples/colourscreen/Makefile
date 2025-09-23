# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eiku example <example@eiku.dev>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/22 23:30:00 by example           #+#    #+#              #
#    Updated: 2025/09/22 23:30:00 by example          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CC = clang
CFLAGS = -Wall -Wextra -Werror -g -std=gnu17 -O2
LDFLAGS = -lm -lX11 -lXext

# Project structure
NAME = colourscreen
EIKU_ROOT = ../..
EIKU_INCLUDE = $(EIKU_ROOT)/include
EIKU_LIB_DIR = $(EIKU_ROOT)/target/x86_64-linux-gnu/eiku/bin
EIKU_LIB = $(EIKU_LIB_DIR)/libeiku.so

# Source files
SRCDIR = .
SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)

# Include paths
INCLUDES = -I$(EIKU_INCLUDE)

# Library paths
LIBPATHS = -L$(EIKU_LIB_DIR)
LIBS = -leiku $(LDFLAGS)

.PHONY: all clean run help

# Default target
all: $(NAME)

# Main executable
$(NAME): $(OBJECTS) $(EIKU_LIB)
	@echo "Linking $(NAME)..."
	$(CC) $(CFLAGS) $(OBJECTS) -o $(NAME) $(LIBPATHS) $(LIBS)
	@echo "✓ $(NAME) built successfully!"

# Object files
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Check if Eiku library exists
$(EIKU_LIB):
	@echo "Error: Eiku library not found at $(EIKU_LIB)"
	@echo "Please build the Eiku library first:"
	@echo "  cd $(EIKU_ROOT) && make all"
	@exit 1

# Run the example
run: $(NAME)
	@echo "Running Color Gradient Demo..."
	@echo "Setting up library path..."
	LD_LIBRARY_PATH=$(EIKU_LIB_DIR):$$LD_LIBRARY_PATH ./$(NAME)

# Clean build files
clean:
	@echo "Cleaning build files..."
	rm -f $(OBJECTS) $(NAME)
	@echo "✓ Clean complete"

# Help target
help:
	@echo "Eiku Color Gradient Demo - Makefile Help"
	@echo ""
	@echo "Available targets:"
	@echo "  all     - Build the color gradient demo (default)"
	@echo "  run     - Build and run the demo"
	@echo "  clean   - Remove build files"
	@echo "  help    - Show this help message"
	@echo ""
	@echo "Prerequisites:"
	@echo "  - Eiku library must be built first:"
	@echo "    cd $(EIKU_ROOT) && make all"
	@echo ""
	@echo "Usage examples:"
	@echo "  make          # Build the demo"
	@echo "  make run      # Build and run the demo"
	@echo "  make clean    # Clean build files"
	@echo ""
	@echo "Demo features:"
	@echo "  - Creates an $(WINDOW_WIDTH)x$(WINDOW_HEIGHT) window"
	@echo "  - Displays a beautiful RGB gradient"
	@echo "  - Demonstrates pixel-level drawing"
	@echo "  - Shows real-time rendering progress"

# Define window dimensions for help (these should match main.c)
WINDOW_WIDTH = 800
WINDOW_HEIGHT = 600