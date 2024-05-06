#include "escape_menu.h"

#include <SDL3_image/SDL_image.h>
#include <math.h>

#include "../animate.h"
#include "../defs.h"
#include "../gui.h"
#include "../input.h"
#include "../main.h"
#include "../menu_components/drop_down.h"
#include "../structs.h"
#include "../types.h"
#include "common.h"
#include "options_menu.h"

void resume(ButtonElement* e, u8 button) {
    if (button == 1) {
        Menu* menu = find_active_menu(objects);
        if (menu != NULL) {  // Hide menu
            menu->flags |= MENU_HIDDEN;
            menu->flags &= (~MENU_ACTIVE);
        }

        menu = find_high_z_menu(objects, MENU_ACTIVE | MENU_HIDDEN, 0);
        if (menu != NULL) {  // Give active to highest non hidden
            menu->flags |= MENU_ACTIVE;
        }

        pressUp(e, button);
    }
}

void quit(ButtonElement* e, u8 button) {
    if (button == 1) {
        // TODO ADD SAVE
        exit(0);
    }
}

void open_options(ButtonElement* e, u8 button) {
    Menu* menu = find_menu(objects, MENU_OPTIONS, 0, 0);
    Menu* pause = find_menu(objects, MENU_PAUSE, 0, 0);
    if (menu != NULL) {
        menu->flags ^= (MENU_ACTIVE | MENU_HIDDEN);
        pause->flags ^= (MENU_ACTIVE | MENU_HIDE_BUTTONS);
    }

    pressUp(e, button);
}

// bool pause_menu(Object* object, SDL_Event* event) {
//     if (event->type == SDL_EVENT_KEY_UP && event->key.keysym.sym == SDLK_ESCAPE) {
//         return escape_toggle(object, event);
//     }

//     return false;
// }

void create_escape_menu() {
    Menu menu = create_menu(MENU_PAUSE, PAUSE, 3, 0, 3, &escape_toggle, MENU_HIDDEN);  // TODO GET CORRECT LENGTH
    Array* m = menu.components;

    Object object = (Object)create_u_element(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 8, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3 * 2, GUI);
    insert(&m, &object, compareObjects);

    // Array* options = create_array(sizeof(char) * 11, 4, 0, NULL);
    // add(&options, "2560x1440");
    // add(&options, "1920x1080");
    // add(&options, "1366x768");
    // add(&options, "1280x720");

    // Menu drop_down = create_drop_down((SDL_FPoint){50, 50}, MENU_ESCAPE, options, 203, &choose_resolution);
    // insert(&m, &drop_down, compareObjects);
    Menu options = create_options_menu();
    insert(&m, &options, compareObjects);

    // object = (Object)create_p_element(SCREEN_WIDTH / 2, lower_third, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3, 5, GUI + 1);
    // insert(&m, &object, compareObjects);

    // TextBox text = create_textbox(&hello, object.padded.inner, (SDL_Color){0, 0, 0, 255}, 0, GUI + 2);
    // insert(&m, &text, compareObjects);

    // Texture texture = (Texture){IMG_LoadTexture(app.renderer, "gfx/ui/container.png")};
    object = (Object)create_construct(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 8, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3 * 2, GRID_SIZE, GUI + 1, get(textures, UI_TEXTURE));
    insert(&m, &object, compareObjects);

    // Buttons should not be inserted
    ButtonElement button = create_button(SCREEN_WIDTH / 5 * 2, SCREEN_HEIGHT / 8, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 6, 6, "Resume", &pressDown, &resume, &basic_hover, CENTER_X | CENTER_Y | SELECTED | ACTIVE, BUTTON);
    ButtonElement* btn_resume = add(&menu.buttons, &button);

    button = create_button(SCREEN_WIDTH / 5 * 2, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 6, 6, "Options", &pressDown, &open_options, &basic_hover, CENTER_X | CENTER_Y, BUTTON);
    ButtonElement* btn_options = add(&menu.buttons, &button);

    button = create_button(SCREEN_WIDTH / 5 * 2, SCREEN_HEIGHT / 8 * 5, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 6, 6, "Quit", &pressDown, &quit, &basic_hover, CENTER_X | CENTER_Y, BUTTON);
    ButtonElement* btn_quit = add(&menu.buttons, &button);

    button_create_connections(menu.buttons, btn_resume, NULL, NULL, NULL, btn_options);
    button_create_connections(menu.buttons, btn_options, NULL, NULL, btn_resume, btn_quit);

    button_create_connections(menu.buttons, btn_quit, NULL, NULL, btn_options, NULL);

    insert(&objects, &menu, compareObjects);
}