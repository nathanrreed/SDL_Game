#include "common.h"

#include "../input.h"
#include "../main.h"
#include "../structs.h"
#include "../types.h"

void pressUp(ButtonElement* e, u8 button) {
    printf("I WAS CLICKED, %s : %s\n", e->text, button == 1 ? "left" : "right");
    e->flags &= ~PRESSING;
    // c->health = 0;
}

void pressDown(ButtonElement* e, u8 button) {
    select_button(find_active(e), e);
    e->flags |= PRESSING;
}

void no_action(ButtonElement* e, u8 button) {}

void basic_hover(ButtonElement* e, bool over) {
    e->flags = (e->flags & ~HOVER) | (over << 31);

    // if(over && (e->flags & DISABLED) > 0) //TODO IMPLEMENT
    //     SDL_SetCursor(deny_cursor);
    // else if(over)
    //     SDL_SetCursor(hand_cursor);
    // else
    //     SDL_SetCursor(normal_cursor);
}

string hello(TextBox* textbox) {
    if (textbox->text == NULL) {
        textbox->text = malloc(64);
    } else {
    }

    strcpy(textbox->text, "AWWWWWWWWWWWWWWWWWWWW");

    return textbox->text;
}

bool no_events(Object* object, SDL_Event* event) { return false; }

bool escape_toggle(Object* object, SDL_Event* event) {
    if (event->type == SDL_EVENT_KEY_UP && event->key.keysym.sym == SDLK_ESCAPE) {
        Menu* sub_menu = find_high_z_menu(object->menu.components, (MENU_HIDDEN | MENU_ACTIVE), MENU_ACTIVE);
        if (sub_menu != NULL) {
            object->menu.flags |= MENU_ACTIVE;
            object->menu.flags &= (~MENU_HIDE_BUTTONS);
            sub_menu->flags ^= (MENU_HIDDEN | MENU_ACTIVE);
        } else {
            Menu* menu = find_active_menu(objects);
            if (menu != NULL && menu != &object->menu) {
                menu->flags &= (~MENU_ACTIVE);
            } else {
                menu = find_high_z_menu(objects, MENU_ACTIVE | MENU_HIDDEN, 0);
                if (menu != NULL && menu != &object->menu) {
                    menu->flags |= MENU_ACTIVE;
                }
            }
            object->menu.flags ^= (MENU_HIDDEN | MENU_ACTIVE);
            object->menu.flags &= (~MENU_HIDE_BUTTONS);
        }

        return true;
    }

    return false;
}