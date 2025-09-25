# Entity Demo Example Makefile

CC      = clang
CFLAGS  = -Wall -Wextra -Werror -std=gnu17 -g3 -fsanitize=address,undefined
LDFLAGS = -lm -L../../target/x86_64-linux-gnu/eiku/bin -leiku -fsanitize=address,undefined
TARGET  = entity_demo
SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)
INCLUDE = -I../../target/x86_64-linux-gnu/eiku/bin/include

# Default target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	LD_LIBRARY_PATH=../../target/x86_64-linux-gnu/eiku/bin ./$(TARGET)

.PHONY: all clean run