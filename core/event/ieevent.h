#ifndef IE_CORE_EVENT
#define IE_CORE_EVENT

#include "core/iecoredefinitions.h"

#include <inttypes.h>

typedef uint16_t ie_keycode_t;

typedef enum {
    IE_KEY_ESC = 9,
    IE_KEY_1 = 10,
    IE_KEY_2 = 11,
    IE_KEY_3 = 12,
    IE_KEY_4 = 13,
    IE_KEY_5 = 14,
    IE_KEY_6 = 15,
    IE_KEY_7 = 16,
    IE_KEY_8 = 17,
    IE_KEY_9 = 18,
    IE_KEY_0 = 19,
    IE_KEY_DASH = 20,
    IE_KEY_EQUAL = 21,
    IE_KEY_BACKSPACE = 22,
    IE_KEY_TAB = 23,
    IE_KEY_Q = 24,
    IE_KEY_W = 25,
    IE_KEY_E = 26,
    IE_KEY_R = 27,
    IE_KEY_T = 28,
    IE_KEY_Y = 29,
    IE_KEY_U = 30,
    IE_KEY_I = 31,
    IE_KEY_O = 32,
    IE_KEY_P = 33,
    IE_KEY_BRACKET_LEFT = 34,
    IE_KEY_BRACKET_RIGHT = 35,
    IE_KEY_ENTER = 36,
    IE_KEY_CTRL_LEFT = 37,
    IE_KEY_A = 38,
    IE_KEY_S = 39,
    IE_KEY_D = 40,
    IE_KEY_F = 41,
    IE_KEY_G = 42,
    IE_KEY_H = 43,
    IE_KEY_J = 44,
    IE_KEY_K = 45,
    IE_KEY_L = 46,
    IE_KEY_SEMICOLON = 47,
    IE_KEY_APOSTROPHE = 48,
    IE_KEY_BACKTICK = 49,
    IE_KEY_SHIFT_LEFT = 50, 
    IE_KEY_BACKSLASH = 51,
    IE_KEY_Z = 52,
    IE_KEY_X = 53,
    IE_KEY_C = 54,
    IE_KEY_V = 55,
    IE_KEY_B = 56,
    IE_KEY_N = 57,
    IE_KEY_M = 58,
    IE_KEY_COMMA = 59,
    IE_KEY_PERIOD = 60,
    IE_KEY_SLASH = 61,
    IE_KEY_SHIFT_RIGHT = 62,
    IE_KEY_NUMPAD_MULTIPLY = 63,
    IE_KEY_ALT_LEFT = 64,
    IE_KEY_SPACE = 65,
    IE_KEY_CAPSLOCK = 66,
    IE_KEY_F1 = 67,
    IE_KEY_F2 = 68,
    IE_KEY_F3 = 69,
    IE_KEY_F4 = 70,
    IE_KEY_F5 = 71,
    IE_KEY_F6 = 72,
    IE_KEY_F7 = 73,
    IE_KEY_F8 = 74,
    IE_KEY_F9 = 75,
    IE_KEY_F10 = 76,
    IE_KEY_NUMLOCK = 77,
    IE_KEY_SCROLLLOCK = 78,
    IE_KEY_NUMPAD_7 = 79,
    IE_KEY_NUMPAD_8 = 80,
    IE_KEY_NUMPAD_9 = 81,
    IE_KEY_NUMPAD_SUBSTRACT = 82,
    IE_KEY_NUMPAD_4 = 83,
    IE_KEY_NUMPAD_5 = 84,
    IE_KEY_NUMPAD_6 = 85,
    IE_KEY_NUMPAD_ADD = 86,
    IE_KEY_NUMPAD_1 = 87,
    IE_KEY_NUMPAD_2 = 88,
    IE_KEY_NUMPAD_3 = 89,
    IE_KEY_NUMPAD_0 = 90,
    IE_KEY_NUMPAD_DOT = 91,
    IE_KEY_NUMPAD_ENTER = 104,
    IE_KEY_CTRL_RIGHT = 105,
    IE_KEY_NUMPAD_DIVIDE = 106,
    IE_KEY_SYSRQ = 107,
    IE_KEY_ALT_RIGHT = 108,
    IE_KEY_HOME = 110,
    IE_KEY_ARROW_UP = 111,
    IE_KEY_PAGEUP = 112,
    IE_KEY_ARROW_LEFT = 113,
    IE_KEY_ARROW_RIGHT = 114,
    IE_KEY_END = 115,
    IE_KEY_ARROW_DOWN = 116,
    IE_KEY_PAGEDOWN = 117,
    IE_KEY_INSERT = 118,
    IE_KEY_DELETE = 119,
    IE_KEY_META_LEFT = 133,
    IE_KEY_META_RIGHT = 134,
    IE_KEY_MENU = 135
} ie_keycode;

typedef enum {
    IE_EVENT_WINDOW,
    IE_EVENT_WINDOW_MOUSE,
    IE_EVENT_MOUSE_BUTTON,
    IE_EVENT_KEY
} ie_event_type;

typedef enum {
    IE_EVENT_WINDOW_CLOSE,
    IE_EVENT_WINDOW_EXPOSE
} ie_event_window_type;

typedef enum {
    IE_MOUSE_BUTTON_LEFT,
    IE_MOUSE_BUTTON_MIDDLE,
    IE_MOUSE_BUTTON_RIGHT,
    IE_MOUSE_BUTTON_SCROLL_UP,
    IE_MOUSE_BUTTON_SCROLL_DOWN
} ie_event_pointer_button_type;

typedef enum {
    IE_MODIFIER_SHIFT = 1,
    IE_MODIFIER_LOCK =  2,
    IE_MODIFIER_CTRL =  4,
    IE_MODIFIER_ALT =   8,
    IE_MODIFIER_MOD2 =  16,
    IE_MODIFIER_MOD3 =  32,
    IE_MODIFIER_MOD4 =  64,
    IE_MODIFIER_MOD5 =  128,
    IE_MODIFIER_BUTTON_LEFT =   256,
    IE_MODIFIER_BUTTON_MIDDLE = 512,
    IE_MODIFIER_BUTTON_RIGHT =  1024,
    IE_MODIFIER_BUTTON_SCROLL_UP = 2048,
    IE_MODIFIER_BUTTON_SCROLL_DOWN = 4096,
    IE_MODIFIER_ANY = 32768
} ie_event_mouse_modifier_mask;

typedef struct {
    ie_event_type type;
} ie_event_generic;

typedef struct {
    ie_event_type type; 
    ie_event_window_type window_type;
} ie_event_window;

typedef struct {
    ie_event_type type;
    int           entered;
    int16_t       window_x;
    int16_t       window_y;
} ie_event_window_mouse;

typedef struct {
    ie_event_type type;
    int           pressed;
    uint8_t       button;
    int16_t       window_x;
    int16_t       window_y;
    uint16_t      modifier;
} ie_event_mouse_button;

typedef struct {
    ie_event_type type;
    int           pressed;
    ie_keycode_t  key;
    uint16_t      modifier;
} ie_event_key;

void ie_core_event_startup(void);
void ie_core_event_shutdown(void);

ie_event_generic* ie_core_event_get(void);

#endif
