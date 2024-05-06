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

void open_close(Object* object) {
    object->dropdown.open ^= 1;  // Toggles open

    if (object->dropdown.open) {
        button_append_connections(get(object->dropdown.buttons, 0), NULL, NULL, NULL, get(object->dropdown.buttons, 1));
    } else {
        ((ButtonElement*) get(object->dropdown.buttons, 0))->direction.down = 0;
    }
}

bool dropdownOnEvent(Object* object, SDL_Event* event) {  // Double dip on events this will not consume, but controls all the logic
    SDL_FPoint point;
    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP || event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        SDL_RenderCoordinatesFromWindow(app.renderer, event->button.x, event->button.y, &point.x, &point.y);
    else if (event->type == SDL_EVENT_MOUSE_MOTION)
        SDL_RenderCoordinatesFromWindow(app.renderer, event->motion.x, event->motion.y, &point.x, &point.y);

    ButtonElement* button;
    u16 length = (object->dropdown.open ? object->dropdown.buttons->length : 1);
    for (int i = 0; i < length; i++) {
        button = get(object->dropdown.buttons, i);

        switch (event->type) {
            case SDL_EVENT_KEY_UP: {
                if (event->key.keysym.sym == SDLK_RETURN && (button->flags & (ACTIVE | SELECTED)) == (ACTIVE | SELECTED)) {
                    if (i == 0) {
                        open_close(object);
                    } else {
                        (*(object->dropdown.onPressUp))(button, 1);
                    }
                }
                break;
            }

            case SDL_EVENT_MOUSE_BUTTON_UP: {
                ButtonElement* active = find_active(button);
                if (active != button && active != NULL && (active->flags & PRESSING) > 0)
                    break;

                if (SDL_PointInRectFloat(&point, &button->rect) || (button->flags & PRESSING) > 0) {
                    if (i == 0) {
                        open_close(object);
                    } else {
                        (*(object->dropdown.onPressUp))(button, event->button.button);
                    }
                }

                break;
            }

            default:
                break;
        }
    }

    return false;
}

Dropdown create_drop_down(SDL_FPoint pos, u32 z_index, Array* items, u16 max_height, btn_press onPressUp) {
    max_height = max_height / 9 * 9;  // Make it a multiple of the button size
    Dropdown dropdown;
    dropdown.type = OBJECT_DROPDOWN;
    dropdown.z_index = z_index;
    // e.id = id;
    dropdown.buttons = create_array(sizeof(ButtonElement), items->length + 1, 0, NULL);
    dropdown.open = false;
    dropdown.curr_idx = 1;
    dropdown.max_num = max_height / 9;
    dropdown.onPressUp = onPressUp;

    string text;
    u32 max_width = 0;
    for (int i = 0; i < items->length; i++) {  // Get max width
        text = get(items, i);
        max_width = fmax(max_width, strlen(text));
    }
    max_width = max_width * 6 + 2;

    bool limited = (max_height < items->length * 9);

    assert(!limited);  // TODO implement

    u32 num_visible = limited ? max_height / 9 : items->length;
    ButtonFlags flags;
    ButtonElement button;
    ButtonElement *btn_el, *prev;

    button = create_button(pos.x, pos.y, max_width + 2, 11, 1, get(items, 0), &pressDown, &pressUp, &basic_hover, CENTER_X | CENTER_Y | ACTIVE, BUTTON);
    btn_el = add(&dropdown.buttons, &button);
    button_create_connections(dropdown.buttons, btn_el, NULL, NULL, NULL, NULL);

    for (int i = 1; i < items->length; i++) {
        text = get(items, i);

        if (i < num_visible)
            flags = CENTER_X | CENTER_Y;
        else
            flags = CENTER_X | CENTER_Y | HIDDEN;

        prev = btn_el;
        button = create_button(pos.x, i * 8 + pos.y, max_width + 2, 11, 1, text, &pressDown, &pressUp, &basic_hover, flags, BUTTON);
        btn_el = add(&dropdown.buttons, &button);

        button_create_connections(dropdown.buttons, btn_el, NULL, NULL, prev, NULL);

        if (i != 1)
            button_append_connections(prev, NULL, NULL, NULL, btn_el);
    }

    // Object object = (Object)create_u_element(pos.x, pos.y, max_width + 2, fmin(max_height, items->length * 9) + (limited ? 1 : -1), GUI);  // TODO MAKE THIS A RELIABLE SIZE LESS THAN MAX_HEIGHT
    // add(&m, &object);

    return dropdown;
}