#include <cmath>

// pango
#include <pango/pangocairo.h>

// original
#include "ui.h"
#include "app.h"
#include "keys.h"
#include "flick.h"
#include "uinput_dev.h"


static bool isMergedEnter(int row, int col) {
    return col == COLS - 1 && (row == ROWS - 2 || row == ROWS - 1);
}

static bool isInKey(double ex, double ey, int row, int col) {
    double kx = col * KEY_W;
    double ky = row * KEY_H + TOP_OFFSET;
    double kh = (row == ROWS - 2 && col == COLS - 1) ? 2 * KEY_H : KEY_H;
    return ex >= kx + 3 && ex <= kx + KEY_W - 3 &&
           ey >= ky + 3 && ey <= ky + kh - 3;
}

static void roundedRect(cairo_t *cr, double x, double y, double w, double h, double r) {
    cairo_move_to(cr, x + r, y);
    cairo_line_to(cr, x + w - r, y);
    cairo_arc(cr, x + w - r, y + r, r, -M_PI/2, 0);
    cairo_line_to(cr, x + w, y + h - r);
    cairo_arc(cr, x + w - r, y + h - r, r, 0, M_PI/2);
    cairo_line_to(cr, x + r, y + h);
    cairo_arc(cr, x + r, y + h - r, r, M_PI/2, M_PI);
    cairo_line_to(cr, x, y + r);
    cairo_arc(cr, x + r, y + r, r, M_PI, 3*M_PI/2);
    cairo_close_path(cr);
}

static void draw123Key(cairo_t *cr, const KeyDef& key, double x, double y, double box_w, double box_h) {
    const double sw = box_w / 3.0;
    const double sh = box_h / 3.0;
    struct { FlickDir dir; double col; double row; int fs; } slots[] = {
        { CENTER, 1   , 0.7, 16 },
        { LEFT,   0.55, 1.7, 10 },
        { UP,     1   , 1.7, 10 },
        { RIGHT,  1.45, 1.7, 10 },
    };
    for (auto& s : slots) {
        const char *ch = key.chars[s.dir];
        if (!ch) continue;
        pangoDrawText(cr, ch, x + s.col * sw, y + s.row * sh, sw, sh, s.fs);
    }
}

static void drawModifyKey(cairo_t *cr, double x, double y) {
    double half_h = KEY_H / 2.0;
    pangoDrawText(cr, "゛  ゜", x + 4, y + 8, KEY_W, half_h, 15);
    pangoDrawText(cr, "小", x, y + half_h - 8, KEY_W, half_h, 11);
}

static void drawCandidateOutput(cairo_t *cr, int row, int col, const KeyDef& key, FlickDir live_dir) {
    double W = KEY_W - 6 , H = KEY_H - 6, r = 8;

    struct { FlickDir dir; int dr; int dc; } positions[] = {
        { UP,     -1,  0 },
        { LEFT,    0, -1 },
        { CENTER,  0,  0 },
        { RIGHT,   0,  1 },
        { DOWN,    1,  0 },
    };

    for (auto& p : positions) {
        const char *ch = key.chars[p.dir];
        if (!ch || (unsigned char)ch[0] < 0x20) continue;
        bool selected = (p.dir == live_dir);
        double bx = (col + p.dc) * KEY_W + 3;
        double by = (row + p.dr) * KEY_H + 3 + TOP_OFFSET;

        if (selected) {
            cairo_set_source_rgb(cr, 0.35, 0.55, 0.95);
        } else {
            cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
        }

        if (p.dir == CENTER) {
            cairo_new_path(cr);
            cairo_move_to(cr, bx, by);
            cairo_line_to(cr, bx + W, by);
            cairo_line_to(cr, bx + W, by + H);
            cairo_line_to(cr, bx, by + H);
            cairo_line_to(cr, bx, by);
            cairo_close_path(cr);
            cairo_fill(cr);
        } else if (p.dir == UP) {
            cairo_new_path(cr);
            cairo_move_to(cr, bx + r, by);
            cairo_line_to(cr, bx + W - r, by);
            cairo_arc(cr, bx + W - r, by + r, r, -M_PI / 2, 0);
            cairo_line_to(cr, bx + W, by + H + 6);
            cairo_line_to(cr, bx, by + H + 6);
            cairo_line_to(cr, bx, by + r);
            cairo_arc(cr, bx + r, by + r, r, M_PI, 3*M_PI/2);
            cairo_close_path(cr);
            cairo_fill(cr);
        } else if (p.dir == LEFT) {
            cairo_new_path(cr);
            cairo_move_to(cr, bx + r, by);
            cairo_line_to(cr, bx + W + 6, by);
            cairo_line_to(cr, bx + W + 6, by + H);
            cairo_line_to(cr, bx + r, by + H);
            cairo_arc(cr, bx + r, by + H - r, r, M_PI/2, M_PI);
            cairo_line_to(cr, bx, by - r);
            cairo_arc(cr, bx + r, by + r, r, M_PI, 3*M_PI/2);
            cairo_close_path(cr);
            cairo_fill(cr);
        } else if (p.dir == RIGHT) {
            cairo_new_path(cr);
            cairo_move_to(cr, bx - 6, by);
            cairo_line_to(cr, bx + W - r, by);
            cairo_arc(cr, bx + W - r, by + r, r, -M_PI/2, 0);
            cairo_line_to(cr, bx + W, by + H - r);
            cairo_arc(cr, bx + W - r, by + H - r, r, 0, M_PI/2);
            cairo_line_to(cr, bx - 6, by + H);
            cairo_line_to(cr, bx - 6, by);
            cairo_close_path(cr);
            cairo_fill(cr);
        } else if (p.dir == DOWN) {
            cairo_new_path(cr);
            cairo_move_to(cr, bx, by - 6);
            cairo_line_to(cr, bx + W, by - 6);
            cairo_line_to(cr, bx + W, by + H - r);
            cairo_arc(cr, bx + W - r, by + H - r, r, 0, M_PI/2);
            cairo_line_to(cr, bx + r, by + H);
            cairo_arc(cr, bx + r, by + H - r, r,  M_PI/2, M_PI);
            cairo_line_to(cr, bx, by - 6);
            cairo_close_path(cr);
            cairo_fill(cr);
        }

        cairo_set_source_rgb(cr, 0, 0, 0);
        pangoDrawText(cr, ch, bx, by, W, H, key.font_size);
    }
}

