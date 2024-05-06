#ifndef INPUT_H
#define INPUT_H

#include "structs.h"
#include "menus/common.h"

extern Array* user_inputs;

void check_inputs(Object* object, event_fn onEvent);

ButtonElement* find_active(ButtonElement* btn);

Menu* find_high_z_menu(Array* arr, MenuEnum flags, MenuEnum flags_check);

Menu* find_menu(Array* arr, MenuIDs id, MenuEnum flags, MenuEnum flags_check);

Menu* find_active_menu(Array* arr);

void select_button(ButtonElement* old, ButtonElement* new);

void doInput(float elapsedMS);
void check_button_input(ButtonElement* btn);

#endif