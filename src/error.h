#pragma once

#include <stdint.h>
#include <stdnoreturn.h>

#include "util/attributes.h"

/* defines statuses in the 0..127, any higher/negative values are POSIX-reserved.
 * The max value (or -1) shall mean the application is running, anything else shall mean an exit code of some kind */
enum {
	// clang-format off
    STATUS_SUCCESS = 0,                // 0; successful exit
    STATUS_ERROR   = 1,                // miscellaneous error
    ERROR_INIT     = STATUS_ERROR | 2, // initialisation error

    ERROR_STD             = STATUS_ERROR   | 64, // standard library error
    ERROR_STD_INIT        = ERROR_INIT     | 64, // standard library initialisation error
    ERROR_STD_MEMORY      = ERROR_STD      | 32, // memory error
    ERROR_STD_MEMORY_INIT = ERROR_STD_INIT | 32, // memory initialization error

    ERROR_SDL                = STATUS_ERROR   | 32, // SDL error
    ERROR_SDL_INIT           = ERROR_INIT     | 32, // SDL initialization error
    ERROR_SDL_RENDERING      = ERROR_SDL      | 16, // rendering error
    ERROR_SDL_RENDERING_INIT = ERROR_SDL_INIT | 16, // rendering initialization error
    ERROR_SDL_AUDIO          = ERROR_SDL      | 8,  // audio error
    ERROR_SDL_AUDIO_INIT     = ERROR_SDL_INIT | 8,  // audio initialization error

    STATUS_RUNNING = -1,
	// clang-format on
};
typedef int8_t gamestatus;

atrb_nonnull(1) atrb_format(printf, 1, 2) void debug(char const*, ...); // prints a debug message to stdout if the DEBUG environment variable is set, otherwise the call is ignored.
atrb_nonnull(1) atrb_format(printf, 1, 2) void info(char const*, ...);  // prints an info message to stdout
atrb_nonnull(1) atrb_format(printf, 1, 2) void warn(char const*, ...);  // prints a warning message to stderr
atrb_nonnull(1) atrb_format(printf, 1, 2) void error(char const*, ...); // prints an warning message to stderr

// prints an error message to stderr before exiting
atrb_nonnull(2, 4) atrb_format(printf, 4, 5) noreturn void fatal(gamestatus, char const* file_name, uint32_t line, char const* fmt, ...);
