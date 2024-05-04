#include "input.h"

#include <SDL3/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "draw.h"
#include "gui.h"
#include "main.h"
#include "structs.h"

enum {
    MOVE_LEFT = 0x1,
    MOVE_RIGHT = 0x2,
    MOVE_UP = 0x4,
    MOVE_DOWN = 0x8,
    KEY_LEFT = 0x10,
    KEY_RIGHT = 0x20,
    KEY_UP = 0x40,
    KEY_DOWN = 0x80,
};

#define isActive(btn) ((btn->flags & ACTIVE) > 0 ? true : false)

u32 pressed = 0;

void check_inputs(Object* object, event_fn onEvent) {
    SDL_Event* event;
    for (int i = 0; i < user_inputs->length; i++) {
        event = get(user_inputs, i);
        if ((*onEvent)(object, event)) {  // Calls for every event waiting
            delete (user_inputs, i);
            i--;
        }
    }
}

// Menu* get_menu(ButtonElement* btn) {
//     Array* btn_arr = (void*)(btn - btn->direction.start) - sizeof(Array);
//     Array** pp = &btn_arr;
//     return (Menu*)((void*)pp - sizeof(Object));  // DOES NOT WORK!!!!!!!!
// }

ButtonElement* find_active(ButtonElement* btn) {
    Array* arr = (void*)(btn - btn->direction.start) - sizeof(Array);
    ButtonElement* curr;
    for (int i = 0; i < arr->length; i++) {
        curr = get(arr, i);
        if (isActive(curr)) {
            return curr;
        }
    }

    return NULL;
}

Menu* find_active_menu(Array* arr) {
    Object* curr;
    for (int i = 0; i < arr->length; i++) {
        curr = get(arr, i);
        if (curr->type == OBJECT_MENU && (curr->menu.flags & MENU_ACTIVE) > 0) {
            return &curr->menu;
        }
    }

    return NULL;
}

void select_button(ButtonElement* old, ButtonElement* new) {
    assert(old != NULL && new != NULL);

    if ((new->flags& DISABLED) > 0) {
        return;
    }

    old->flags &= ~(SELECTED | ACTIVE);
    new->flags |= (SELECTED | ACTIVE);
}

void press_button(ButtonElement* btn, u8 btn_pressed) {
    if ((btn->flags & DISABLED) > 0) {
        return;
    }

    (*btn->onPressUp)(btn, btn_pressed);
    select_button(find_active(btn), btn);
}

// Return true if event was used
bool check_button(ButtonElement* button, SDL_Event* event) {
    SDL_FPoint point;
    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP || event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        SDL_RenderCoordinatesFromWindow(app.renderer, event->button.x, event->button.y, &point.x, &point.y);
    else if (event->type == SDL_EVENT_MOUSE_MOTION)
        SDL_RenderCoordinatesFromWindow(app.renderer, event->motion.x, event->motion.y, &point.x, &point.y);

    ButtonElement* active = find_active(button);
    if (active != button && active != NULL && ((active->flags & PRESSING) > 0 && event->type == SDL_EVENT_MOUSE_BUTTON_UP))
        return false;  // Some other button is being pressed do not take this press

    // TODO || counts as pressed even if you click off it, maybe change this
    if (SDL_PointInRectFloat(&point, &button->rect) || ((button->flags & PRESSING) > 0 && event->type == SDL_EVENT_MOUSE_BUTTON_UP)) {
        if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
            press_button(button, event->button.button);
            return true;
        } else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            (*button->onPressDown)(button, event->button.button);  // TODO CHECK NOT DISABLED like press_button
            return true;
        } else if (event->type == SDL_EVENT_MOUSE_MOTION) {
            (*button->onHover)(button, true);
        }
    } else {
        if (event->type == SDL_EVENT_MOUSE_MOTION)
            (*button->onHover)(button, false);
    }

    return false;
}

// void check_GUI(SDL_Event* event) {

//     check_menu(objects, event, point);
// }

