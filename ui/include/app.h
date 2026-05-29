#ifndef APP_H
#define APP_H


#include <cstdint>

// gtk
#include <gtk/gtk.h>

// original
#include "keys.h"


struct AppState {
    uint32_t     text = 0;
    KeyMode      mode   = MODE_HIRAGANA;
    bool         ime_on = true;
    bool         pressing   = false;
    int          press_row  = -1;
    int          press_col  = -1;
    double       press_x    = 0;
    double       press_y    = 0;
    double       cur_x      = 0;
    double       cur_y      = 0;
    int          hover_row  = -1;
    int          hover_col  = -1;
    guint        press_timer = 0;
    bool         show_candidates  = false;
    GtkWidget   *drawing_area = nullptr;
};

inline AppState app;

#endif
