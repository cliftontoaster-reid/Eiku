# Makefile for colourscreen_fast example

# Compiler settings
CC = clang
CFLAGS = -Wall -Wextra -Werror -g -std=gnu17 -O2
LDFLAGS = -lm -lX11 -lXext

# Paths
EIKU_LIB_DIR = ../../target/x86_64-linux-gnu/eiku/bin
EIKU_INCLUDE_DIR = ../../include
EIKU_BIN_INCLUDE_DIR = ../../target/x86_64-linux-gnu/eiku/bin/include

# Target
TARGET = colourscreen_fast
SOURCE = main.c

# Default target
all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -I$(EIKU_INCLUDE_DIR) -I$(EIKU_BIN_INCLUDE_DIR) \
		-L$(EIKU_LIB_DIR) -o $(TARGET) $(SOURCE) \
		$(EIKU_LIB_DIR)/libeiku.a $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run