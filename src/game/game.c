#include "game.h"

#include <SDL_scancode.h>
#include <stdint.h>

#include "../main.h"


// called every time the game's state is updated
void game_update(GameData game_data, const uint8_t* keys) {
    (void)game_data;
    if (keys[SDL_SCANCODE_ESCAPE]) {
        stop();
    }
}
