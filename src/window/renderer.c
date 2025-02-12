#include "renderer.h"

#include <SDL_error.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <stdio.h>

#include "../errors.h"
#include "../game/game.h"


// initializes the window and renderer
void renderer_init(render_data* const render_dat, game_data const* const game_dat) {
    // create a new window
    SDL_Window* const window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 100, 100, SDL_WINDOW_SHOWN);
    if (window == NULL)
        error(ERROR_SDL_RENDERER_INIT, "Window failed to be created! SDL Error: %s", SDL_GetError());

    // create a renderer
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL)
        error(ERROR_SDL_RENDERER_INIT, "Renderer failed to be created! SDL Error: %s", SDL_GetError());

    *render_dat = (render_data){
        window,
        renderer,
        game_dat,
    };
}

void renderer_update(const render_data* render_data) {
    SDL_Renderer* renderer = render_data->renderer;

    int success = 0; // if an error occurs, this value is <0

    // clear render
    success |= SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x50);
    success |= SDL_RenderClear(renderer);

    if (success < 0) {
        warn("\033[93mW\033[0m: something went wrong whilst renderering! SDL Error: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderPresent(renderer);
}
