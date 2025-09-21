CC      = clang
CFLAGS  = -Wall -Wextra -Werror -g -std=gnu17 -O2 -pipe
LDFLAGS = -lm
RM      ?= rm -rf

NAME    = eiku
VERSION = 0.1.0
PREFIX  ?= /usr/local
DESTDIR ?=

# Folders
SRC_DIR      = src
TARGET       = x86_64-linux-gnu
ORIGIN_DIR   = target
TARGET_DIR   = $(ORIGIN_DIR)/$(TARGET)/$(NAME)
OBJ_DIR      = $(TARGET_DIR)/obj
DEP_DIR      = $(TARGET_DIR)/dep
BIN_DIR      = $(TARGET_DIR)/bin
SCRIPT_DIR   = scripts
INC_DIR      = include

# Source files
SRC := $(shell find $(SRC_DIR) -type f -name '*.c')
INC := $(shell find $(INC_DIR) -type f -name '*.h' -not -name '*.int.h')
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
DEP = $(patsubst $(SRC_DIR)/%.c,$(DEP_DIR)/%.d,$(SRC))

SHARED      = $(abspath $(BIN_DIR)/$(NAME).so)
SHARED_VER  = $(abspath $(BIN_DIR)/$(NAME).so.$(VERSION))
STATIC      = $(abspath $(BIN_DIR)/lib$(NAME).a)
INCLUDE_OUT = $(abspath $(BIN_DIR)/include)

INCLUDE        = -I$(INC_DIR)
INCLUDED_FILES = $(patsubst $(INC_DIR)/%.h,$(INCLUDE_OUT)/%.h,$(INC))

DIRS = $(OBJ_DIR) $(DEP_DIR) $(BIN_DIR) $(INCLUDE_OUT)

.PHONY: all clean fclean re install uninstall dirs

all: dirs $(SHARED) $(STATIC)

-include $(DEP)

install: all
# Install system wide
	sudo install -D -m 755 $(SHARED) $(DESTDIR)$(PREFIX)/lib/$(NAME).so.$(VERSION)
	sudo install -l $(DESTDIR)$(PREFIX)/lib/$(NAME).so.$(VERSION) $(DESTDIR)$(PREFIX)/lib/$(NAME).so
	sudo install -D -m 644 $(STATIC) $(DESTDIR)$(PREFIX)/lib/lib$(NAME).a.$(VERSION)
	sudo install -l $(DESTDIR)$(PREFIX)/lib/lib$(NAME).a.$(VERSION) $(DESTDIR)$(PREFIX)/lib/lib$(NAME).a
	sudo mkdir -p $(DESTDIR)$(PREFIX)/include/$(NAME)
	sudo install -m 644 $(INC_DIR)/*.h $(DESTDIR)$(PREFIX)/include/$(NAME)/
	sudo install -m 644 $(SRC_DIR)/*.h $(DESTDIR)$(PREFIX)/include/$(NAME)/
	sudo ldconfig

uninstall:
# Uninstall system wide
	sudo rm -f $(DESTDIR)$(PREFIX)/lib/$(NAME).so
	sudo rm -f $(DESTDIR)$(PREFIX)/lib/$(NAME).so.$(VERSION)
	sudo rm -f $(DESTDIR)$(PREFIX)/lib/lib$(NAME).a
	sudo rm -f $(DESTDIR)$(PREFIX)/lib/lib$(NAME).a.$(VERSION)
	sudo rm -rf $(DESTDIR)$(PREFIX)/include/$(NAME)
	sudo ldconfig

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D) $(dir $(DEP_DIR)/$*.d)
	$(CC) $(CFLAGS) -fPIC -MMD -MP -MF $(DEP_DIR)/$*.d -c $< -o $@ -I$(SRC_DIR)

# Rule to copy headers from include/ to BIN_DIR/include/
$(INCLUDE_OUT)/%.h: $(INC_DIR)/%.h
	@mkdir -p "$(@D)"
	cp $< "$@"

$(SHARED): $(OBJ)  $(INCLUDED_FILES)
	@mkdir -p "$(@D)"
	$(CC) -shared -Wl,-soname,"$(SHARED_VER)" -o "$@" $^ $(LDFLAGS)

$(STATIC): $(OBJ) $(INCLUDED_FILES)
	@mkdir -p "$(@D)"
	ar rcs "$@" $^

dirs:
	@$(foreach d, $(DIRS), mkdir -p "$(d)";)

clean:
	$(RM) -r $(OBJ_DIR) $(DEP_DIR)

fclean: clean
	$(RM) -r $(ORIGIN_DIR)

re: fclean all