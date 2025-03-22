#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <stdint.h>

#include "error.h"
#include "game/game.h"
#include "game/gametime.h"
#include "window/render.h"

// initialized in init(), reading beforehand is undefined behaviour
static gametime gt;
static gamedata gdat;
static renderdata rdat;

// initialize the game
static void init(void) {
	// initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		fatal(ERROR_SDL_INIT, __FILE_NAME__, __LINE__, "SDL could not initialize! SDL Error: %s", SDL_GetError());

	// initialize other game components
	gt = gametime_new();
	game_init(&gdat);
	render_init(&rdat, &gdat);
}

// perform the updates to the game
static void update(void) {
	// update the input
	{
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				gdat->run = false;
				break;
			}
		}
	}

	// perform updates
	gametime_update(&gt);
	game_update(&gdat);
	render_update(&rdat);
}

// entry-point of the application
int32_t main(int32_t argc, char** argv) {
	(void)argc, (void)argv;

	init();
	debug("successfully initialized!");

	while (gdat->run == true)
		update();

	debug("done! starting to free resources...");
	game_free(&gdat);
	render_free(&rdat);
	SDL_Quit();

	gamestatus exit_code = get_gamestatus();
	debug("quitting with an exit code of %u", exit_code);
	return exit_code;
}
