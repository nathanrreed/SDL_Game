#ifndef STRUCTS
#define STRUCTS true

#include <SDL3/SDL.h>

#include "types.h"

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;
} App;

// typedef enum {
//     UNPADDED,
//     PADDED,
//     BUTTON
// } ElementsEnum;

typedef enum {
    OBJECT_ANIMATION,
    OBJECT_LERP,
    OBJECT_UNPADDED,
    OBJECT_PADDED,
    OBJECT_CONSTRUCT,
    OBJECT_BUTTON,
    OBJECT_MENU,
    OBJECT_TEXT
} ObjectEnum;

typedef enum {
    CON_TOP_LEFT,
    CON_TOP,
    CON_TOP_RIGHT,
    CON_RIGHT,
    CON_BOTTOM_RIGHT,
    CON_BOTTOM,
    CON_BOTTOM_LEFT,
    CON_LEFT,
    CON_FILL
} ConstuctEnum;

typedef struct {
    SDL_Texture* texture;
} Texture;

typedef struct {
    ObjectEnum type;
    u32 z_index;
    u32 size;
    SDL_FRect rect;
    SDL_Texture* texture;
    SDL_FPoint offsets[9];
} Construct;

typedef enum {
    LOOP = 0x1,
    LERP = 0x2
} AnimationEnum;

typedef struct {
    ObjectEnum type;
    u32 z_index;
    u32 frames;
    float duration;
    u64 start_time;
    SDL_Texture* texture;
    SDL_FPoint t_size;
    u32 flags;
} Animation;

typedef enum {
    LERP_ANIMATION,
    LERP_TEXTURE
} LerpEnum;

typedef union {
    Animation animation;
    Texture texture;
} TextureOrAnimation;

typedef struct {
    ObjectEnum type;
    u32 z_index;
    LerpEnum lerp_type;
    TextureOrAnimation graphic;
    SDL_FPoint start;
    SDL_FPoint end;
    u64 start_time;
    float duration;
} Lerp;

typedef struct {
    ObjectEnum type;
    u32 z_index;
} DummyObject;

typedef enum {
    JUSTIFY_X = 0x1,
    CENTER_X = 0x2,
    JUSTIFY_Y = 0x4,
    CENTER_Y = 0x8,
    // SMALL = 0x10
} TextRenderFlags;

#define BUTTON_TEXT_LEN 16  // Increase later if needed
typedef enum {
    // TextRenderFlags,
    PRESSING = 0x4000000,
    HIDDEN = 0x8000000,
    DISABLED = 0x10000000,
    ACTIVE = 0x20000000,
    SELECTED = 0x40000000,
    HOVER = 0x80000000
} ButtonFlags;

// struct ButtonElement;

// typedef struct {
//     struct ButtonElement* left;
//     struct ButtonElement* right;
//     struct ButtonElement* up;
//     struct ButtonElement* down;
// } ButtonDirections;

// typedef enum {
//     LEFT,
//     RIGHT,
//     UP,
//     DOWN
// } Direction;

typedef struct {  // Offsets from the current button to the button in question
    s32 left;
    s32 right;
    s32 up;
    s32 down;
    s32 start;
} ButtonDirections;

struct ButtonElement;

typedef void (*btn_press)(struct ButtonElement*, u8);
typedef void (*btn_hover)(struct ButtonElement*, bool);

typedef struct ButtonElement {
    ObjectEnum type;
    u32 z_index;
    SDL_FRect rect;
    ButtonFlags flags;
    SDL_Color color;  // ADD TEXTURE OPTION
    SDL_Color hover_color;
    ButtonDirections direction;
    char text[BUTTON_TEXT_LEN];
    btn_press onPressDown;
    btn_press onPressUp;
    btn_hover onHover;
} ButtonElement;

typedef struct {
    ObjectEnum type;
    u32 z_index;
    SDL_FRect inner;
    SDL_FRect outer;
    SDL_Color inner_color;
    SDL_Color outer_color;  // ADD TEXTURE OPTION
} PaddedElement;

typedef struct {
    ObjectEnum type;
    u32 z_index;
    SDL_FRect rect;
    SDL_Color color;  // ADD TEXTURE OPTION
} UnPaddedElement;

typedef enum {
    MENU_HIDDEN = 0x1,  // Will it be drawn
    MENU_ACTIVE = 0x2,  // Can any buttons be pressed
    MENU_HIDE_BUTTONS = 0x4,
} MenuEnum;

union Object;
typedef bool (*event_fn)(union Object*, SDL_Event*);

typedef struct {
    ObjectEnum type;
    u32 z_index;
    u32 id;
    MenuEnum flags;
    Array* components;
    Array* textures;
    Array* buttons;
    event_fn onEvent;
} Menu;

struct TextBox;
typedef string (*text_call)(struct TextBox*);
typedef struct TextBox {
    ObjectEnum type;
    u32 z_index;
    text_call toDraw;
    SDL_FRect rect;
    SDL_Color color;
    TextRenderFlags flags;
    string text;
} TextBox;

typedef union Object {
    ObjectEnum type;
    DummyObject obj;
    Lerp lerp;
    Animation animation;
    Menu menu;
    UnPaddedElement unpadded;
    PaddedElement padded;
    Construct construct;
    TextBox textbox;
    // ButtonElement button;
    // Texture texture;
} Object;

// typedef union {
//     ElementsEnum type; // All children have the type
//     UnPaddedElement unpadded;
//     PaddedElement padded;
//     ButtonElement button;
// } ElementGUI;

// typedef struct {
//     int num_elements;
//     ElementGUI* elements;
// } GUI;

#endif