#include "../structs.h"
#include "../types.h"

void pressDown(ButtonElement* e, u8 button);
void pressUp(ButtonElement* e, u8 button);
void no_action(ButtonElement* e);
void basic_hover(ButtonElement* e, bool over);

string hello(TextBox* textbox);

bool no_events(Object* object, SDL_Event* event);

bool escape_toggle(Object* object, SDL_Event* event);

typedef enum {
    MENU_MAIN,
    MENU_ESCAPE,
    MENU_BATTLE
} MenuIDs;

#pragma once