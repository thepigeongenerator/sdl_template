#pragma once
#include <stdint.h>

// stores the data used in the game
typedef struct {
    void* val;
} GameData;

// updates the game's state
void game_update(GameData game_data, const uint8_t* keys);