static void drawCallout(cairo_t *cr, double kx, double ky, KeyDef key, FlickDir d) {
    if (d == CENTER) return;

    const double pw = KEY_W - 3, ph = KEY_H - 3, pr = 12, pt = 10;
    double px, py;
    if (d == UP) {
        px = kx - pw/2;
        py = ky - ph/2 - ph + 3;
    }
    else if (d == DOWN) {
        px = kx - pw/2;
        py = ky + KEY_H/2 - 3;
    }
    else if (d == RIGHT) {
        px = kx + KEY_W/2;
        py = ky - ph/2;
    }
    else if (d == LEFT) {
        px = kx - KEY_W/2 - pw;
        py = ky - ph/2;
    }

    cairo_new_path(cr);
    switch (d) {
        case UP:
            cairo_move_to(cr, px + pr, py);
            cairo_line_to(cr, px + pw - pr, py);
            cairo_arc(cr, px + pw - pr, py + pr, pr, -M_PI/2, 0);
            cairo_line_to(cr, px + pw, py + ph - pr);
            cairo_line_to(cr, px + pw/2, py + ph + pt);
            cairo_line_to(cr, px, py + ph - pr);
            cairo_line_to(cr, px, py + pr);
            cairo_arc(cr, px + pr, py + pr, pr, M_PI, 3*M_PI/2);
            break;
        case DOWN:
            cairo_move_to(cr, px, py + pr);
            cairo_line_to(cr, px + pw/2, py - pt);
            cairo_line_to(cr, px + pw, py + pr);
            cairo_line_to(cr, px + pw, py + ph - pr);
            cairo_arc(cr, px + pw - pr, py + ph - pr, pr, 0, M_PI/2);
            cairo_line_to(cr, px + pr, py + ph);
            cairo_arc(cr, px + pr, py + ph - pr, pr,  M_PI/2, M_PI);
            cairo_line_to(cr, px, py + pr);
            break;
        case RIGHT:
            cairo_move_to(cr, px + pr, py);
            cairo_line_to(cr, px + pw - pr, py);
            cairo_arc(cr, px + pw - pr, py + pr, pr, -M_PI/2, 0);
            cairo_line_to(cr, px + pw, py + ph - pr);
            cairo_arc(cr, px + pw - pr, py + ph - pr, pr, 0, M_PI/2);
            cairo_line_to(cr, px + pr, py + ph);
            cairo_line_to(cr, px - pt, py + ph/2);
            cairo_line_to(cr, px + pr, py);
            break;
        case LEFT:
            cairo_move_to(cr, px + pr, py);
            cairo_line_to(cr, px + pw - pr, py);
            cairo_line_to(cr, px + pw + pt, py + ph/2);
            cairo_line_to(cr, px + pw - pr, py + ph);
            cairo_line_to(cr, px + pr, py + ph);
            cairo_arc(cr, px + pr, py + ph - pr, pr, M_PI/2, M_PI);
            cairo_line_to(cr, px, py + pr);
            cairo_arc(cr, px + pr, py + pr, pr, M_PI, 3*M_PI/2);
            break;
        default: break;
    }
    cairo_close_path(cr);

    cairo_set_source_rgb(cr, 0.7, 0.7, 0.7);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    pangoDrawText(cr, key.chars[d], px, py, pw, ph, key.font_size);
}

