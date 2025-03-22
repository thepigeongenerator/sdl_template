#include "game.h"

#include <SDL_keyboard.h>
#include <SDL_scancode.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "gametime.h"

void game_init(gamedata* dat) {
	// initialize the game time
	struct gametime gt = {{0}, 0};
	gametime_get(&gt.ts);

	// initialize the game data
	*dat = (gamedata){
		gt,
		true,
	};
}

static inline void update_gametime(gamedata* dat) {
	struct timespec ts;
	gametime_get(&ts);
	dat->time.delta = (ts.tv_sec - dat->time.ts.tv_sec) + (ts.tv_nsec - dat->time.ts.tv_nsec) * 1e-9;
	dat->time.ts = ts;
}

void game_update(gamedata* dat) {
	update_gametime(dat); // update the game time
	uint8_t const* keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_ESCAPE])
		dat->run = false;
}

void game_free(gamedata* dat) {
	*dat = (gamedata){0};
}
