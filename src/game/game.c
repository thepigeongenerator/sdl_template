#include "game.h"

#include <SDL_scancode.h>
#include <stdint.h>

#include "../main.h"


void game_init(game_data* const dat) {
    *dat = (game_data){0};
}

// called every time the game's state is updated
void game_update(game_data* const dat, uint8_t const* const keys) {
    (void)dat;
    if (keys[SDL_SCANCODE_ESCAPE]) stop();
}
