#include "../types.h"
#include "../structs.h"

#include "common.h"

void pressUp(ButtonElement* e, u8 button){
    printf("I WAS CLICKED, %s : %s\n", e->text, button == 1 ? "left" : "right");
    e->flags &= ~PRESSING;
    // c->health = 0;
}

void pressDown(ButtonElement* e, u8 button){
    e->flags |= PRESSING;
}

void no_action(ButtonElement* e){}

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
    if(textbox->text == NULL) {
        textbox->text = malloc(64);
    } else {

    }

    strcpy(textbox->text, "AWWWWWWWWWWWWWWWWWWWW");

    return textbox->text;
}