#define MOVE_DIST 48
void doInput(float elapsedMS) {
    empty(user_inputs);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                exit(0);
                break;
            // case SDL_EVENT_JOYSTICK_BUTTON_DOWN: { //TODO ADD CONTROLLER
            //     break;
            // }
            case SDL_EVENT_KEY_DOWN: {
                switch (event.key.keysym.sym) {
                    case 'w':
                        pressed |= MOVE_UP;
                        break;
                    case 's':
                        pressed |= MOVE_DOWN;
                        break;
                    case 'a':
                        pressed |= MOVE_LEFT;
                        break;
                    case 'd':
                        pressed |= MOVE_RIGHT;
                        break;

                    default:
                        add(&user_inputs, &event);
                        break;
                }
                break;
            }
            case SDL_EVENT_KEY_UP: {
                switch (event.key.keysym.sym) {
                    case SDL_SCANCODE_ESCAPE:
                        exit(0);
                        break;
                    case 'w':
                        pressed &= ~MOVE_UP;
                        break;
                    case 's':
                        pressed &= ~MOVE_DOWN;
                        break;
                    case 'a':
                        pressed &= ~MOVE_LEFT;
                        break;
                    case 'd':
                        pressed &= ~MOVE_RIGHT;
                        break;

                    default:
                        add(&user_inputs, &event);
                        break;
                }
                break;
            }

            default:
                add(&user_inputs, &event);
                break;
        }
    }

    float temp;
    if ((pressed & MOVE_LEFT) > 0) {
        temp = fmax(0, lerp(c->position.x, c->position.x - MOVE_DIST, elapsedMS));
        if (temp < SCREEN_WIDTH / 3 && c->grid_position.x > 1)
            c->grid_position.x = fmax(1, lerp(c->grid_position.x, c->grid_position.x - MOVE_DIST / GRID_SIZE, elapsedMS));
        else
            c->position.x = temp;
    }
    if ((pressed & MOVE_RIGHT) > 0) {
        temp = fmin(SCREEN_WIDTH - GRID_SIZE, lerp(c->position.x, c->position.x + MOVE_DIST, elapsedMS));
        if (temp > SCREEN_WIDTH / 3 * 2 && c->grid_position.x < map->size_x - 1)
            c->grid_position.x = fmin(map->size_x - 1, lerp(c->grid_position.x, c->grid_position.x + MOVE_DIST / GRID_SIZE, elapsedMS));
        else
            c->position.x = temp;
    }
    if ((pressed & MOVE_UP) > 0) {
        temp = fmax(0, lerp(c->position.y, c->position.y - MOVE_DIST, elapsedMS));
        if (temp < SCREEN_HEIGHT / 3 && c->grid_position.y > 1)
            c->grid_position.y = fmax(1, lerp(c->grid_position.y, c->grid_position.y - MOVE_DIST / GRID_SIZE, elapsedMS));
        else
            c->position.y = temp;
    }
    if ((pressed & MOVE_DOWN) > 0) {
        temp = fmin(SCREEN_HEIGHT - GRID_SIZE, lerp(c->position.y, c->position.y + MOVE_DIST, elapsedMS));
        if (temp > SCREEN_HEIGHT / 3 * 2 && c->grid_position.y < map->size_y - 1)
            c->grid_position.y = fmin(map->size_y - 1, lerp(c->grid_position.y, c->grid_position.y + MOVE_DIST / GRID_SIZE, elapsedMS));
        else
            c->position.y = temp;
    }
}

#define consume(i, btn)      \
    delete (user_inputs, i); \
    i--;                     \
    draw_button(btn, 0);

void check_button_input(ButtonElement* btn) {
    SDL_Event* event;
    for (int i = 0; i < user_inputs->length; i++) {
        event = get(user_inputs, i);
        switch (event->type) {
            // case SDL_EVENT_JOYSTICK_BUTTON_DOWN: { //TODO ADD CONTROLLER
            //     break;
            // }
            // case SDL_EVENT_KEY_DOWN: {
            //     switch (event->key.keysym.sym) {

            //         default:
            //             break;
            //         }

            //     break;
            // }
            case SDL_EVENT_KEY_UP: {
                switch (event->key.keysym.sym) {
                    case SDLK_RETURN: {
                        if (isActive(btn)) {
                            press_button(btn, SDL_BUTTON_LEFT);
                            consume(i, NULL);
                        }
                        break;
                    }
                        // case SDLK_ESCAPE: {
                        //     get_menu(btn);
                        //     //     hide_menu(objects, 0, true);
                        //     //     if(isActive(btn)) {
                        //     //         btn->flags &= ~(SELECTED | ACTIVE);
                        //     //     }
                        //     //     break;
                        // }

                    default:
                        break;
                }
                break;
            }
            case SDL_EVENT_KEY_DOWN: {
                switch (event->key.keysym.sym) {
                    case SDLK_RIGHT: {
                        if (isActive(btn) && get_btn_dir(btn, btn->direction.right) != NULL) {
                            select_button(btn, get_btn_dir(btn, btn->direction.right));
                            consume(i, get_btn_dir(btn, btn->direction.right));
                        }
                        break;
                    }
                    case SDLK_LEFT: {
                        if (isActive(btn) && get_btn_dir(btn, btn->direction.left) != NULL) {
                            select_button(btn, get_btn_dir(btn, btn->direction.left));
                            consume(i, get_btn_dir(btn, btn->direction.left));
                        }
                        break;
                    }
                    case SDLK_UP: {
                        if (isActive(btn) && get_btn_dir(btn, btn->direction.up) != NULL) {
                            select_button(btn, get_btn_dir(btn, btn->direction.up));
                            consume(i, get_btn_dir(btn, btn->direction.up));
                        }
                        break;
                    }
                    case SDLK_DOWN: {
                        if (isActive(btn) && get_btn_dir(btn, btn->direction.down) != NULL) {
                            select_button(btn, get_btn_dir(btn, btn->direction.down));
                            consume(i, get_btn_dir(btn, btn->direction.down));
                        }
                        break;
                    }
                    case SDLK_RETURN: {
                        if (isActive(btn)) {
                            (*btn->onPressDown)(btn, event->button.button);  // TODO CHECK NOT DISABLED like press_button
                            consume(i, NULL);
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                if (check_button(btn, event)) {
                    consume(i, NULL);
                }
                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP: {
                if (check_button(btn, event)) {
                    consume(i, NULL);
                }
                break;
            }
            case SDL_EVENT_MOUSE_MOTION: {
                check_button(btn, event);
                break;
            }

            default:
                break;
        }
    }
}