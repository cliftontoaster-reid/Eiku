CC = clang
CFLAGS = -Wall -Wextra -Werror -std=gnu17 -pipe
LDFLAGS = -lX11 -lXtst -lXext -lm

TARGET = fakemouse_demo
SOURCE = main.c

MODE ?= debug
ifeq ($(MODE),release)
	CFLAGS += -O3 -g0 -D_FORTIFY_SOURCE=2
else ifeq ($(MODE),debug)
	CFLAGS += -O0 -g3 -fstack-protector-strong -fsanitize=address,undefined -fno-omit-frame-pointer
	LDFLAGS += -fsanitize=address,undefined
else
	$(error "Invalid MODE specified: $(MODE). Use 'debug' or 'release'.")
endif

# Use zig cc if available, otherwise fallback to clang or gcc
ifneq (, $(shell which zig))
	CC = zig cc
else ifneq (, $(shell which clang))
	CC = clang
else
	CC = gcc
endif

.PHONY: all clean fclean re run

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGET)

fclean: clean

re: fclean all

run: $(TARGET)
	@echo "=== Running Fake Mouse Events Demo ==="
	@echo "WARNING: This will move your mouse cursor and generate clicks!"
	@echo "Make sure you're ready for mouse movement on your screen."
	@echo "The demo will start in a few seconds..."
	@echo
	./$(TARGET)