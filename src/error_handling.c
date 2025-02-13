#include "error_handling.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "SDL_messagebox.h"

#define PRINT_BUFFER_SIZE 128 // defines the buffer size for printing

// writes the arguments to the specified buffer
#define write_args(buf, fmt)                            \
    va_list args;                                       \
    va_start(args, fmt);                                \
    (void)vsnprintf(buf, PRINT_BUFFER_SIZE, fmt, args); \
    va_end(args);

static gamestatus status = STATUS_RUNNING;

gamestatus get_gamestatus(void) {
    return status;
}

void debug(char const* fmt, ...) {
    char const* env = getenv("DEBUG");
    if (env == NULL || *env != '1')
        return;

    char buf[PRINT_BUFFER_SIZE] = {0};
    write_args(buf, fmt);

    (void)fprintf(stdout, "\033[95m%s\033[0m\n", buf);
}

void info(char const* fmt, ...) {
    char buf[PRINT_BUFFER_SIZE] = {0};
    write_args(buf, fmt);
    (void)fprintf(stdout, "\033[0m%s\033[0m\n", buf); // write colour here for consistency
}

void warn(char const* fmt, ...) {
    char buf[PRINT_BUFFER_SIZE] = {0};
    write_args(buf, fmt);
    (void)fprintf(stderr, "\033[93mW: %s\033[0m\n", buf);
}

void error(gamestatus error_code, char const* fmt, ...) {
    char buf[PRINT_BUFFER_SIZE] = {0};
    write_args(buf, fmt);
    (void)fprintf(stderr, "\033[91mE: %s\033[0m\n", buf);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "something went wrong! :O", buf, NULL);
    status = error_code;
}
