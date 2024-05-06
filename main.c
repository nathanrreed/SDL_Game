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

    // c->inventory.items = malloc(sizeof(Item) * INV_SIZE);
    // printf("%d\n", c->inventory.items[0]);
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
        doInput((startPerf - endPerf) / (float) SDL_GetPerformanceFrequency());

        // Grid
        // draw_map(); //TODO HAVE THIS RETURN A LIST OF THINGS TO DRAW WITH A Z-INDEX

        // draw_grid();

        //Guy
        // if(c->health > 0)
        //     blit_rect(textures[GUY], &c->position, true);
        // else
        // blit_rect(textures[GUY], &c->position, NULL, true);
        //SDL_RenderFillRect(app.renderer, &c->position);

        // renderText("COOL", dest);
        
        //TODO CHECK IF IT SHOULD COVER
        //    (map->tiles[((int)floor(position.y) + y) * map->size_x + x + (int)floor(c->grid_position.x)].flags & HIGH_Z) > 0.

        draw_objects(objects);

        // printf("Class: %s, Name: %s, Health: %d\nStats:\n\t", CLASSES[c->class].name, c->name, c->health);

        // char sss[200];
        // // sprintf(sss, "Class: %s, Name: %s, Health: %d\nStats:\n\t", CLASSES[c->class].name, c->name, c->health);
        // sprintf(sss, "%s, %dhp", c->name, c->health);
        // SDL_FRect r = {SCREEN_WIDTH / 2 + 6, SCREEN_HEIGHT / 3 * 2 + 6, SCREEN_WIDTH / 2 - 12, 24};
        // render_text(sss, &r, 0);

        #ifdef DEBUG

        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
        float mx, my;
        SDL_GetMouseState(&mx, &my);
        SDL_RenderCoordinatesFromWindow(app.renderer, mx, my, &mx, &my);
        SDL_RenderPoint(app.renderer,(int) round(mx), (int) round(my));
        // sprintf(sss, "Class: %s, Name: %s, Health: %d\nStats:\n\t", CLASSES[c->class].name, c->name, c->health);
        sprintf(sss, "%d, %d", (int) round(mx), (int) round(my));
        r = (SDL_FRect) {round(mx), round(my), 60, 24};
        render_text(sss, &r, 0);


		char fps[100];
        snprintf(fps, 100, "%d fps", framePerf > 6.9444f ? (int)floor(1.0f / framePerf) : 144);

		// Display string
		SDL_FRect dest = { SCREEN_WIDTH, 0, 0, 0 };
		render_text_right(fps, &dest);

        #endif

        if(cursor == NULL) {
            SDL_SetCursor(SDL_GetDefaultCursor());
        }


        SDL_RenderPresent(app.renderer);


        endPerf = SDL_GetPerformanceCounter();
        float framePerf = (endPerf - startPerf) / (float) SDL_GetPerformanceFrequency();
        float elapsedMS = framePerf * 1000.0f;
        SDL_Delay((int)fmax(0, floor(6.9444f - elapsedMS))); // Cap FPS at 144
    }
    
    return 0;
}