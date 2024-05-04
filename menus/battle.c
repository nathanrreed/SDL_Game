#include "battle.h"

#include <SDL3_image/SDL_image.h>

#include "../animate.h"
#include "../defs.h"
#include "../gui.h"
#include "../main.h"
#include "../structs.h"
#include "../types.h"
#include "common.h"

void create_battle_menu() {
    float lower_third = SCREEN_HEIGHT - SCREEN_HEIGHT / 3;

    Menu menu = create_menu(MENU_BATTLE, GUI, 10, 1, 4, &no_events, MENU_ACTIVE);  // TODO GET CORRECT LENGTH
    Array* m = menu.components;

    Object object = (Object)create_u_element(0, lower_third, SCREEN_WIDTH, SCREEN_HEIGHT / 3, GUI);
    insert(&m, &object, compareObjects);

    object = (Object)create_p_element(SCREEN_WIDTH / 2, lower_third, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3, 5, GUI + 1);
    insert(&m, &object, compareObjects);

    TextBox text = create_textbox(&hello, object.padded.inner, (SDL_Color){0, 0, 0, 255}, 0, GUI + 2);
    insert(&m, &text, compareObjects);

    Texture texture = (Texture){IMG_LoadTexture(app.renderer, "gfx/ui/container.png")};
    add(&menu.textures, &texture);
    object = (Object)create_construct(0, lower_third, SCREEN_WIDTH, SCREEN_HEIGHT / 3, GRID_SIZE, GUI + 1, texture.texture);
    insert(&m, &object, compareObjects);

    // Buttons should not be inserted
    ButtonElement button = create_button(0, lower_third, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 6, "Attack", &pressDown, &pressUp, &basic_hover, CENTER_X | CENTER_Y | SELECTED | ACTIVE, BUTTON);
    ButtonElement* btn1 = add(&menu.buttons, &button);

    button = create_button(0, SCREEN_HEIGHT - SCREEN_HEIGHT / 6, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 6, "Equipment", &pressDown, &pressUp, &basic_hover, CENTER_X | CENTER_Y, BUTTON);
    ButtonElement* btn2 = add(&menu.buttons, &button);

    button = create_button(SCREEN_WIDTH / 4, lower_third, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 6, "Magic", &pressDown, &pressUp, &basic_hover, CENTER_X | CENTER_Y, BUTTON);
    ButtonElement* btn3 = add(&menu.buttons, &button);

    button = create_button(SCREEN_WIDTH / 4, SCREEN_HEIGHT - SCREEN_HEIGHT / 6, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 6, "Run", &pressDown, &pressUp, &basic_hover, CENTER_X | CENTER_Y, BUTTON);
    ButtonElement* btn4 = add(&menu.buttons, &button);

    button_create_connections(menu.buttons, btn1, NULL, btn3, NULL, btn2);
    button_create_connections(menu.buttons, btn2, NULL, btn4, btn1, NULL);
    button_create_connections(menu.buttons, btn3, btn1, NULL, NULL, btn4);
    button_create_connections(menu.buttons, btn4, btn2, NULL, btn3, NULL);

    // // button = create_button(0, lower_third - 50, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 6, "ASS", &test, &basic_hover, CENTER_X | CENTER_Y | SELECTED | HIDDEN | DISABLED, BUTTON);
    // // ButtonElement* btn5 = add(&buttons, &button);
    // // button_create_connections(btn5, -1, 2, -1, 1);

    active = btn1;

    insert(&objects, &menu, compareObjects);
}