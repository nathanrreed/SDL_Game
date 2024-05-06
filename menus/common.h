#include "../structs.h"
#include "../types.h"

void pressDown(ButtonElement* e, u8 button);
void pressUp(ButtonElement* e, u8 button);
void no_action(ButtonElement* e, u8 button);
void basic_hover(ButtonElement* e, bool over);

string hello(TextBox* textbox);

bool no_events(Object* object, SDL_Event* event);

bool escape_toggle(Object* object, SDL_Event* event);

typedef enum {
    MENU_MAIN,
    MENU_PAUSE,
    MENU_BATTLE,
    MENU_DROP_DOWN,
    MENU_OPTIONS
} MenuIDs;

#pragma once