#pragma once

#include <stdint.h>
#include "gametime.h"

typedef struct {
    gametime time;
} game_data;

void game_init(game_data*);   // initializes everything needed to start the game; outputs to game_data
void game_update(game_data*); // causes an update to occur within the game
void game_free(game_data*);   // frees the resources associated with the game
