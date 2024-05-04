#include "draw.h"

#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <math.h>

#include "defs.h"
#include "gui.h"
#include "input.h"
#include "main.h"
#include "types.h"

// #include <SDL3/SDL_ttf.h>

void prepareScene(void) {
    SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
    SDL_RenderClear(app.renderer);
}

void blit(SDL_Texture* texture, float x, float y, bool round_coords) {
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);  // Get the width and height from the texture
    SDL_FRect dest = (SDL_FRect){.x = x, .y = y, .w = w, .h = h};
    blit_rect(texture, &dest, NULL, round_coords);
}

void blit_rect(SDL_Texture* texture, SDL_FRect* dest, SDL_FRect* src, bool round_coords) {
    SDL_FRect temp;
    if (round_coords) {
        temp = (SDL_FRect){.x = round(dest->x), .y = round(dest->y), .w = dest->w, .h = dest->h};
    } else {
        temp = *dest;
    }

    SDL_RenderTexture(app.renderer, texture, src, &temp);
}

float lerp(float start, float end, float time) {
    return start + time * (end - start);
}

SDL_FPoint lerp2d(SDL_FPoint start, SDL_FPoint end, float time) {
    return (SDL_FPoint){start.x + time * (end.x - start.x), start.y + time * (end.y - start.y)};
}

float diff_lerp(float a, float b, float t) {
    return t * (b - a);
}

// Texture must be destroyed
SDL_Texture* render_text(char* text, SDL_FRect* dest, TextRenderFlags flags) {
    SDL_Surface* surf = TTF_RenderText_Solid(font, text, (SDL_Color){0, 0, 0});

    if ((flags & JUSTIFY_X) > 0) {
        dest->x -= surf->w;
    }
    if ((flags & CENTER_X) > 0) {
        assert(surf->w <= dest->w);
        dest->x += floorf((dest->w - surf->w) / 2);
    }
    if ((flags & JUSTIFY_Y) > 0) {
        dest->y -= surf->h;
    }
    if ((flags & CENTER_Y) > 0) {
        assert(surf->h <= dest->h);
        dest->y += floorf((dest->h - surf->h) / 2);
    }

    dest->w = surf->w;
    dest->h = surf->h;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(app.renderer, surf);
    SDL_DestroySurface(surf);

    return texture;
}

void draw_text(SDL_Texture* texture, SDL_FRect* dest) {
    SDL_RenderTexture(app.renderer, texture, NULL, dest);
    SDL_DestroyTexture(texture);
}

void draw_render_text(char* text, SDL_FRect* dest, TextRenderFlags flags) {
    draw_text(render_text(text, dest, flags), dest);
}

void draw_textbox(TextBox* textbox) {
    draw_render_text((*textbox->toDraw)(textbox), &textbox->rect, textbox->flags);
}

void draw_button(ButtonElement* btn, MenuEnum flags) {
    if (btn == NULL)
        return;
    // Get any changes from cached events

    if ((flags & MENU_ACTIVE) > 0 || find_active_menu(objects) == NULL)  // Active menu so check inputs
        check_button_input(btn);

    if ((btn->flags & (HIDDEN | DISABLED)) > 0) {
        SDL_SetCursor(deny_cursor);
        cursor = deny_cursor;
        return;
    } else if ((btn->flags & PRESSING) > 0) {  // Pressing
        SDL_SetRenderDrawColor(app.renderer, 255, btn->hover_color.g, btn->hover_color.b, btn->hover_color.a);
        SDL_SetCursor(hand_cursor);
        cursor = hand_cursor;
    } else if ((btn->flags & (SELECTED | HOVER)) == (SELECTED | HOVER)) {  // Selected & Hovering
        SDL_SetRenderDrawColor(app.renderer, 41, 90, 225, 255);
        SDL_SetCursor(hand_cursor);
        cursor = hand_cursor;
    } else if ((btn->flags & SELECTED) > 0) {  // Selected
        SDL_SetRenderDrawColor(app.renderer, 31, 90, 184, 255);
    } else if ((btn->flags & HOVER) > 0) {  // Hovering
        SDL_SetRenderDrawColor(app.renderer, btn->hover_color.r, btn->hover_color.g, btn->hover_color.b, btn->hover_color.a);
        SDL_SetCursor(hand_cursor);
        cursor = hand_cursor;
    } else {
        SDL_SetRenderDrawColor(app.renderer, btn->color.r, btn->color.g, btn->color.b, btn->color.a);
    }
    // SDL_Cursor* cc = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);

    // SDL_DestroyCursor(cc);

    // Will get size and texture of text
    SDL_FRect text_rect = btn->rect;  // render alters the rect so make a copy
    SDL_Texture* text = render_text(btn->text, &text_rect, btn->flags);

    // // Add padding //TODO readd as an option
    // SDL_FRect btn_rect = text_rect;
    // btn_rect.x -= 3;
    // btn_rect.y -= 3;
    // btn_rect.h += 6;
    // btn_rect.w += 6;

    SDL_RenderFillRect(app.renderer, &btn->rect);
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 100);
    SDL_RenderRect(app.renderer, &btn->rect);

    draw_text(text, &text_rect);
}

