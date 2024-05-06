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

// GUI* create_GUI() {
//      // TODO TAKE IN FILE OR SOMETHING
//     float lower_third = SCREEN_HEIGHT - SCREEN_HEIGHT / 3;
//     int num_elements = 6;
//     GUI* gui = malloc(sizeof(GUI) + sizeof(ElementGUI) * num_elements); // Allocates space for the gui and its elements
//     gui->num_elements = num_elements;
//     gui->elements = ((void *) gui + sizeof(GUI));

//     // Will be draw in the order they appear
//     int i = 0;
//     gui->elements[i++].unpadded = create_element(0, lower_third, SCREEN_WIDTH, SCREEN_HEIGHT / 3);
//     gui->elements[i++].padded = create_p_element(SCREEN_WIDTH / 2, lower_third, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3, 5);
//     // gui->elements[i++].unpadded = create_element(SCREEN_WIDTH / 2, lower_third, 20, 24);

//     gui->elements[i++].button = create_button(0,                lower_third,                       SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 5, "Attack", &test, &basic_hover, CENTER_X | CENTER_Y);
//     gui->elements[i++].button = create_button(0,                SCREEN_HEIGHT - SCREEN_HEIGHT / 6, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 5, "Equipment", &test, &basic_hover, CENTER_X | CENTER_Y);
//     gui->elements[i++].button = create_button(SCREEN_WIDTH / 4, lower_third,                       SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 5, "Magic", &test, &basic_hover, CENTER_X | CENTER_Y);
//     gui->elements[i++].button = create_button(SCREEN_WIDTH / 4, SCREEN_HEIGHT - SCREEN_HEIGHT / 6, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 5, "Run", &test, &basic_hover, CENTER_X | CENTER_Y);

//     assert(i == num_elements);
//     return gui;
// }

// void set_active(Array* arr) {
//     // printf("CALLED\n");
//     ButtonElement* temp;
//     for(int i = 0; i < arr->length; i++) {
//         temp = get(arr, i);
//         temp->id = i;
//         if((temp->flags & ACTIVE) > 0) {
//             // printf("%p\n", active);
//             active = temp;
//             // printf("%p\n", active);
//             // printf("FOUND\n");
//             // return;
//         }
//     }
// }

Construct create_construct(u32 x, u32 y, u32 w, u32 h, u32 size, u32 z_index, SDL_Texture* texture) {
    Construct e;
    e.type = OBJECT_CONSTRUCT;
    e.rect = (SDL_FRect){x, y, w, h};
    e.z_index = z_index;
    e.texture = texture;
    e.size = size;

    for (int i = 0; i < 9; i++) {
        e.offsets[i] = (SDL_FPoint){i * size, 0};
    }

    return e;
}

// void hide_menu(Array* menu, u32 id, bool hide) {
//     Object* object;
//     for(int j = 0; j < menu->length; j++) {
//         object = get(menu, j);
//         if(object->type == OBJECT_MENU) {
//             if(object->menu.id == id) {
//                 object->menu.hidden ^= hide;
//                 return;
//             }
//             hide_menu(object->menu.components, id, hide);
//         }
//     }
// }

Menu create_menu(u32 id, u32 z_index, u32 length, u32 textures_length, u32 buttons_length, event_fn onEvent, MenuEnum flags) {
    Menu e;
    e.type = OBJECT_MENU;
    e.z_index = z_index;
    e.id = id;
    e.components = create_array(sizeof(Object), length, 0, NULL);
    e.textures = create_array(sizeof(Texture), textures_length, 0, NULL);
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
    // // TODO TAKE IN FILE OR SOMETHING
    // float lower_third = SCREEN_HEIGHT - SCREEN_HEIGHT / 3;

    // Object object = (Object) create_u_element(0, lower_third, SCREEN_WIDTH, SCREEN_HEIGHT / 3, GUI);
    // insert(&objects, &object, compareObjects);

    // object = (Object) create_p_element(SCREEN_WIDTH / 2, lower_third, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3, 5, GUI + 1);
    // insert(&objects, &object, compareObjects);

    // textures[num_textures] = IMG_LoadTexture(app.renderer, "gfx/ui/container.png");
    // object = (Object) create_construct(0, lower_third, SCREEN_WIDTH, SCREEN_HEIGHT / 3, GRID_SIZE, GUI + 1, textures[num_textures++]);
    // insert(&objects, &object, compareObjects);

    // // Buttons are not sorted!!
    // ButtonElement button = create_button(0, lower_third, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 6, "Attack", &test, &basic_hover, CENTER_X | CENTER_Y | SELECTED | ACTIVE, BUTTON);
    // ButtonElement* btn1 = add(&buttons, &button);
    // button_create_connections(btn1, -1, 2, -1, 1);

    // button = create_button(0, SCREEN_HEIGHT - SCREEN_HEIGHT / 6, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 6, "Equipment", &test, &basic_hover, CENTER_X | CENTER_Y, BUTTON);
    // ButtonElement* btn2 = add(&buttons, &button);
    // button_create_connections(btn2, -1, 3, 0, -1);

    // button = create_button(SCREEN_WIDTH / 4, lower_third, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 6, "Magic", &test, &basic_hover, CENTER_X | CENTER_Y, BUTTON);
    // ButtonElement* btn3 = add(&buttons, &button);
    // button_create_connections(btn3, 0, -1, -1, 3);

    // button = create_button(SCREEN_WIDTH / 4, SCREEN_HEIGHT - SCREEN_HEIGHT / 6, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 6, "Run", &test, &basic_hover, CENTER_X | CENTER_Y, BUTTON);
    // ButtonElement* btn4 = add(&buttons, &button);
    // button_create_connections(btn4, 1, -1, 2, -1);

    // // button = create_button(0, lower_third - 50, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 6, "ASS", &test, &basic_hover, CENTER_X | CENTER_Y | SELECTED | HIDDEN | DISABLED, BUTTON);
    // // ButtonElement* btn5 = add(&buttons, &button);
    // // button_create_connections(btn5, -1, 2, -1, 1);

    create_battle_menu();
    create_escape_menu();

    // set_active(buttons);
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

// void destroy_GUI(GUI* gui) {
//     free(gui);
// }

// void draw_GUI(GUI* gui) {
//     float lower_third = SCREEN_HEIGHT - SCREEN_HEIGHT / 3;

//     // SDL_FRect rect = {0, lower_third, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6};
//     // render_text("AAAS", rect, CENTER_X | CENTER_Y | JUSTIFY_Y);

//     // SDL_RenderLine(app.renderer, 0, lower_third, SCREEN_WIDTH, lower_third);

//     for(int i = 0; i < gui->num_elements; i++) {
//         switch (gui->elements[i].type) {
//         case UNPADDED:
//         {
//             UnPaddedElement element = gui->elements[i].unpadded;
//             SDL_SetRenderDrawColor(app.renderer, element.color.r, element.color.g, element.color.b, element.color.a);
//             SDL_RenderFillRect(app.renderer, &element.rect);
//             break;
//         }
//         case PADDED:
//         {
//             PaddedElement element = gui->elements[i].padded;
//             SDL_SetRenderDrawColor(app.renderer, element.inner_color.r, element.inner_color.g, element.inner_color.b, element.inner_color.a);
//             SDL_RenderFillRect(app.renderer, &element.inner);
//             break;
//         }
//         case BUTTON:
//         {
//             ButtonElement element = gui->elements[i].button;
//             draw_button(&element);
//             break;
//         }
//         default:
//             break;
//         }
//     }
// }