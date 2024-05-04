// #include "../types.h"
// #include "../gui.h"
// #include "../defs.h"
// #include "../structs.h"

// int main() {
//     float lower_third = SCREEN_HEIGHT - SCREEN_HEIGHT / 3;

//     Object object = (Object) create_u_element(0, lower_third, SCREEN_WIDTH, SCREEN_HEIGHT / 3, GUI);
//     // insert(&objects, &object, compareObjects);

//     object = (Object) create_p_element(SCREEN_WIDTH / 2, lower_third, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3, 5, GUI + 1);
//     // insert(&objects, &object, compareObjects);

//     // textures[num_textures] = IMG_LoadTexture(app.renderer, "gfx/ui/container.png");
//     // object = (Object) create_construct(0, lower_third, SCREEN_WIDTH, SCREEN_HEIGHT / 3, GRID_SIZE, GUI + 1, textures[num_textures++]);
//     // insert(&objects, &object, compareObjects);

//     // Buttons are not sorted!!
//     ButtonElement button = create_button(0, lower_third, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 6, "Attack", &test, &basic_hover, CENTER_X | CENTER_Y | SELECTED | ACTIVE, BUTTON);    
//     // ButtonElement* btn1 = add(&buttons, &button);
//     button_create_connections(btn1, -1, 2, -1, 1);
    
//     button = create_button(0, SCREEN_HEIGHT - SCREEN_HEIGHT / 6, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 6, "Equipment", &test, &basic_hover, CENTER_X | CENTER_Y, BUTTON);
//     // ButtonElement* btn2 = add(&buttons, &button);
//     button_create_connections(btn2, -1, 3, 0, -1);
    
//     button = create_button(SCREEN_WIDTH / 4, lower_third, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 6, "Magic", &test, &basic_hover, CENTER_X | CENTER_Y, BUTTON);
//     // ButtonElement* btn3 = add(&buttons, &button);
//     button_create_connections(btn3, 0, -1, -1, 3);
    
//     button = create_button(SCREEN_WIDTH / 4, SCREEN_HEIGHT - SCREEN_HEIGHT / 6, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 6, "Run", &test, &basic_hover, CENTER_X | CENTER_Y, BUTTON);
//     // ButtonElement* btn4 = add(&buttons, &button);
//     button_create_connections(btn4, 1, -1, 2, -1);


//     button = create_button(0, lower_third - 50, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 6, 6, "ASS", &test, &basic_hover, CENTER_X | CENTER_Y | SELECTED | DISABLED, BUTTON);    
//     // ButtonElement* btn5 = add(&buttons, &button);
//     button_create_connections(btn5, -1, 2, -1, 1);
// }