void draw_construct(Construct* construct) {
    SDL_FRect dest = {construct->rect.x, construct->rect.y, construct->size, construct->size};
    SDL_FRect src = {construct->offsets[CON_TOP_LEFT].x, construct->offsets[CON_TOP_LEFT].y, construct->size, construct->size};
    blit_rect(construct->texture, &dest, &src, false);

    dest.y = construct->rect.y + construct->rect.h - construct->size;
    src = (SDL_FRect){construct->offsets[CON_BOTTOM_LEFT].x, construct->offsets[CON_BOTTOM_LEFT].y, construct->size, construct->size};
    blit_rect(construct->texture, &dest, &src, false);

    dest.x = construct->rect.x + construct->rect.w - construct->size;
    src = (SDL_FRect){construct->offsets[CON_BOTTOM_RIGHT].x, construct->offsets[CON_BOTTOM_RIGHT].y, construct->size, construct->size};
    blit_rect(construct->texture, &dest, &src, false);

    dest.y = construct->rect.y, construct->size, construct->size;
    src = (SDL_FRect){construct->offsets[CON_TOP_RIGHT].x, construct->offsets[CON_TOP_RIGHT].y, construct->size, construct->size};
    blit_rect(construct->texture, &dest, &src, false);

    int length = construct->rect.w - construct->size * 2;
    int i = 1;
    src = (SDL_FRect){construct->offsets[CON_TOP].x, construct->offsets[CON_TOP].y, construct->size, construct->size};
    SDL_FRect src2 = {construct->offsets[CON_BOTTOM].x, construct->offsets[CON_BOTTOM].y, construct->size, construct->size};
    while (length > 0) {
        dest = (SDL_FRect){construct->rect.x + i * construct->size, construct->rect.y, length > construct->size ? construct->size : length, construct->size};
        blit_rect(construct->texture, &dest, &src, false);
        dest.y = construct->rect.y + construct->rect.h - construct->size;
        blit_rect(construct->texture, &dest, &src2, false);

        i++;
        length -= construct->size;
    }

    int height = construct->rect.h - construct->size * 2;
    i = 1;
    src = (SDL_FRect){construct->offsets[CON_LEFT].x, construct->offsets[CON_LEFT].y, construct->size, construct->size};
    src2 = (SDL_FRect){construct->offsets[CON_RIGHT].x, construct->offsets[CON_RIGHT].y, construct->size, construct->size};
    while (height > 0) {
        dest = (SDL_FRect){construct->rect.x, construct->rect.y + i * construct->size, construct->size, height > construct->size ? construct->size : height};
        blit_rect(construct->texture, &dest, &src, false);
        dest.x = construct->rect.x + construct->rect.w - construct->size;
        blit_rect(construct->texture, &dest, &src2, false);

        i++;
        height -= construct->size;
    }
}

void draw_map() {
    double fract_x, fract_y;
    double intpart;
    fract_x = modf(c->grid_position.x, &intpart);
    fract_y = modf(c->grid_position.y, &intpart);

    int tile;
    for (int x = -1; x <= ceil(SCREEN_WIDTH / (float)GRID_SIZE); x++) {
        for (int y = -1; y <= ceil(SCREEN_HEIGHT / (float)GRID_SIZE); y++) {
            // blit_rect(get(textures, map->tiles[((int)floor(c->grid_position.y) + y) * map->size_x + x + (int)floor(c->grid_position.x)].id), &((SDL_FRect) {(x - fract_x) * GRID_SIZE, (y - fract_y) * GRID_SIZE, GRID_SIZE, GRID_SIZE}), NULL, true);
            tile = map->tiles[((int)floor(c->grid_position.y) + y) * map->size_x + x + (int)floor(c->grid_position.x)].id;
            blit_rect(((Texture*)get(textures, CURRENT_MAP_TEXTURE))->texture, &((SDL_FRect){(x - fract_x) * GRID_SIZE, (y - fract_y) * GRID_SIZE, GRID_SIZE, GRID_SIZE}), &((SDL_FRect){tile * GRID_SIZE, 0, GRID_SIZE, GRID_SIZE}), true);
        }
    }
}

