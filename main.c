#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <math.h>

#include "init.h"
#include "draw.h"
#include "input.h"
#include "main.h"
#include "globals.h"
#include "types.h"
#include "defs.h"
#include "gui.h"

App app = {};
Character *c = NULL;
TTF_Font* font = NULL;
TTF_Font* font2 = NULL;
Array* textures = NULL;
Array* animations= NULL;
Array* objects = NULL;
ButtonElement* active = NULL;
SDL_Cursor* cursor = NULL, *hand_cursor = NULL, *deny_cursor = NULL; 
Map* map = NULL;
SDL_Point resolution = {1792, 1008};
GameState state = PLAYER_TURN;

bool add_item(Character* c, u32 id) {
    assert(id > 0);
    for (u8 i = 0; i < INV_SIZE; i++) {
        if (c->inventory.items[i].id == 0) {
            c->inventory.items[i] = (HeldItem) {.id=id, .flags=0, .durability=ITEMS[id].durability};
            return true;
        }
    }

    return false;
}

Item get_item(Character* c, u16 slot) {
    return ITEMS[c->inventory.items[slot].id];
}

// TODO MOVE
Character* create_character(ClassEnum class, int x, int y, int health, string name) {
    Character* c = calloc(1, sizeof(Character));
    c->class = class;
    c->position = (SDL_FRect) {.x=x, .y=y, .w=16, .h=16};
    c->health = CLASSES[class].base_hp;
    c->grid_position = (SDL_FPoint) {map->start_x, map->start_y};

    c->name = malloc(sizeof(char) * (strlen(name) + 1));
    strcpy(c->name, name);

    assert(add_item(c, 1) == true);

    // Set all stats to 0
    for (u8 i = 0; i < NUM_STATS; i++) {
        c->stats[i] = (HeldStat) {.id=i, .level=0, .flags=0};
    }
    
    return c;
}

void destroy_character(Character *c){
    // free(c->inventory.items);
    free(c->name);
    free(c);
}

int main(){
    memset(&app, 0, sizeof(App)); // Clear app

    initSDL();
    atexit(cleanup);

    u64 startPerf, endPerf;    
    while(true) {
        startPerf = SDL_GetPerformanceCounter();
        prepareScene();

        cursor = NULL;
        if(state != ENEMY_TURN) // TODO only checking input when player can act
            doInput((startPerf - endPerf) / (float) SDL_GetPerformanceFrequency());

        draw_map(); //TODO HAVE THIS RETURN A LIST OF THINGS TO DRAW WITH A Z-INDEX

        draw_objects(objects);

        endPerf = SDL_GetPerformanceCounter();
        float framePerf = (endPerf - startPerf) / (float) SDL_GetPerformanceFrequency();
        float elapsedMS = framePerf * 1000.0f;

        #ifdef DEBUG

        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
        float mx, my;
        char mouse_string[200];
        char fps[100];
        SDL_GetMouseState(&mx, &my);
        SDL_RenderCoordinatesFromWindow(app.renderer, mx, my, &mx, &my);
        SDL_RenderPoint(app.renderer,(int) round(mx), (int) round(my));

        sprintf(mouse_string, "%d,%d", (int) round(mx), (int) round(my));
        SDL_FRect r = (SDL_FRect) {round(mx), round(my), 60, 24};
        draw_text(render_text(mouse_string, &r, 0), &r);

        snprintf(fps, 100, "%d fps", framePerf > 6.9444f ? (int)floor(1.0f / framePerf) : 144);

		// Display string
		SDL_FRect dest = { SCREEN_WIDTH, 0, 0, 8 };
		draw_text(render_text(fps, &dest, JUSTIFY_X | CENTER_Y), &dest);

        #endif

        if(cursor == NULL) {
            SDL_SetCursor(SDL_GetDefaultCursor());
        }


        SDL_RenderPresent(app.renderer);

        SDL_Delay((int)fmax(0, floor(6.9444f - elapsedMS))); // Cap FPS at 144
    }
    
    return 0;
}