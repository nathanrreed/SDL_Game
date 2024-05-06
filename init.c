#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdlib.h>
#include <stdio.h>

#include "animate.h"
#include "array.h"
#include "defs.h"
#include "structs.h"
#include "init.h"
#include "map.h"
#include "main.h"
#include "gui.h"
#include "input.h"

Array* user_inputs;

void initSDL(void) {
    // assert(1440 % SCREEN_HEIGHT == 0);
    // assert(2560 % SCREEN_WIDTH == 0);

    int rendererFlags, windowFlags;

    rendererFlags = 0; //SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow("SDL Game", resolution.x, resolution.y, windowFlags);
    
    if (!app.window) {
        printf("Failed to open %d x %d window: %s\n", resolution.x, resolution.y, SDL_GetError());
        exit(1);
    }

    // SDL_SetHint(SDL_HINT_RENDER, "linear");
    // SDL_HINT_RENDER_SCALE_QUALITY

    app.renderer = SDL_CreateRenderer(app.window, NULL, rendererFlags);

    if (!app.renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

	if (IMG_Init(IMG_INIT_PNG) < 0) {
		printf("Error initializing SDL_image: %s\n", IMG_GetError());
		exit(1);
	}

	if (TTF_Init() < 0) {
		printf("Error initializing SDL_ttf: %s\n", TTF_GetError());
		exit(1);
	}

    font = TTF_OpenFont("fonts/font.ttf", 8);
    if (font == NULL) {
		printf("Error initializing font: %s\n", TTF_GetError());
		exit(1);
	}

    // SDL_HideCursor(); // TODO ADD CUSTOM CURSOR

    SDL_SetRenderLogicalPresentation(app.renderer, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX, SDL_SCALEMODE_NEAREST);

    user_inputs = create_array(sizeof(SDL_Event), 8, 4, NULL);

    textures = create_array(sizeof(Texture), 64, 32, NULL);
    Texture texture = {IMG_LoadTexture(app.renderer, "gfx/maps/simple_map.png")};
    add(&textures, &texture);
    texture = (Texture) {IMG_LoadTexture(app.renderer, "gfx/a.png")};
    add(&textures, &texture);

    // textures[num_textures++] = IMG_LoadTexture(app.renderer, "gfx/tiles/cobble_2.png"));
    // textures[num_textures++] = IMG_LoadTexture(app.renderer, "gfx/tiles/dirt_1.png");
    // textures[num_textures++] = IMG_LoadTexture(app.renderer, "gfx/a.png");


    // num_animations = 0;
    animations = create_array(sizeof(Animation), 64, 32, NULL);
    Animation animation = create_animation("gfx/an.png", 10, 2, (SDL_FPoint) {16, 16}, true);
    add(&animations, &animation);
    animation = create_animation("gfx/ui/load.png", 6, 0.65f, (SDL_FPoint) {8, 8}, true);
    add(&animations, &animation);
    animation = create_animation("gfx/ui/load_s.png", 4, 0.65f, (SDL_FPoint) {6, 6}, true);
    add(&animations, &animation);
    // animations[num_animations++] = create_animation("gfx/an.png", 10, 2, (SDL_FPoint) {16, 16}, true);
    // animations[num_animations++] = create_animation("gfx/ui/load.png", 6, 0.65f, (SDL_FPoint) {8, 8}, true);
    // animations[num_animations++] = create_animation("gfx/ui/load_s.png", 4, 0.65f, (SDL_FPoint) {6, 6}, true);

    objects = create_array(sizeof(Object), 16, 8, NULL);
    // buttons = create_array(sizeof(ButtonElement), 16, 8, set_active);
    create_GUI();

    start_animation(get(animations, 0), LOOP, 9, true);
    start_animation(get(animations, 2), LOOP, 0, true);

    start_lerp_texture((*(Texture*) get(textures, 1)), (SDL_FPoint) {0,0}, (SDL_FPoint) {90,78}, 0, 0);
    Animation anim = start_animation(get(animations, 0), LOOP | LERP, 1, false);
    start_lerp_animation(&anim, (SDL_FPoint) {0,0}, (SDL_FPoint) {10,20}, 16, 0);

    create_map("data/map1.data");
    c = create_character(WARRIOR, 0, 0, 25, "Nred");

    hand_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    deny_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);

    SDL_Surface* icon = IMG_Load("gfx/icon.png");
    SDL_SetWindowIcon(app.window, icon);
}

void cleanup(void) {    
    for(int i = 0; i < textures->length; i++) {
        SDL_DestroyTexture(get(textures, i));
    }

    for(int i = 0; i < animations->length; i++) {
        destroy_animation(get(animations, i));
    }

    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    
    destroy_character(c);

    destroy_objects(objects);
    free(textures);
    free(animations);
    // free(buttons);

    destroy_map(map);
    SDL_DestroyCursor(hand_cursor);
    SDL_DestroyCursor(deny_cursor);

    TTF_CloseFont(font);
    TTF_Quit();
	IMG_Quit();
    SDL_Quit();
}