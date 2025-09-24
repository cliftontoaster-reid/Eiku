# Makefile for colourscreen_fast example

# Compiler settings
CC = clang
CFLAGS = -Wall -Wextra -Werror -g -std=gnu17 -O2
LDFLAGS = -lm -lX11 -lXext

MODE    ?= debug
ifeq ($(MODE),release)
	CFLAGS  += -O3 -g0 -D_FORTIFY_SOURCE=2
	LDFLAGS += -Wl,-z,relro -Wl,-z,now
else ifeq ($(MODE),debug)
	CFLAGS  += -O0 -g3 -fstack-protector-strong -fstack-clash-protection -Warray-bounds -Wformat-security -fsanitize=address,undefined -fno-omit-frame-pointer
	LDFLAGS += -fsanitize=address,undefined
else
	$(error "Invalid MODE specified: $(MODE). Use 'debug' or 'release'.")
endif

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