NAME := sdl_template

# compiler settings
CC := clang
STD := c17
LANG = c
CFLAGS := $(shell pkg-config --cflags sdl2) -Wall -Wall -Wextra -Wpedantic -Wno-pointer-arith
LDFLAGS := $(shell pkg-config --libs sdl2) -lm

ifeq ($(DEBUG),1)
CFLAGS += -Og -g
else
REL_FLAGS += -O3
endif

# dirs
DIR_BIN := bin
DIR_OBJ := obj
DIR := $(DIR_BIN)/$(ARCH) $(DIR_OBJ)/$(ARCH)

# source files
SRC := $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c) $(wildcard src/**/**/**/**/*.c)
SRC_ASSETS := $(wildcard assets/*)

# output locations
OBJ := $(patsubst src/%,$(DIR_OBJ)/$(ARCH)/%,$(SRC:.c=.o))
DEP := $(OBJ:.o=.d)
ASSETS := $(patsubst assets/%,$(DIR_BIN)/$(ARCH)/%,$(SRC_ASSETS))
TARGET := $(DIR_BIN)/$(ARCH)/$(NAME)$(EXT)


# sets the variables for the different targets
linux-x86_64:
	$(MAKE) build ARCH=linux-x86_64 CFLAGS="$(CFLAGS) -target x86_64-pc-linux-gnu"
win-x86_64:
	$(MAKE) build ARCH=win-x86-64 CFLAGS="$(CFLAGS) -target x86_64-pc-windows-gnu" EXT=".exe"
web:
	$(MAKE) build ARCH=web CC=emcc EXT=".html"

all: linux-x86_64 win-x86_64 web
build: $(DIR) $(TARGET) $(ASSETS) compile_commands.json
clean:
	rm -rf $(DIR_BIN) $(DIR_OBJ)

# create the binary
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $^ $(CFLAGS) $(LDFLAGS)

# create .o and .d files
$(DIR_OBJ)/$(ARCH)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ -MD -MP -c $< $(CFLAGS) -std=$(STD) -x $(LANG) -Wno-unused-command-line-argument

# copy assets
$(DIR_BIN)/$(ARCH)/%: assets/%
	@mkdir -p $(dir $@)
	cp $< $@

# create directories
$(DIR):
	@mkdir -p $@

# update compile commands if the makefile has been updated (for linting)
ifeq ($(NO_CMDS),1)
compile_commands.json: makefile
else
compile_commands.json: makefile
	@touch compile_commands.json
	$(MAKE) clean
	bear -- make
endif

# include the dependencies
-include $(DEP)
