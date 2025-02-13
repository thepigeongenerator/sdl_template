#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <stdint.h>
#include <stdlib.h>

#include "error.h"
#include "game/game.h"
#include "window/render.h"

static game_data gdat;   // initialized in init(), reading beforehand is undefined behaviour
static render_data rdat; // initialized in init(), reading beforehand is undefined behaviour

// initialize the game
static void init(void) {
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
        error(ERROR_SDL_INIT, "SDL could not initialize! SDL Error: %s", SDL_GetError());

    // initialize other game components
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
                exit(STATUS_SUCCESS);
                break;
            }
        }
    }

    // perform updates
    game_update(&gdat);
    render_update(&rdat);
}

// entry-point of the application
int32_t main(int32_t argc, char** argv) {
    (void)argc, (void)argv;

    init();

    while (get_gamestatus() == STATUS_RUNNING)
        update();

    return get_gamestatus();
}
