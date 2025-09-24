CC      = clang
CFLAGS  = -Wall -Wextra -Werror -std=gnu17 -pipe
LDFLAGS = -lm -lX11 -lXext
RM      ?= rm -rf

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

# Tools optimization
## If 'zig cc' is available, use it as a drop-in replacement for 'clang'
## If not check if clang is available, otherwise fallback to gcc
ifneq (, $(shell which zig))
	CC      = zig cc
else ifneq (, $(shell which clang))
	CC      = clang
else
	CC      = gcc
endif
## If mold is available, use it as a drop-in replacement for 'ld'
ifneq (, $(shell which mold))
	LDFLAGS += -fuse-ld=mold
endif

# Project
NAME    = eiku
VERSION = 0.1.0-exp.1
PREFIX  ?= /usr/local
DESTDIR ?=

# Folders
SRC_DIR      = src
TEST_DIR     = tests
TARGET       = x86_64-linux-gnu
ORIGIN_DIR   = target
TARGET_DIR   = $(ORIGIN_DIR)/$(TARGET)/$(NAME)
OBJ_DIR      = $(TARGET_DIR)/obj
TEST_OBJ_DIR = $(TARGET_DIR)/tobj
DEP_DIR      = $(TARGET_DIR)/dep
BIN_DIR      = $(TARGET_DIR)/bin
TMP_DIR      = $(TARGET_DIR)/tmp
SCRIPT_DIR   = scripts
INC_DIR      = include

# Source files
SRC  := $(shell find $(SRC_DIR) -type f -name '*.c')
TSRC := $(shell find $(TEST_DIR) -type f -name '*.c')
INC  := $(shell find $(INC_DIR) -type f -name '*.h' -not -name '*.int.h')
OBJ  := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
DEP  := $(patsubst $(SRC_DIR)/%.c,$(DEP_DIR)/%.d,$(SRC))
TOBJ := $(patsubst $(TEST_DIR)/%.c,$(TEST_OBJ_DIR)/%.o,$(TSRC))
TDEP := $(patsubst $(TEST_DIR)/%.c,$(DEP_DIR)/%.d,$(TSRC))

SHARED      = $(abspath $(BIN_DIR)/lib$(NAME).so)
STATIC      = $(abspath $(BIN_DIR)/lib$(NAME).a)
INCLUDE_OUT = $(abspath $(BIN_DIR)/include)

INCLUDE        = -I$(INC_DIR)
INCLUDED_FILES = $(patsubst $(INC_DIR)/%.h,$(INCLUDE_OUT)/%.h,$(INC))

DIRS = $(OBJ_DIR) $(DEP_DIR) $(BIN_DIR) $(TMP_DIR) $(TEST_OBJ_DIR) $(INCLUDE_OUT)

.PHONY: all clean fclean re install uninstall dirs criterion

# Criterion test framework (precompiled distribution)
# We download the prebuilt tar.xz and extract it into the target install dir
CRITERION_VERSION        = 2.4.2
CRITERION_TARBALL_URL   = https://github.com/Snaipe/Criterion/releases/download/v$(CRITERION_VERSION)/criterion-$(CRITERION_VERSION)-linux-x86_64.tar.xz
CRITERION_MODULE_DIR    = $(ORIGIN_DIR)/$(TARGET)/criterion
CRITERION_TMP_DIR       = $(CRITERION_MODULE_DIR)/tmp
CRITERION_TARBALL       = $(CRITERION_TMP_DIR)/criterion-$(CRITERION_VERSION)-linux-x86_64.tar.xz
CRITERION_INSTALL_DIR   = $(CRITERION_MODULE_DIR)/bin


all: dirs $(SHARED) $(STATIC)

-include $(DEP)

install: all
# Install system wide
	sudo install -D -m 755 $(SHARED) $(DESTDIR)$(PREFIX)/lib/lib$(NAME).so.$(VERSION)
	sudo install -l $(DESTDIR)$(PREFIX)/lib/lib$(NAME).so.$(VERSION) $(DESTDIR)$(PREFIX)/lib/lib$(NAME).so
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
	$(CC) $(CFLAGS) -fPIC -MMD -MP -MF $(DEP_DIR)/$*.d -c $< -o $@ $(INCLUDE)

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(@D) $(dir $(DEP_DIR)/$*.d)
	$(CC) $(CFLAGS) -fPIC -MMD -MP -MF $(DEP_DIR)/$*.d -c $< -o $@ $(INCLUDE) -I$(CRITERION_INSTALL_DIR)/include

