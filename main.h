#ifndef MAIN_H
#define MAIN_H

#include <SDL3_ttf/SDL_ttf.h>

#include "animate.h"
#include "enums.h"
#include "structs.h"
#include "types.h"

extern App app;
extern Character* c;
extern TTF_Font* font;
extern TTF_Font* font2;
// GUI * gui;

// u32 num_textures;
// SDL_Texture* textures[256]; // TEMP YOU HAVE 256 textures
// u32 num_animations;
// Animation animations[256]; // TEMP YOU HAVE 256 animations

// Array* running_animations;
// Array* lerps;

extern SDL_Point resolution;

typedef enum {
    CURRENT_MAP_TEXTURE,
    UI_TEXTURE,
    PLAYER_TEXTURE
} DefinedTextures;

typedef enum {
    PLAYER_TURN,
    ENEMY_TURN,
    PAUSED
} GameState;

extern GameState state;

extern Array* textures;
extern Array* animations;
extern Array* objects;
// Array* buttons;

extern ButtonElement* active;

extern SDL_Cursor *cursor, *hand_cursor, *deny_cursor;

extern Map* map;

bool add_item(Character* c, u32 id);

Item get_item(Character* c, u16 slot);
Character* create_character(ClassEnum, int x, int y, int health, string name);

void destroy_character(Character* c);

#endif