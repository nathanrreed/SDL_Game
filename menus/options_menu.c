#include "options_menu.h"

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
#include "escape_menu.h"

void choose_resolution(ButtonElement* e, u8 button) {
    if (button == 1) {
        u32 x = strcspn(e->text, "x");
        e->text[x] = '\0';

        int width = atoi(e->text), height = atoi(&e->text[x + 1]);
        int old_width, old_height, old_x, old_y;
        SDL_GetWindowPosition(app.window, &old_x, &old_y);
        SDL_GetWindowSize(app.window, &old_width, &old_height);

        SDL_SetWindowPosition(app.window, old_x + (old_width - width) / 2, old_y + (old_height - height) / 2);
        SDL_SetWindowSize(app.window, width, height);

        e->text[x] = 'x';
    }
    // pressUp(e, button);
}

Menu create_options_menu() {
    Menu menu = create_menu(MENU_OPTIONS, PAUSE + 1, 3, 0, 3, &escape_toggle, MENU_HIDDEN);  // TODO GET CORRECT LENGTH
    Array* m = menu.components;

    Object object = (Object)create_u_element(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 8, SCREEN_WIDTH / 4 * 3, SCREEN_HEIGHT / 4 * 3, GUI);
    insert(&m, &object, compareObjects);

    Array* options = create_array(sizeof(char) * 12, 6, 0, NULL);
    add(&options, "Resolution");
    add(&options, "2560x1440");
    add(&options, "1920x1080");
    add(&options, "1600x900");
    add(&options, "1366x768");
    add(&options, "1280x720");

    // Menu drop_down = create_drop_down((SDL_FPoint){50, 50}, PAUSE, options, 203, &choose_resolution);
    // insert(&m, &drop_down, compareObjects);

    Dropdown drop_down = create_drop_down((SDL_FPoint){50, 50}, PAUSE, options, 203, &choose_resolution);
    insert(&m, &drop_down, compareObjects);



    // object = (Object)create_p_element(SCREEN_WIDTH / 2, lower_third, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3, 5, GUI + 1);
    // insert(&m, &object, compareObjects);

    // TextBox text = create_textbox(&hello, object.padded.inner, (SDL_Color){0, 0, 0, 255}, 0, GUI + 2);
    // insert(&m, &text, compareObjects);

    // Texture texture = (Texture){IMG_LoadTexture(app.renderer, "gfx/ui/container.png")};
    object = (Object)create_construct(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 8, SCREEN_WIDTH / 4 * 3, SCREEN_HEIGHT / 4 * 3, GRID_SIZE, GUI + 1, get(textures, UI_TEXTURE));
    insert(&m, &object, compareObjects);

    // Buttons should not be inserted
    // ButtonElement button = create_button(SCREEN_WIDTH / 5 * 2, SCREEN_HEIGHT / 8, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 6, 6, "Resume", &pressDown, &resume, &basic_hover, CENTER_X | CENTER_Y | SELECTED | ACTIVE, BUTTON);
    // ButtonElement* btn_resume = add(&menu.buttons, &button);

    // button = create_button(SCREEN_WIDTH / 5 * 2, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 6, 6, "Options", &pressDown, &swap_res, &basic_hover, CENTER_X | CENTER_Y, BUTTON);
    // ButtonElement* btn_options = add(&menu.buttons, &button);

    // button = create_button(SCREEN_WIDTH / 5 * 2, SCREEN_HEIGHT / 8 * 5, SCREEN_WIDTH / 5, SCREEN_HEIGHT / 6, 6, "Quit", &pressDown, &quit, &basic_hover, CENTER_X | CENTER_Y, BUTTON);
    // ButtonElement* btn_quit = add(&menu.buttons, &button);

    // button_create_connections(menu.buttons, btn_resume, NULL, NULL, NULL, btn_options);
    // button_create_connections(menu.buttons, btn_options, NULL, NULL, btn_resume, btn_quit);

    // button_create_connections(menu.buttons, btn_quit, NULL, NULL, btn_options, NULL);

    return menu;
}