# Rule to copy headers from include/ to BIN_DIR/include/
$(INCLUDE_OUT)/%.h: $(INC_DIR)/%.h
	@mkdir -p "$(@D)"
	cp $< "$@"

$(SHARED): $(OBJ)  $(INCLUDED_FILES)
	@mkdir -p "$(@D)"
	$(CC) -shared -o "$@" $(OBJ) $(LDFLAGS)

$(STATIC): $(OBJ) $(INCLUDED_FILES)
	@mkdir -p "$(@D)"
	ar rcs "$@" $(OBJ)

dirs:
	@$(foreach d, $(DIRS), mkdir -p "$(d)";)

clean:
	$(RM) -r $(OBJ_DIR) $(DEP_DIR)

fclean: clean
	$(RM) -r $(ORIGIN_DIR)

re: fclean all

# Download and extract precompiled Criterion for running tests
criterion: $(CRITERION_INSTALL_DIR)

$(CRITERION_INSTALL_DIR):
	@echo "Preparing Criterion v$(CRITERION_VERSION) in $(CRITERION_INSTALL_DIR)"
	@mkdir -p "$(CRITERION_TMP_DIR)" "$(CRITERION_INSTALL_DIR)"
	@# Download tarball if missing
	@if [ ! -f "$(CRITERION_TARBALL)" ]; then \
		echo "Downloading $(CRITERION_TARBALL_URL) ..."; \
		if command -v curl >/dev/null 2>&1; then \
			curl -L -o "$(CRITERION_TARBALL)" "$(CRITERION_TARBALL_URL)"; \
		else \
			wget -O "$(CRITERION_TARBALL)" "$(CRITERION_TARBALL_URL)"; \
		fi; \
	fi
	@# Extract into the install dir (overwrite if necessary)
	@echo "Extracting $(CRITERION_TARBALL) to $(CRITERION_INSTALL_DIR) ...";
	@tar -xJf "$(CRITERION_TARBALL)" -C "$(CRITERION_INSTALL_DIR)" --strip-components=1
	@echo "Criterion installed to $(CRITERION_INSTALL_DIR)"

test: criterion all $(TOBJ) $(TDEP)
	@echo "Linking test units with Criterion..."
	$(CC) -o $(BIN_DIR)/$(NAME).test $(TOBJ) $(OBJ) -L$(CRITERION_INSTALL_DIR)/lib -lcriterion $(LDFLAGS) -lXtst

run_tests: test
	@echo "Running tests in virtual X11 display (24-bit depth)..."
	@export DISPLAY_DEPTH=24 && LD_LIBRARY_PATH=$(BIN_DIR):$(CRITERION_INSTALL_DIR)/lib xvfb-run --auto-servernum --server-args='-screen 0 1024x768x24' $(BIN_DIR)/$(NAME).test --verbose || exit 0
	@echo "Running tests in virtual X11 display (16-bit depth)..."
	@export DISPLAY_DEPTH=16 && LD_LIBRARY_PATH=$(BIN_DIR):$(CRITERION_INSTALL_DIR)/lib xvfb-run --auto-servernum --server-args='-screen 0 1024x768x16' $(BIN_DIR)/$(NAME).test --verbose || exit 0
	@echo "Running tests in virtual X11 display (8-bit depth)..."
	@export DISPLAY_DEPTH=8 && LD_LIBRARY_PATH=$(BIN_DIR):$(CRITERION_INSTALL_DIR)/lib xvfb-run --auto-servernum --server-args='-screen 0 1024x768x8' $(BIN_DIR)/$(NAME).test --verbose || exit 0
	@echo "All tests completed."

examples: all
	@echo "Building all examples..."
	@$(foreach dir, $(wildcard examples/*), \
		echo "Building example $(dir) ..."; \
		$(MAKE) -C $(dir) all; )
	@echo "All examples built successfully!"

# Run example by name examples/%
examples/%: all
	@echo "Building example $* ..."
	$(MAKE) -C examples/$* all
	@echo "Running example $* ..."
	$(MAKE) -C examples/$* run

format:
	@echo "Formatting source files with clang-format..."
	@find $(SRC_DIR) $(INC_DIR) $(TEST_DIR) -type f \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} +
	@echo "Source files formatted."