#pragma once

#include <stdint.h>

#include "gametime.h"

typedef struct {
    gametime time;
} gamedata;

void game_init(gamedata*);   // initializes everything needed to start the game; outputs to game_data
void game_update(gamedata*); // causes an update to occur within the game
void game_free(gamedata*);   // frees the resources associated with the game
