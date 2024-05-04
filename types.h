#ifndef TYPES
#define TYPES true

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>

#include <SDL3/SDL.h>

#include "enums.h"
// #include "structs.h"

#define INV_SIZE 27 // maybe different sizes?

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t    s8;
typedef int16_t   s16;
typedef int32_t   s32;
typedef int64_t   s64;

typedef char*     string;

typedef struct {
    u16 level;
    u16 xp;
} Level;

typedef struct {
    //Something to have the picture or whatever
    string name;
    u16 level_requirement;
    s32 durability;
} Item; // Item found in the global scope

typedef struct {
    u32 id; // Position of the item in the gloal array of items
    u32 flags;
    s32 durability;
    // To get name ITEMS[id].name
} HeldItem; // Item found in local scope

// typedef enum {
// 	INV_LVL_1 = 1,
// 	INV_LVL_2 = 1,
//     INV_LVL_3 = 1,
// } InvSize;

typedef struct {
    HeldItem items[INV_SIZE];
    u32 flags; //
} Inventory;

typedef struct {
    s32 x;
    s32 y;
} Vector;

typedef struct {
    string name;
} Stat;

typedef struct {
    u16 id;
    u16 level;
    u32 flags;
} HeldStat;

typedef struct {
    string name;
    u16 base_hp;
} Class;

typedef struct {
    ClassEnum class;
    string name; //Change to be fixed size so it is all contiguous?
    SDL_FPoint grid_position;
    SDL_FRect position;
    s32 health;
    Level lvl_info;
    Inventory inventory;
    HeldStat stats[NUM_STATS];
} Character;

typedef enum {
    HIGH_Z = 0x1
} MapEnum;

typedef struct {
    u32 id;
    MapEnum flags;
} MapTile;

typedef struct {
    u32 size_x;
    u32 size_y;
    u32 start_x;
    u32 start_y;
    MapTile* tiles;
} Map;

// File format
// enum name, stat name
#include "array.h"
#endif