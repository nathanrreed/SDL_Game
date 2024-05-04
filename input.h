#ifndef INPUT_H
#define INPUT_H

#include "structs.h"

extern Array* user_inputs;

void doInput(float elapsedMS);
void check_button_input(ButtonElement* btn);

#endif