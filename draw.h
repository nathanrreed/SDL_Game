#include <stdbool.h>

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_keycode.h>

#include "structs.h"

void prepareScene(void);
void presentScene(void);

void blit(SDL_Texture *texture, float x, float y, bool round_coords);

void blit_rect(SDL_Texture *texture, SDL_FRect* dest, SDL_FRect* src, bool round_coords);

float lerp(float start, float end, float time);

SDL_FPoint lerp2d(SDL_FPoint start, SDL_FPoint end, float time);

float diff_lerp(float a, float b, float t);

void render_text(char* text, SDL_FRect* dest, TextRenderFlags flags);

void draw_map();

void draw_grid();

void draw_grid_diagonal();

void draw_objects(Array* objs);

// void renderText(string text, SDL_Rect dest);


#pragma once
