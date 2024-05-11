#include "gui.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <math.h>

#include "animate.h"
#include "defs.h"
#include "draw.h"
#include "main.h"
#include "menus/battle.h"
#include "menus/escape_menu.h"
#include "types.h"

PaddedElement create_p_element(int x, int y, int w, int h, int padding, u32 z_index) {
    PaddedElement e;
    e.type = OBJECT_PADDED;
    e.outer = (SDL_FRect){x, y, w, h};
    e.inner = (SDL_FRect){x + padding, y + padding, w - 2 * padding, h - 2 * padding};
    e.inner_color = (SDL_Color){150, 150, 150, 255};
    e.outer_color = (SDL_Color){255, 255, 0, 255};
    e.z_index = z_index;

    assert(SDL_RectEmptyFloat(&e.outer) == SDL_FALSE && SDL_RectEmptyFloat(&e.inner) == SDL_FALSE);  // Makes sure the rects are well formed

    return e;
}

UnPaddedElement create_u_element(int x, int y, int w, int h, u32 z_index) {
    UnPaddedElement e;
    e.type = OBJECT_UNPADDED;
    e.rect = (SDL_FRect){x, y, w, h};
    e.color = (SDL_Color){157, 187, 150, 255};
    e.z_index = z_index;

    assert(SDL_RectEmptyFloat(&e.rect) == SDL_FALSE);  // Makes sure the rects are well formed

    return e;
}

#define calc(button, dir) (dir != NULL ? button - dir : 0)

void button_create_connections(Array* arr, ButtonElement* button, ButtonElement* left, ButtonElement* right, ButtonElement* up, ButtonElement* down) {
    button->direction = (ButtonDirections){calc(button, left), calc(button, right), calc(button, up), calc(button, down), (button - (ButtonElement*)arr->memory)};
}

// Assumes create connections was called first
void button_append_connections(ButtonElement* button, ButtonElement* left, ButtonElement* right, ButtonElement* up, ButtonElement* down) {
    button->direction.left = (left != NULL) ? (calc(button, left)) : button->direction.left;
    button->direction.right = (right != NULL) ? (calc(button, right)) : button->direction.right;
    button->direction.up = (up != NULL) ? (calc(button, up)) : button->direction.up;
    button->direction.down = (down != NULL) ? (calc(button, down)) : button->direction.down;
}

ButtonElement create_button(int x, int y, int w, int h, int padding, string text, btn_press onPressDown, btn_press onPressUp, btn_hover onHover, ButtonFlags flags, u32 z_index) {
    ButtonElement e;
    e.type = OBJECT_BUTTON;
    e.flags = flags;
    e.rect = (SDL_FRect){x + padding, y + padding, w - 2 * padding, h - 2 * padding};
    e.color = (SDL_Color){200, 50, 50, 255};
    e.hover_color = (SDL_Color){120, 200, 21, 255};
    e.onPressUp = onPressUp;
    e.onPressDown = onPressDown;
    e.onHover = onHover;
    e.z_index = z_index;
    e.direction = (ButtonDirections){0, 0, 0, 0};

    assert(strlen(text) < BUTTON_TEXT_LEN - 1);
    strncpy(e.text, text, BUTTON_TEXT_LEN - 1);

    assert(SDL_RectEmptyFloat(&e.rect) == SDL_FALSE);  // Makes sure the rects are well formed

    return e;
}

// TODO add offset into texture to start
Construct create_construct(u32 x, u32 y, u32 w, u32 h, u32 size, u32 z_index, SDL_Texture** texture) {
    Construct e;
    e.type = OBJECT_CONSTRUCT;
    e.rect = (SDL_FRect){x, y, w, h};
    e.z_index = z_index;
    e.texture = *texture;
    e.size = size;

    for (int i = 0; i < 9; i++) {
        e.offsets[i] = (SDL_FPoint){i * size, 0};
    }

    return e;
}

Menu create_menu(u32 id, u32 z_index, u32 length, u32 textures_length, u32 buttons_length, event_fn onEvent, MenuEnum flags) {
    Menu e;
    e.type = OBJECT_MENU;
    e.z_index = z_index;
    e.id = id;
    e.components = create_array(sizeof(Object), length, 0, NULL);
    e.textures = create_array(sizeof(SDL_Texture**), textures_length, 0, NULL);
    e.buttons = create_array(sizeof(ButtonElement), buttons_length, 0, NULL);
    e.flags = flags;
    e.onEvent = onEvent;

    return e;
}

TextBox create_textbox(text_call toDraw, SDL_FRect rect, SDL_Color color, TextRenderFlags flags, u32 z_index) {
    TextBox e;
    e.type = OBJECT_TEXT;
    e.z_index = z_index;
    e.rect = rect;
    e.color = color;
    e.flags = flags;
    e.toDraw = toDraw;
    e.text = NULL;

    return e;
}

void create_GUI() {
    create_battle_menu();
    create_escape_menu();
}

ButtonElement* get_btn_dir(ButtonElement* btn, s32 direction) {
    if ((btn->flags & PRESSING) > 0) {  // Disable moving while pressing
        return NULL;
    }

    return direction != 0 ? (ButtonElement*)(btn - direction) : NULL;
}

void destroy_objects(Array* objs) {
    Object* object;
    for (int i = 0; i < objs->length; i++) {
        object = get(objs, i);
        if (object->type == OBJECT_MENU) {
            destroy_objects(object->menu.components);
            free(object->menu.textures);
            free(object->menu.buttons);
        } else if (object->type == OBJECT_TEXT && object->textbox.text != NULL) {
            free(object->textbox.text);
        }
    }
    free(objs);
}