#pragma once
#include <SDL_render.h>
#include <SDL_video.h>

#include "../game/game.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    game_data const* game_dat;
} render_data;

void renderer_init(render_data* render_dat, game_data const* game_dat);
void renderer_update(render_data const* render_data);
