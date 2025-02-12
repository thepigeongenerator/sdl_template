#pragma once
#include <stdint.h>

// fits colours with each channel having a width of 2 bits
typedef union {
    uint32_t packed;
    struct {
        uint8_t a;
        uint8_t b;
        uint8_t g;
        uint8_t r;
    };
} colour32;
#define color32 colour32

#define COLOUR32_BLACK   ((colour32){0x000000FF})
#define COLOUR32_RED     ((colour32){0xFF0000FF})
#define COLOUR32_YELLOW  ((colour32){0xFFFF00FF})
#define COLOUR32_ORANGE  ((colour32){0xFF6D00FF})
#define COLOUR32_GREEN   ((colour32){0x00FF00FF})
#define COLOUR32_CYAN    ((colour32){0x00FFFFFF})
#define COLOUR32_BLUE    ((colour32){0x0000FFFF})
#define COLOUR32_MAGENTA ((colour32){0xFF00FFFF})
#define COLOUR32_WHITE   ((colour32){0xFFFFFFFF})
