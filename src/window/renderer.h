#pragma once
#include <SDL_render.h>
#include <SDL_video.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} RenderData;

int renderer_init(SDL_Window** window, SDL_Renderer** renderer);
void renderer_update(const RenderData* render_data);
