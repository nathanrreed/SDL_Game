#include "drop_down.h"

#include <math.h>

#include "../animate.h"
#include "../defs.h"
#include "../gui.h"
#include "../input.h"
#include "../main.h"
#include "../menus/common.h"
#include "../structs.h"
#include "../types.h"

Menu create_drop_down(SDL_FPoint pos, u32 z_index, Array* items, u16 max_height, btn_press onPressUp) {
    Menu menu = create_menu(MENU_DROP_DOWN, z_index, 1, 0, items->length, &no_events, 0);  // TODO GET CORRECT LENGTH
    Array* m = menu.components;

    string text;
    u32 max_width = 0;
    for (int i = 0; i < items->length; i++) {  // Get max width
        text = get(items, i);
        max_width = fmax(max_width, strlen(text));
    }

    max_width = max_width * 6 + 2;
    max_height = max_height / 9 * 9;  // Make it a multiple of the button size
    bool limited = (max_height < items->length * 9);

    assert(!limited); // TODO implement

    u32 num_visible = limited ? max_height / 9 : items->length;
    ButtonFlags flags;
    ButtonElement button;
    ButtonElement *btn_el, *prev;
    for (int i = 0; i < items->length; i++) {
        text = get(items, i);

        if (i == 0)
            flags = CENTER_X | CENTER_Y | SELECTED | ACTIVE;
        else if (i < num_visible)
            flags = CENTER_X | CENTER_Y;
        else
            flags = CENTER_X | CENTER_Y | HIDDEN;

        prev = btn_el;
        button = create_button(pos.x, i * 8 + pos.y, max_width + 2, 11, 1, text, &pressDown, onPressUp, &basic_hover, flags, BUTTON);
        btn_el = add(&menu.buttons, &button);
        
        if (i == 0) {
            button_create_connections(menu.buttons, btn_el, NULL, NULL, NULL, NULL);
        } else {
            button_create_connections(menu.buttons, btn_el, NULL, NULL, prev, NULL);
            button_append_connections(prev, NULL, NULL, NULL, btn_el);
        }
    }

    // Object object = (Object)create_u_element(pos.x, pos.y, max_width + 2, fmin(max_height, items->length * 9) + (limited ? 1 : -1), GUI);  // TODO MAKE THIS A RELIABLE SIZE LESS THAN MAX_HEIGHT
    // add(&m, &object);

    return menu;
}