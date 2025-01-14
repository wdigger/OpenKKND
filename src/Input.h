#pragma once

#pragma pack(push, 1)

#define INPUT_KEYBOARD_UP_MASK      0x1
#define INPUT_KEYBOARD_DOWN_MASK    0x2
#define INPUT_KEYBOARD_LEFT_MASK    0x4
#define INPUT_KEYBOARD_RIGHT_MASK   0x8
#define INPUT_KEYBOARD_ARROW_KEYS_MASK (INPUT_KEYBOARD_UP_MASK | INPUT_KEYBOARD_DOWN_MASK | INPUT_KEYBOARD_LEFT_MASK | INPUT_KEYBOARD_RIGHT_MASK)

#define INPUT_KEYBOARD_CONTROL_MASK 0x10
#define INPUT_KEYBOARD_MENU_MASK    0x20
#define INPUT_KEYBOARD_RETURN_MASK  0x80
#define INPUT_KEYBOARD_TAB_MASK     0x100
#define INPUT_KEYBOARD_ESCAPE_MASK  0x200

/* 378 */
struct KeyboardInput
{
    int pressed_keys_mask;
    int just_pressed_keys_mask;
    int unpressed_keys_mask;
    int field_C;
    int _10_wndproc_mapped_key;
};


#define INPUT_MOUSE_LBUTTON_MASK 0x10
#define INPUT_MOUSE_RBUTTON_MASK 0x20
#define INPUT_MOUSE_MBUTTON_MASK 0x80

/* 356 */
struct MouseInput
{
    int pressed_buttons_mask;
    int just_pressed_buttons_mask;
    int just_released_buttons_mask;
    int field_C;
    int cursor_x_x256;
    int cursor_y_x256;
    int cursor_dx_x256;
    int cursor_dy_x256;
};

#pragma pack(pop)


bool input_initialize();

void input_update_keyboard();
bool input_get_keyboard_state(KeyboardInput *state);
void input_reset_keyboard();


int input_update_mouse();
bool input_get_mouse_state(MouseInput *a1);

bool input_4283A0_set_cursor_pos(__int16 x, __int16 y);
bool input_428470(__int16 a1);



extern MouseInput _47A5E0_mouse_input; // idb;