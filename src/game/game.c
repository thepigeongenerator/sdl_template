#include "game.h"

#include "gametime.h"

void game_init(game_data* dat) {
    *dat = (game_data){
        gametime_new(),
    };
}

void game_update(game_data* dat) {
    gametime_update(&dat->time);
}

void game_free(game_data* dat) {
    (void)dat;
}
