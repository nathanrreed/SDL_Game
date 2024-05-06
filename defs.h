#ifndef DEFS
#define DEFS

#define SCREEN_WIDTH   320 // Divides well into 1080p and 1440p
#define SCREEN_HEIGHT  180

#define GRID_SIZE 16

typedef enum {
    BACKGROUND = 0,
    FOREGROUND = 10,
    GUI = 100,
    BUTTON = 120,

    PAUSE = 1000,
    
    TOP = INT_MAX
} ZIndexes;

#endif