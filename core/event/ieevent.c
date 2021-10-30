#include "ieevent.h"

#include "core/iecoredefinitions.h"
#include "core/window/iewindow.h"
#include "core/log/ielog.h"

#include <stdlib.h>
#include <string.h>
#include <xcb/xcb.h>

#include "core/debug_memory/iedebugmemory.h"


extern xcb_connection_t* xcb_connection;
extern xcb_atom_t xcb_atom_wm_delete_window;

void ie_core_event_startup(void)
{
    INFO("Core Event Startup")
}


void ie_core_event_shutdown(void)
{
    INFO("Core Event Shutdown")
}


ie_event_generic* ie_core_event_get(void)
{
    xcb_generic_event_t* xcb_event = xcb_poll_for_event(xcb_connection); 

    if (xcb_event == NULL) {
        free(xcb_event);
        return NULL;
    }

    switch(xcb_event->response_type & ~0x80) {

        case XCB_CLIENT_MESSAGE: { 
            xcb_client_message_event_t* xcb_event_client_message = (xcb_client_message_event_t*) xcb_event;
            if (xcb_event_client_message->data.data32[0] == xcb_atom_wm_delete_window) {
                INFO("Window Event: Close")
                ie_event_window* ie_event = (ie_event_window*) malloc(sizeof(ie_event_window));
                ie_event->type = IE_EVENT_WINDOW;
                ie_event->window_type = IE_EVENT_WINDOW_CLOSE;
                free(xcb_event_client_message);
                return (ie_event_generic*) ie_event;
            }
        break;}

        case XCB_EXPOSE: {
            ie_event_window* ie_event = (ie_event_window*) malloc(sizeof(ie_event_window));
            ie_event->type = IE_EVENT_WINDOW;
            ie_event->window_type = IE_EVENT_WINDOW_EXPOSE;
            free(xcb_event);
            return (ie_event_generic*) ie_event;
        break;}
        
        case XCB_BUTTON_PRESS: {
            xcb_button_press_event_t* xcb_event_button_press = (xcb_button_press_event_t*) xcb_event;
            INFO("Mouse Button Event: Pressed [%u] at (%" PRId16 ", %" PRId16 ")",
                    xcb_event_button_press->detail,
                    xcb_event_button_press->event_x,
                    xcb_event_button_press->event_y)
            ie_event_mouse_button* ie_event_mouse_press = (ie_event_mouse_button*) malloc(sizeof(ie_event_mouse_button));
            ie_event_mouse_press->type = IE_EVENT_MOUSE_BUTTON; 
            ie_event_mouse_press->pressed = IE_TRUE;
            ie_event_mouse_press->window_x = xcb_event_button_press->event_x;
            ie_event_mouse_press->window_y = xcb_event_button_press->event_y;
            ie_event_mouse_press->button = xcb_event_button_press->detail;
            ie_event_mouse_press->modifier = xcb_event_button_press->state;
            free(xcb_event_button_press);
            return (ie_event_generic*) ie_event_mouse_press;
        break;}

        case XCB_BUTTON_RELEASE: {
            xcb_button_press_event_t* xcb_event_button_release = (xcb_button_release_event_t*) xcb_event;
            INFO("Mouse Button Event: Released [%u] at (%" PRId16 ", %" PRId16 ")",
                    xcb_event_button_release->detail,
                    xcb_event_button_release->event_x,
                    xcb_event_button_release->event_y)
            ie_event_mouse_button* ie_event_mouse_release = (ie_event_mouse_button*) malloc(sizeof(ie_event_mouse_button));
            ie_event_mouse_release->type = IE_EVENT_MOUSE_BUTTON; 
            ie_event_mouse_release->pressed = IE_FALSE;
            ie_event_mouse_release->window_x = xcb_event_button_release->event_x;
            ie_event_mouse_release->window_y = xcb_event_button_release->event_y;
            ie_event_mouse_release->button = xcb_event_button_release->detail;
            ie_event_mouse_release->modifier = xcb_event_button_release->state;
            free(xcb_event_button_release);
            return (ie_event_generic*) ie_event_mouse_release;
        break;}

        case XCB_ENTER_NOTIFY: {
            xcb_enter_notify_event_t* xcb_event_enter_window = (xcb_enter_notify_event_t*) xcb_event;
            INFO("Window Mouse Event: Enter (%" PRId16 ", %" PRId16 ")",
                    xcb_event_enter_window->event_x, xcb_event_enter_window->event_y)
            ie_event_window_mouse* ie_event_enter_window = (ie_event_window_mouse*) malloc(sizeof(ie_event_window_mouse));
            ie_event_enter_window->type = IE_EVENT_WINDOW_MOUSE;
            ie_event_enter_window->entered = IE_TRUE;
            ie_event_enter_window->window_x = xcb_event_enter_window->event_x;
            ie_event_enter_window->window_y = xcb_event_enter_window->event_y;
            free(xcb_event_enter_window);
            return (ie_event_generic*) ie_event_enter_window;
        break;}

        case XCB_LEAVE_NOTIFY: {
            xcb_leave_notify_event_t* xcb_event_leave_window = (xcb_leave_notify_event_t*) xcb_event;
            INFO("Window Mouse Event: Leave (%" PRId16 ", %" PRId16 ")",
                    xcb_event_leave_window->event_x, xcb_event_leave_window->event_y)
            ie_event_window_mouse* ie_event_leave_window = (ie_event_window_mouse*) malloc(sizeof(ie_event_window_mouse));
            ie_event_leave_window->type = IE_EVENT_WINDOW_MOUSE;
            ie_event_leave_window->entered = IE_FALSE;
            ie_event_leave_window->window_x = xcb_event_leave_window->event_x;
            ie_event_leave_window->window_y = xcb_event_leave_window->event_y;
            free(xcb_event_leave_window);
            return (ie_event_generic*) ie_event_leave_window;
        break;}
        
        case XCB_KEY_PRESS: {
            xcb_key_press_event_t* xcb_event_key_press = (xcb_key_press_event_t*) xcb_event;
            INFO("Key Event: Pressed [%u]", xcb_event_key_press->detail)
            ie_event_key* ie_event_key_press = (ie_event_key*) malloc(sizeof(ie_event_key));
            ie_event_key_press->type = IE_EVENT_KEY;
            ie_event_key_press->pressed = IE_TRUE;
            ie_event_key_press->key = xcb_event_key_press->detail;
            ie_event_key_press->modifier = xcb_event_key_press->state;
            free(xcb_event_key_press);
            return (ie_event_generic*) ie_event_key_press;
        break;}

        case XCB_KEY_RELEASE: {
            xcb_key_release_event_t* xcb_event_key_release = (xcb_key_release_event_t*) xcb_event;
            INFO("Key Event: Released [%u]", xcb_event_key_release->detail)
            ie_event_key* ie_event_key_release = (ie_event_key*) malloc(sizeof(ie_event_key));
            ie_event_key_release->type = IE_EVENT_KEY;
            ie_event_key_release->pressed = IE_FALSE;
            ie_event_key_release->key = xcb_event_key_release->detail;
            ie_event_key_release->modifier = xcb_event_key_release->state;
            free(xcb_event_key_release);
            return (ie_event_generic*) ie_event_key_release;
        break;}

        default:
            //INFO("UNKNOWN EVENT")
            free(xcb_event);
            return NULL;
    }

    free(xcb_event);
    return NULL;
}
