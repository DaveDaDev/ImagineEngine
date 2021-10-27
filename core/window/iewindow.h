#ifndef IE_CORE_WINDOW
#define IE_CORE_WINDOW

#include <xcb/xcb.h>

typedef struct {
    xcb_window_t xcb_window;
} ie_window;


void ie_core_window_startup(void);
void ie_core_window_shutdown(void);

ie_window* ie_window_create(int16_t position_x, int16_t position_y, uint16_t width, uint16_t height, uint16_t boarder, char* title);

#endif
