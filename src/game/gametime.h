#pragma once

#include <time.h>

typedef struct {
    struct timespec ts; // stores the time at the current update
    float timescale;    // multiplier for the time calculation, default value is 1.0
    float deltatime;    // the time that it took between updates
} gametime;

// initializes the gametime struct
static inline gametime gametime_new(void) {
    return (gametime){
        {0},
        1.0F,
        0.0F,
    };
}

// updates the internal variables
static inline void gametime_update(gametime* gt) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    gt->deltatime = ((double)(ts.tv_nsec - gt->ts.tv_nsec) * 1e-9) * gt->timescale;
    gt->ts = ts;
}

// gets how many times the game updates per second
static inline float gametime_get_ups(gametime* gt) {
    return 1.0F / gt->deltatime;
}
