#include "renderer.h"

#include <SDL_error.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <stdio.h>

#include "../errors.h"
#include "../game/game.h"
#include "colour/colour32.h"
#include "colour/colour8.h"


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

// sets the colour32 from the colour8
static inline void set_colour32(SDL_Renderer* const renderer, colour32 const c) {
    (void)SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

// sets the colour32 from the colour8
//static inline void set_colour8(SDL_Renderer* const renderer, colour8 const c) {
//    (void)SDL_SetRenderDrawColor(renderer, colour8_red32(c), colour8_green32(c), colour8_blue32(c), 0xFF);
//}

void renderer_update(const render_data* render_data) {
    SDL_Renderer* renderer = render_data->renderer;

    int success = 0; // if an error occurs, this value is <0

    // clear render
    set_colour32(renderer, COLOUR32_BLACK);
    success |= SDL_RenderClear(renderer);

    if (success < 0) {
        warn("\033[93mW\033[0m: something went wrong whilst renderering! SDL Error: %s\n", SDL_GetError());
        return;
    }

    SDL_RenderPresent(renderer);
}
