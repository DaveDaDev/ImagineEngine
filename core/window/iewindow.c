#include "iewindow.h"

#include "../log/ielog.h"
#include "../loop/ieloop.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>

#define XCB_STRUCTURE_PADDING 0
#define XCB_CONNECTION_OK 0

xcb_connection_t* xcb_connection;
int               xcb_screen_count;
xcb_screen_t*     xcb_screen;
xcb_atom_t        xcb_atom_wm_delete_window;
xcb_atom_t        xcb_atom_wm_protocols;
xcb_gcontext_t    xcb_gcid;

void ie_core_window_startup(void)
{
    INFO("Core Window Startup") 
    
    xcb_connection = xcb_connect(NULL, &xcb_screen_count); 
    int connection_status = xcb_connection_has_error(xcb_connection);
    if (connection_status != XCB_CONNECTION_OK) 
        ERROR("XCB Connection to XServer Failed")

    const xcb_setup_t* setup = xcb_get_setup(xcb_connection);
    xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(setup);

    int i;
    for (i = 0; i < xcb_screen_count; ++i)
        xcb_screen_next(&screen_iterator);

    xcb_screen = screen_iterator.data;

    xcb_intern_atom_cookie_t cookie_delete_window_atom_request = xcb_intern_atom(xcb_connection,
                                                                               XCB_STRUCTURE_PADDING,
                                                                               strlen("WM_DELETE_WINDOW"),
                                                                               "WM_DELETE_WINDOW");

    xcb_intern_atom_cookie_t cookie_protocol_atom_request = xcb_intern_atom(xcb_connection,
                                                                            XCB_STRUCTURE_PADDING,
                                                                            strlen("WM_PROTOCOLS"),
                                                                            "WM_PROTOCOLS");

    xcb_intern_atom_reply_t* delete_window_atom_reply = xcb_intern_atom_reply(xcb_connection,
                                                                              cookie_delete_window_atom_request,
                                                                              NULL);

    xcb_intern_atom_reply_t* protocol_atom_reply = xcb_intern_atom_reply(xcb_connection,
                                                                         cookie_protocol_atom_request,
                                                                         NULL);
    xcb_atom_wm_delete_window = delete_window_atom_reply->atom;
    xcb_atom_wm_protocols = protocol_atom_reply->atom;
}


void ie_core_window_shutdown(void)
{
    INFO("Core Window Shutdown")

    xcb_disconnect(xcb_connection);
}


ie_window* ie_window_create(int16_t position_x, int16_t position_y, uint16_t width, uint16_t height, uint16_t boarder, char* title)
{
    ie_window* new_window = (ie_window*) malloc(sizeof(ie_window));

    uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t value_list[] = {xcb_screen->black_pixel, 
                             XCB_EVENT_MASK_EXPOSURE |
                             XCB_EVENT_MASK_BUTTON_PRESS |
                             XCB_EVENT_MASK_BUTTON_RELEASE |
                             XCB_EVENT_MASK_POINTER_MOTION |
                             XCB_EVENT_MASK_ENTER_WINDOW |
                             XCB_EVENT_MASK_LEAVE_WINDOW |
                             XCB_EVENT_MASK_KEY_PRESS |
                             XCB_EVENT_MASK_KEY_RELEASE |
                             XCB_EVENT_MASK_PROPERTY_CHANGE};

    new_window->xcb_window = xcb_generate_id(xcb_connection);
    xcb_create_window(xcb_connection,
                      XCB_COPY_FROM_PARENT,
                      new_window->xcb_window,
                      xcb_screen->root,
                      position_x, position_y,
                      width, height,
                      boarder,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      xcb_screen->root_visual,
                      value_mask,
                      value_list);

    xcb_change_property(xcb_connection,
                        XCB_PROP_MODE_REPLACE,
                        new_window->xcb_window,
                        XCB_ATOM_WM_NAME,
                        XCB_ATOM_STRING,
                        8,
                        strlen(title),
                        title);

    xcb_change_property(xcb_connection,
                        XCB_PROP_MODE_REPLACE,
                        new_window->xcb_window,
                        xcb_atom_wm_protocols,
                        XCB_ATOM_ATOM,
                        32,
                        1,
                        &xcb_atom_wm_delete_window);

    xcb_gcid = xcb_generate_id(xcb_connection); 

    uint32_t gc_value_mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    uint32_t gc_value_list[2] = {xcb_screen->white_pixel, 0};  

    xcb_create_gc(xcb_connection, xcb_gcid, new_window->xcb_window, gc_value_mask, gc_value_list);

    xcb_map_window(xcb_connection, new_window->xcb_window);
    xcb_flush(xcb_connection);

    return new_window;
}
