#pragma once

#include <SDL_render.h>
#include <SDL_video.h>

#include "../game/game.h"

// contains the data necessary for rendering
typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
	gamedata const* gdat;
} renderdata;

void render_init(renderdata*, gamedata const*); // initializes the renderer, outputs to render_data
void render_update(renderdata const*);          // causes a draw to occur, will also determine update rate
void render_free(renderdata*);                  // frees the memory allocated to the renderer in render_data
