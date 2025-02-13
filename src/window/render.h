#pragma once

#include <SDL_render.h>
#include <SDL_video.h>

#include "../game/game.h"

// contains the data necessary for rendering
typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    game_data const* gdat;
} render_data;

void render_init(render_data*, game_data const*); // initializes the renderer, outputs to render_data
void render_update(render_data const*);           // causes a draw to occur, will also determine update rate
void render_free(render_data*);                   // frees the memory allocated to the renderer in render_data
