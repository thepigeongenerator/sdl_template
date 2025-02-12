#include "main.h"

#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "game/game.h"
#include "window/renderer.h"

bool playing = true;

render_data render_dat = {0};
game_data game_dat = {0};

// handles game application initialisation
static void init(void) {
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        error(ERROR_SDL_INIT, "SDL could not initialize! SDL Error: %s", SDL_GetError());
        return;
    }

    renderer_init(&render_dat, &game_dat);

    // initialize audio
    // AudioDevice* audio_device = audio_device_init(32000, AUDIO_S16, 1, 255);

    // AudioData audio1 = audio_load_wav(audio_device, "FILE NAME");
    game_init(&game_dat);
}

// handles game application updating
static void update(void) {
    // update the input
    {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                exit(SUCCESS);
                break;
            }
        }
    }

    // perform updates
    game_update(&game_dat, SDL_GetKeyboardState(NULL));
    renderer_update(&render_dat);
}

// handles game application quitting
void stop(void) {
    playing = false;
}

// entry point of the application
int32_t main(int32_t argc, char** argv) {
    (void)argc;
    (void)argv;
    init();

    while (playing)
        update();

    // cleanup of resources
    SDL_Quit();

    return 0;
}
