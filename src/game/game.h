#pragma once

#include <math.h>

#include "gametime.h"

// constants for pi(π) and tau(τ)
#define PI   (M_PI)         // π constant
#define TAU  (M_PI * 2.0)   // τ constant
#define PIf  (M_PIf)        //
#define TAUf (M_PIf * 2.0F) //

typedef struct {
    gametime time;
} gamedata;

void game_init(gamedata*);   // initializes everything needed to start the game; outputs to game_data
void game_update(gamedata*); // causes an update to occur within the game
void game_free(gamedata*);   // frees the resources associated with the game
