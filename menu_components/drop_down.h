#ifndef DROP_DOWN_H
#define DROP_DOWN_H

#include "../types.h"
#include "../structs.h"

Menu create_drop_down(SDL_FPoint pos, u32 z_index, Array* items, u16 max_height, btn_press onPressUp);

#endif