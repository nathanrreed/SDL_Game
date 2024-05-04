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

void no_action(ButtonElement* e) {}

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
        object->menu.flags ^= (MENU_HIDDEN | MENU_ACTIVE);

        Menu* menu = find_active_menu(objects);
        if (menu != NULL && menu != &object->menu) {
            menu->flags &= (~MENU_ACTIVE);
        }

        return true;
    }

    return false;
}