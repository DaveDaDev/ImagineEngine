#ifndef IE_CORE_DRAW_PRIMITIVE
#define IE_CORE_DRAW_PRIMITIVE

#include "../window/iewindow.h"

#include <inttypes.h>

void ie_core_window_clear(ie_window window);

void ie_core_draw_rectangle(ie_window* window,
                            int16_t position_x, int16_t position_y,
                            uint16_t width, uint16_t height);

#endif
