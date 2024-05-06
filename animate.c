#include <SDL3_image/SDL_image.h>

#include <math.h>

#include "animate.h"
#include "draw.h"
#include "main.h"

bool draw_animation_base(Animation* animation, SDL_FRect* dest, bool round_coords) {
    float elapsed = (SDL_GetPerformanceCounter() - animation->start_time) / (float) SDL_GetPerformanceFrequency();

    if(elapsed > animation->duration) {
        if((animation->flags & LOOP) > 0) {
            animation->start_time = SDL_GetPerformanceCounter();
            elapsed = 0;
        } else {
            return true;
        }
    }

    SDL_FRect current = {animation->t_size.x, 0, animation->t_size.x, animation->t_size.y};
    dest->w = current.w;
    dest->h = current.h;

    int current_frame = elapsed / animation->duration * animation->frames;
    current.x *= current_frame;
    current.y = 0;
    
    blit_rect(animation->texture, dest, &current, round_coords);

    return false;
}

bool draw_animation(u32 pos, SDL_FRect* dest, bool round_coords) {
    Animation* animation = (Animation *) get(objects, pos);
    return draw_animation_base(animation, dest, round_coords);
}

Animation create_animation(const string file, u32 frames, float duration, SDL_FPoint t_size, bool loop) {
    return (Animation) {OBJECT_ANIMATION, 0, frames, duration, 0, IMG_LoadTexture(app.renderer, file), t_size, loop};
}

void destroy_animation(Animation *animation) {
    SDL_DestroyTexture(animation->texture);
}

int compareObjects(const void * a, const void * b) {
    if (((Object*)a)->obj.z_index < ((Object*)b)->obj.z_index) return -1;
    if (((Object*)a)->obj.z_index > ((Object*)b)->obj.z_index) return  1;
    return  0;
}

Animation start_animation(Animation* animation, u32 flags, u32 z_index, bool insert_anim) {
    Animation new = *animation;
    new.start_time = SDL_GetPerformanceCounter();
    new.flags = flags;
    new.z_index = z_index;

    if(insert_anim)
        insert(&objects, &new, compareObjects);

    return new;
}

void start_lerp_animation(Animation* animation, SDL_FPoint start, SDL_FPoint end, float speed, u32 z_index) {
    assert((animation->flags & LOOP) > 0); // If not looping wouldn't work properly
    Lerp l = {OBJECT_LERP, z_index, LERP_ANIMATION, *animation, start, end, SDL_GetPerformanceCounter(), (get_distance(end, start) / speed)};
    insert(&objects, &l, compareObjects);
}

void start_lerp_texture(SDL_Texture** texture, SDL_FPoint start, SDL_FPoint end, float speed, u32 z_index) {
    Lerp l = {OBJECT_LERP, z_index, LERP_TEXTURE, {.texture = *texture}, start, end, SDL_GetPerformanceCounter(), (get_distance(end, start) / speed)};
    
    insert(&objects, &l, compareObjects);
}

bool draw_lerp(u32 pos) {
    Lerp* l = get(objects, pos);
    SDL_FPoint temp = lerp2d(l->start, l->end, ((SDL_GetPerformanceCounter() - l->start_time) / (float) SDL_GetPerformanceFrequency()) / l->duration);
    
    if(round(temp.x) == round(l->end.x) && round(temp.y) == round(l->end.y)) {
        return true;
    }

    switch (l->lerp_type) {
        case LERP_ANIMATION: {
            SDL_FRect temp2 = {temp.x, temp.y, 0, 0};
            return draw_animation_base(&l->graphic.animation, &temp2, true);
            break;
        }
        case LERP_TEXTURE: {
            blit(l->graphic.texture, temp.x, temp.y, true);
            break;
        }
        
        default:
            break;
    }

    return false;
}