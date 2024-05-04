#include "types.h"

PaddedElement create_p_element(int x, int y, int w, int h, int padding, u32 z_index);
UnPaddedElement create_u_element(int x, int y, int w, int h, u32 z_index);
void button_create_connections(
    Array* arr, ButtonElement* button, ButtonElement* left, ButtonElement* right, ButtonElement* up, ButtonElement* down);
ButtonElement create_button(int x, int y, int w, int h, int padding, string text, btn_press onPressDown, btn_press onPressUp, btn_hover onHover, ButtonFlags flags, u32 z_index);
void draw_button(ButtonElement* btn, MenuEnum flags);
void draw_construct(Construct* construct);
// void set_active(Array* arr);
Construct create_construct(u32 x, u32 y, u32 w, u32 h, u32 size, u32 z_index, SDL_Texture* texture);
// void hide_menu(Array* menu, u32 id, bool hide);
Menu create_menu(u32 id, u32 z_index, u32 length, u32 textures_length, u32 buttons_length, event_fn onEvent, MenuEnum flags);
TextBox create_textbox(text_call toDraw, SDL_FRect rect, SDL_Color color, TextRenderFlags flags, u32 z_index);
void create_GUI();
ButtonElement* get_btn_dir(ButtonElement* btn, s32 direction);
void destroy_objects(Array* objs);
// void destroy_GUI(GUI* gui);
// void draw_GUI(GUI* gui);
#pragma once