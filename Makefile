CC      = clang
CFLAGS  = -Wall -Wextra -Werror -g -std=gnu17 -O2 -pipe
LDFLAGS = -lm -lX11 -lXext -g -O2
RM      ?= rm -rf

NAME    = eiku
VERSION = 0.1.0
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
	$(CC) -o $(BIN_DIR)/$(NAME).test $(TOBJ) -L$(BIN_DIR) -l$(NAME) -L$(CRITERION_INSTALL_DIR)/lib -lcriterion -lm -lpthread -ldl -I$(CRITERION_INSTALL_DIR)/include

run_tests: test
	@echo "Running tests..."
	@LD_LIBRARY_PATH=$(BIN_DIR):$(CRITERION_INSTALL_DIR)/lib $(BIN_DIR)/$(NAME).test