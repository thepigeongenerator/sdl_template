#include "game.h"

#include <SDL_keyboard.h>
#include <SDL_scancode.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../error.h"
#include "gametime.h"

void game_init(gamedata* dat) {
    *dat = (gamedata){
        gametime_new(),
    };
}

void game_update(gamedata* dat) {
    gametime_update(&dat->time);
    uint8_t const* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_ESCAPE])
        set_gamestatus(STATUS_SUCCESS);
}

void game_free(gamedata* dat) {
    (void)dat;
}
