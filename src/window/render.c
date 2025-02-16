#include "render.h"

#include <SDL_error.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#include "../error.h"
#include "../game/game.h"
#include "colour/colour32.h"

void render_init(renderdata* const rdat, gamedata const* const gdat) {
    SDL_Window* const window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 100, 100, SDL_WINDOW_SHOWN);
    if (window == NULL)
        error(ERROR_SDL_RENDERING_INIT, "failed to create a window. SDL Error: %s", SDL_GetError());

    // rendere using vsync to limit updates to the refresh rate of the monitor
    SDL_Renderer* const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
        error(ERROR_SDL_RENDERING_INIT, "failed to create a renderer. SDL Error: %s", SDL_GetError());

    *rdat = (renderdata){
        window,
        renderer,
        gdat,
    };
}

void render_update(renderdata const* const rdat) {
    set_colour32(rdat->renderer, COLOUR32_BLACK);
    SDL_RenderClear(rdat->renderer);

    // present the renderer
    SDL_RenderPresent(rdat->renderer);
}

void render_free(renderdata* const rdat) {
    SDL_DestroyRenderer(rdat->renderer);
    SDL_DestroyWindow(rdat->window);
    *rdat = (renderdata){0};
}
