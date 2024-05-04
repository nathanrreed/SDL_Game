#include "types.h"

typedef struct {
    unsigned int magic;
    unsigned int version;
    unsigned int size_x;
    unsigned int size_y;
    unsigned int start_x;
    unsigned int start_y;
} Header;

void create_map(string location);
void destroy_map(Map* map);

Array* a_star(SDL_FPoint start, SDL_FPoint goal);

#pragma once