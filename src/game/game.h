#pragma once
#include <stdint.h>

// stores the data used in the game
typedef struct {
    uint8_t tmp;
} game_data;

// updates the game's state
void game_init(game_data* game_dat);
void game_update(game_data* game_dat, const uint8_t* keys);