void pangoDrawText(cairo_t *cr, const char *text, double x, double y,
                   double box_w, double box_h, int font_size, bool center) {
    PangoLayout *layout = pango_cairo_create_layout(cr);
    char font_desc_str[32];
    snprintf(font_desc_str, sizeof(font_desc_str), "Sans %d", font_size);
    PangoFontDescription *fd = pango_font_description_from_string(font_desc_str);
    pango_layout_set_font_description(layout, fd);
    pango_font_description_free(fd);
    pango_layout_set_text(layout, text, -1);

    if (center) {
        int tw, th;
        pango_layout_get_pixel_size(layout, &tw, &th);
        cairo_move_to(cr, x + (box_w - tw) / 2.0, y + (box_h - th) / 2.0);
    } else {
        cairo_move_to(cr, x, y);
    }
    pango_cairo_show_layout(cr, layout);
    g_object_unref(layout);
}

static gboolean onLongPressTimeout(gpointer data) {
    app.press_timer = 0;
    const KeyDef& key = keysForMode(app.mode)[app.press_row][app.press_col];
    bool isHenkanKey = (app.mode == MODE_HIRAGANA && key.type == SPACE && app.text != 0);
    if (key.type == NORMAL || isHenkanKey) {
        app.show_candidates = true;
        gtk_widget_queue_draw(GTK_WIDGET(data));
    }
    return G_SOURCE_REMOVE;
}

gboolean onButtonPress(GtkWidget *widget, GdkEventButton *event, gpointer) {
    if (event->button != 1) return FALSE;
    int col = (int)(event->x / KEY_W);
    int row = (int)((event->y - TOP_OFFSET) / KEY_H);
    if (isMergedEnter(row, col)) row = ROWS - 2;
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return FALSE;
    if (!isInKey(event->x, event->y, row, col)) return FALSE;

    if (app.press_timer) {
        g_source_remove(app.press_timer);
        app.press_timer = 0;
    }
    app.pressing        = true;
    app.press_row       = row;
    app.press_col       = col;
    app.press_x         = event->x;
    app.press_y         = event->y;
    app.cur_x           = event->x;
    app.cur_y           = event->y;
    app.show_candidates = false;
    app.hover_row       = -1;
    app.hover_col       = -1;
    app.press_timer = g_timeout_add(400, onLongPressTimeout, widget);
    gtk_widget_queue_draw(widget);
    return TRUE;
}

gboolean onMotion(GtkWidget *widget, GdkEventMotion *event, gpointer) {
    if (!app.pressing) {
        int col = (int)(event->x / KEY_W);
        int row = (int)((event->y - TOP_OFFSET) / KEY_H);
        if (isMergedEnter(row, col)) row = ROWS - 2;
        int new_hover_row = (row >= 0 && row < ROWS && col >= 0 && col < COLS
                            && isInKey(event->x, event->y, row, col))
                            ? row : -1;
        int new_hover_col = (new_hover_row >= 0) ? col : -1;
        if (new_hover_row != app.hover_row || new_hover_col != app.hover_col) {
            app.hover_row = new_hover_row;
            app.hover_col = new_hover_col;
            gtk_widget_queue_draw(widget);
        }
        return FALSE;
    }
    app.cur_x = event->x;
    app.cur_y = event->y;
    if (app.press_timer) {
        double dx = event->x - app.press_x;
        double dy = event->y - app.press_y;
        if (getDirection(dx, dy) != CENTER) {
            g_source_remove(app.press_timer);
            app.press_timer = 0;
        }
    }
    gtk_widget_queue_draw(widget);
    return TRUE;
}

gboolean onLeaveNotify(GtkWidget *widget, GdkEventCrossing*, gpointer) {
    app.hover_row = -1;
    app.hover_col = -1;
    gtk_widget_queue_draw(widget);
    return FALSE;
}

