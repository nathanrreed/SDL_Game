#include "types.h"
#include "structs.h"

bool draw_animation_base(Animation* animation, SDL_FRect* dest, bool round_coords);

bool draw_animation(u32 pos, SDL_FRect* dest, bool round_coords);

Animation create_animation(const string file, u32 frames, float duration, SDL_FPoint t_size, bool loop);

void destroy_animation(Animation* animation);

void start_lerp_animation(Animation* animation, SDL_FPoint start, SDL_FPoint end, float speed, u32 z_index);

void start_lerp_texture(SDL_Texture** texture, SDL_FPoint start, SDL_FPoint end, float speed, u32 z_index);

bool draw_lerp(u32 pos);

float get_distance(SDL_FPoint start, SDL_FPoint end);

Array* a_star(SDL_FPoint start, SDL_FPoint goal);

int compareObjects(const void * a, const void * b);

Animation start_animation(Animation* animation, u32 flags, u32 z_index, bool insert_anim);

#pragma once