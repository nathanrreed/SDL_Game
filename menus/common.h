#include "../types.h"
#include "../structs.h"

void pressDown(ButtonElement* e, u8 button);
void pressUp(ButtonElement* e, u8 button);
void no_action(ButtonElement* e);
void basic_hover(ButtonElement* e, bool over);

string hello(TextBox* textbox);

typedef enum {
    BATTLE
} MenuIDs;

#pragma once