gboolean onButtonRelease(GtkWidget *widget, GdkEventButton *event, gpointer) {
    if (!app.pressing || event->button != 1) return FALSE;
    if (app.press_timer) {
        g_source_remove(app.press_timer);
        app.press_timer = 0;
    }
    double dx = event->x - app.press_x;
    double dy = event->y - app.press_y;
    applyKey(app.press_row, app.press_col, getDirection(dx, dy));
    app.pressing        = false;
    app.press_row       = -1;
    app.press_col       = -1;
    app.show_candidates = false;
    gtk_widget_queue_draw(widget);
    return TRUE;
}

gboolean onDraw(GtkWidget*, cairo_t *cr, gpointer) {
    cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);
    cairo_paint(cr);

    FlickDir live_dir = CENTER;
    if (app.pressing) {
        live_dir = getDirection(app.cur_x - app.press_x,
                                app.cur_y - app.press_y);
    }

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            if (r == ROWS - 1 && c == COLS - 1) continue;

            double x = c * KEY_W;
            double y = r * KEY_H + TOP_OFFSET;
            bool merged = (r == ROWS - 2 && c == COLS - 1);
            bool pressed = (app.pressing && (app.press_row == r) && (app.press_col == c));
            bool hovered = (app.hover_row == r && app.hover_col == c);
            const KeyDef key = keysForMode(app.mode)[r][c];

            static const char* modes[] = {ACT_MODE_HIRA, ACT_MODE_ABC, ACT_MODE_123};
            bool isCurrentMode = key.chars[CENTER] && key.chars[CENTER][0] == modes[app.mode][0];
            if (pressed) {
                cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);
            } else if (isCurrentMode) {
                cairo_set_source_rgb(cr, 0.83, 0.83, 0.83);
            } else if (hovered) {
                cairo_set_source_rgb(cr, 0.88, 0.93, 1.0);
            } else if (app.show_candidates) {
                cairo_set_source_rgb(cr, 0.93, 0.93, 0.93);
            } else {
                cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
            }
            double kh = merged ? (2 * KEY_H - 6) : (KEY_H - 6);
            roundedRect(cr, x+3, y+3, KEY_W-6, kh, 10);
            cairo_fill(cr);

            if (app.show_candidates) {
                cairo_set_source_rgb(cr, 0.3, 0.3, 0.3);
            } else {
                cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
            }

            double label_h = merged ? 2 * KEY_H : KEY_H;
            if (app.mode  == MODE_123 &&
                key.type  == NORMAL   &&
                key.label != "()[]"   &&
                key.label != ".,-/"
            ) {
                draw123Key(cr, key, x, y, KEY_W, KEY_H);
            } else {
                const char *label = key.label;
                if (app.text != 0 && app.mode == MODE_HIRAGANA) {
                    if (key.type == SPACE) {
                        label = "変換";
                    } else if (key.chars[CENTER] && key.chars[CENTER][0] == ACT_MODIFY[0]) {
                        drawModifyKey(cr, x, y);
                        continue;
                    }
                }
                pangoDrawText(cr, label, x, y, KEY_W, label_h, key.font_size);
            }
        }
    }

    if (app.pressing) {
        const KeyDef key = keysForMode(app.mode)[app.press_row][app.press_col];
        if (app.show_candidates) {
            if (app.mode == MODE_HIRAGANA && key.type == SPACE && app.text != 0) {
                KeyDef tmp = key;
                tmp.chars[UP]     = "↑";
                tmp.chars[DOWN]   = "↓";
                tmp.chars[CENTER] = "変換";
                drawCandidateOutput(cr, app.press_row, app.press_col, tmp, live_dir);
            } else {
                drawCandidateOutput(cr, app.press_row, app.press_col, key, live_dir);
            }
        } else {
            double kx = app.press_col * KEY_W + KEY_W / 2.0;
            double ky = app.press_row * KEY_H + KEY_H / 2.0 + TOP_OFFSET;
            if (app.mode == MODE_HIRAGANA && key.type == SPACE && app.text != 0 &&
                (live_dir == UP || live_dir == DOWN)) {
                KeyDef tmp = key;
                tmp.chars[UP]   = "↑";
                tmp.chars[DOWN] = "↓";
                drawCallout(cr, kx, ky, tmp, live_dir);
            } else {
                const char *ch = key.chars[live_dir];
                if (ch && ch[0] != ACT_MODIFY[0]) {
                    drawCallout(cr, kx, ky, key, live_dir);
                }
            }
        }
    }

    return FALSE;
}

void onTabClicked(GtkButton*, gpointer) {
    uinputDev.sendChar(0x0009);
}

void onSpaceClicked(GtkButton*, gpointer) {
    app.text = 0;
    uinputDev.sendChar(0x0020);
}
