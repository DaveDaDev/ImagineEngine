#include "ieprimitive.h"

#include "../window/iewindow.h"

#include <xcb/xcb.h>

extern xcb_connection_t* xcb_connection;
extern int               xcb_screen_number;
extern xcb_screen_t*     xcb_screen;
extern xcb_gcontext_t    xcb_gcid;

void ie_core_draw_rectangle(ie_window* window,
                            int16_t position_x, int16_t position_y,
                            uint16_t width, uint16_t height)
{
    
    xcb_rectangle_t new_rectangle = {};
    new_rectangle.x = position_x;
    new_rectangle.y = position_y;
    new_rectangle.width = width;
    new_rectangle.height = height;
    
    xcb_poly_fill_rectangle(xcb_connection, window->xcb_window, xcb_gcid, 1, &new_rectangle);

    xcb_flush(xcb_connection);

}

