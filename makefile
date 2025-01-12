NAME = sdl_template
CFLAGS = -Wall -g -march=x86-64 -lm

DIR_BIN = bin
DIR_OBJ = obj

SRC = $(wildcard src/*.c) $(wildcard src/**/*.c) $(wildcard src/**/**/*.c) $(wildcard src/**/**/**/*.c) $(wildcard src/**/**/**/**/*.c)
OBJ = $(patsubst src/%,$(DIR_OBJ)/$(ARCH)/%,$(SRC:.c=.o))

TARGET = $(DIR_BIN)/$(ARCH)/$(NAME)$(EXT)

clean:
	rm -rf $(DIR_BIN) $(DIR_OBJ)

# sets the variables for the different targets
linux-x86_64:
	$(MAKE) build ARCH=linux-x86_64 CC=gcc PCFLAGS="-lSDL2"
win-x86_64:
	$(MAKE) build ARCH=win-x86-64 CC=x86_64-w64-mingw32-gcc PCFLAGS="-lmingw32 -lSDL2main -lSDL2 -mwindows" EXT=".exe"
web:
	$(MAKE) build ARCH=web CC=emcc PCFLAGS="-s USE_SDL=2" EXT=".html"

build: dirs binary

# creates the binary
binary: $(OBJ)
	$(CC) -o $(TARGET) $^ $(CFLAGS) $(PCFLAGS)

# creates the object files
$(DIR_OBJ)/$(ARCH)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) -o $@ -c $< $(CFLAGS) $(PCFLAGS)

dirs:
	mkdir -p $(DIR_BIN)/$(ARCH)
	mkdir -p $(DIR_OBJ)/$(ARCH)
