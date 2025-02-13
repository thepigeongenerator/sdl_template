#include "game.h"

#include "gametime.h"

void game_init(gamedata* dat) {
    *dat = (gamedata){
        gametime_new(),
    };
}

void game_update(gamedata* dat) {
    gametime_update(&dat->time);
}

void game_free(gamedata* dat) {
    (void)dat;
}
