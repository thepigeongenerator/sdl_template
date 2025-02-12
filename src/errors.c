#include "errors.h"

#include <SDL_messagebox.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void error(error_code const error_code, char const* const format, ...) {
    char buffer[ERROR_MAX_STR_LEN] = {0}; // contains the buffer of the final string

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, ERROR_MAX_STR_LEN, format, args);
    va_end(args);

    printf("\033[91mE\033[0m: %s\n", buffer);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "something went wrong! :O", buffer, NULL);

    exit(error_code);
}

void warn(char const* const format, ...) {
    char buffer[ERROR_MAX_STR_LEN] = {0}; // contains the buffer of the final string

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, ERROR_MAX_STR_LEN, format, args);
    va_end(args);

    printf("\033[93mW\033[0m: %s\n", buffer);
}
