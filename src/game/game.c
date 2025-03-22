#include "game.h"

#include <SDL_keyboard.h>
#include <SDL_scancode.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "gametime.h"

void game_init(gamedata* dat, gametime* gt) {
	*dat = (gamedata){
		gt,
		true,
	};
}

void game_update(gamedata* dat) {
	uint8_t const* keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_ESCAPE])
		dat->run = false;
}

void game_free(gamedata* dat) {
	*dat = (gamedata){0};
}
