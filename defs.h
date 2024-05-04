#ifndef DEFS
#define DEFS

#define SCREEN_WIDTH   284 //256 320
#define SCREEN_HEIGHT  160 //144 180

#define GRID_SIZE 16

typedef enum {
    BACKGROUND = 0,
    FOREGROUND = 10,
    GUI = 100,
    BUTTON = 120,

    TOP = INT_MAX
} ZIndexes;

#endif