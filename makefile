NAME := $(shell basename $(PWD))

# compiler settings
CC := clang
STD := c17
LANG = c
CFLAGS := $(shell pkg-config --cflags sdl2) -Wall -Wextra -Wpedantic -Wno-pointer-arith
LDFLAGS := $(shell pkg-config --libs sdl2) -lm

ifeq ($(DEBUG),1)
CFLAGS += -DDEBUG -fsanitize=address,undefined -g -Og
PROF := dbg
else
REL_FLAGS += -O3
PROF := rel
endif

# dirs
DIR_BIN := bin
DIR_OBJ := obj
DIR_BUILD := $(DIR_BIN)/$(ARCH)/$(PROF)
DIR := $(DIR_BIN)/$(ARCH)/$(PROF) $(DIR_OBJ)/$(ARCH)/$(PROF)

# source files
SRC := $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c) $(wildcard src/**/**/**/**/*.c)
SRC_ASSETS := $(wildcard assets/*)

# output locations
OBJ := $(patsubst src/%,$(DIR_OBJ)/$(ARCH)/$(PROF)/%,$(SRC:.c=.o))
DEP := $(OBJ:.o=.d)
ASSETS := $(patsubst assets/%,$(DIR_BUILD)/%,$(SRC_ASSETS))
TARGET := $(DIR_BUILD)/$(NAME)$(EXT)

define wr_colour
	@echo -e "\033[$(2)m$(1)\033[0m"
endef

# sets the variables for the different targets
linux-x86_64:
	@$(MAKE) _build ARCH=$@ CFLAGS="$(CFLAGS) -target x86_64-pc-linux-gnu"
win-x86_64:
	@$(MAKE) _build ARCH=$@ CFLAGS="$(CFLAGS) -target x86_64-pc-windows-gnu" EXT=".exe"
web:
	@$(MAKE) _build ARCH=$@ CC=emcc EXT=".html"

all: linux-x86_64 win-x86_64 web
_build: compile_commands.json $(DIR) $(TARGET) $(ASSETS)
	@$(call wr_colour,"current profile: '$(PROF)'",93)
clean:
	rm -rf $(DIR_BIN) $(DIR_OBJ) compile_commands.json

# create the binary (linking step)
$(TARGET): $(DIR_OBJ) $(OBJ)
	@$(call wr_colour,"CC: '$(CC)'",94)
	@$(call wr_colour,"CFLAGS: '$(CFLAGS)'",94)
	@$(call wr_colour,"LDFLAGS: '$(LDFLAGS)'",94)
	@$(CC) -o $(TARGET) $^ $(CFLAGS) $(LDFLAGS)

# create .o and .d files
$(DIR_OBJ)/$(ARCH)/$(PROF)/%.o: src/%.c
	@$(call wr_colour,"compiling $(notdir $@) from $(notdir $<)",92)
	@mkdir -p $(dir $@)
	@$(CC) -o $@ -MD -MP -c $< $(CFLAGS) -std=$(STD) -x $(LANG) -Wno-unused-command-line-argument

# copy assets
$(DIR_BUILD)/%: assets/%
	@mkdir -p $(dir $@)
	cp -v $< $@

# create directories
$(DIR):
	mkdir -p $@

# update compile commands if the makefile has been updated (for linting)
ifeq ($(DEBUG),1)
compile_commands.json: makefile
	$(MAKE) clean
	@touch compile_commands.json
	bear -- make $(ARCH)
else
compile_commands.json: makefile
	$(MAKE) clean
	@touch compile_commands.json
endif

# include the dependencies
-include $(DEP)