void draw_grid() {
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    for (int x = 0; x < ceil(SCREEN_WIDTH / (float)GRID_SIZE); x++) {
        SDL_RenderLine(app.renderer, x * GRID_SIZE, 0, x * GRID_SIZE, SCREEN_HEIGHT);
    }
    for (int y = 0; y < ceil(SCREEN_HEIGHT / (float)GRID_SIZE); y++) {
        SDL_RenderLine(app.renderer, 0, y * GRID_SIZE, SCREEN_WIDTH, y * GRID_SIZE);
    }
}

void draw_grid_diagonal() {
    int num1 = floor((SCREEN_HEIGHT / 1.414213562f) / (GRID_SIZE / 1.5f));  // TODO MAKE THIS MORE ACCURATE IN COUNTING THE NUMBER OF LINES ON EITHER SIDE OF THE START POS
    int num2 = floor((SCREEN_WIDTH / 1.414213562f) / (GRID_SIZE / 1.5f));
    int b_scaled;
    for (int b = 0; b < num2; b++) {
        b_scaled = (b * GRID_SIZE);
        SDL_SetRenderDrawColor(app.renderer, b * 30, 0, 0, 255);

        if (b < num1) {
            SDL_RenderLine(app.renderer, 0, SCREEN_HEIGHT - b_scaled, SCREEN_HEIGHT - b_scaled, 0);
            SDL_RenderLine(app.renderer, -b_scaled, 0, SCREEN_HEIGHT - b_scaled, SCREEN_HEIGHT);
        }

        SDL_RenderLine(app.renderer, b_scaled, SCREEN_HEIGHT, SCREEN_HEIGHT + b_scaled, 0);
        SDL_RenderLine(app.renderer, b_scaled, 0, SCREEN_HEIGHT + b_scaled, SCREEN_HEIGHT);
    }
}

// TODO make it possible to lerp on map coords and not just screen coords

void draw_objects(Array* objs) {
    Object* object;
    for (int i = 0; i < objs->length; i++) {
        object = get(objs, i);
        switch (object->type) {
            case OBJECT_ANIMATION: {
                if (draw_animation(i, &c->position, true)) {
                    delete (objects, i);
                    i--;
                }
                break;
            }
            case OBJECT_LERP: {
                if (draw_lerp(i)) {  // object
                    delete (objects, i);
                    i--;
                }
                break;
            }
            case OBJECT_UNPADDED: {
                // UnPaddedElement element = gui->elements[i].unpadded;
                SDL_SetRenderDrawColor(app.renderer, object->unpadded.color.r, object->unpadded.color.g, object->unpadded.color.b, object->unpadded.color.a);
                SDL_RenderFillRect(app.renderer, &object->unpadded.rect);
                break;
            }
            case OBJECT_PADDED: {
                // PaddedElement element = gui->elements[i].padded;
                SDL_SetRenderDrawColor(app.renderer, object->padded.inner_color.r, object->padded.inner_color.g, object->padded.inner_color.b, object->padded.inner_color.a);
                SDL_RenderFillRect(app.renderer, &object->padded.inner);
                break;
            }
            case OBJECT_CONSTRUCT: {
                draw_construct(&object->construct);
                break;
            }
            // case OBJECT_BUTTON: {
            //     draw_button(&object->button);
            //     break;
            // }
            case OBJECT_MENU: {
                check_inputs(object, object->menu.onEvent);  // gets input even while hidden check that in the onEvent function
                if ((object->menu.flags & MENU_HIDDEN) > 0)  // TODO check if this needs to be changed to let input do stuff
                    break;
                draw_objects(object->menu.components);

                for (int j = 0; j < object->menu.buttons->length; j++) {
                    draw_button(get(object->menu.buttons, j), object->menu.flags);
                }

                break;
            }

            case OBJECT_TEXT: {
                draw_textbox(&object->textbox);
                break;
            }

            default:
                break;
        }
    }
}