#include <cmath>

// original
#include "flick.h"
#include "app.h"
#include "modifier.h"
#include "utf8.h"
#include "uinput_dev.h"


FlickDir getDirection(double dx, double dy) {
    if (std::sqrt(dx*dx + dy*dy) < FLICK_THRESHOLD) return CENTER;
    if (std::abs(dx) > std::abs(dy)) return dx > 0 ? RIGHT : LEFT;
    return dy < 0 ? UP : DOWN;
}

void applyKey(int row, int col, FlickDir dir) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return;
    const char *ch = keysForMode(app.mode)[row][col].chars[dir];
    if (!ch) return;

    uint32_t old_cp = app.text;

    if (ch[0] == ACT_BACKSPACE[0]) {
        app.text = 0;
        uinputDev.sendBackspace();
    } else if (ch[0] == ACT_MODE_HIRA[0]) {
        app.mode = MODE_HIRAGANA;
        if (!app.ime_on) {
            uinputDev.toggleIME();
            app.ime_on = true;
        }
    } else if (ch[0] == ACT_MODE_ABC[0]) {
        app.mode = MODE_ABC;
        if (app.ime_on) {
            uinputDev.toggleIME();
            app.ime_on = false;
        }
    } else if (ch[0] == ACT_MODE_123[0]) {
        app.mode = MODE_123;
        if (app.ime_on) {
            uinputDev.toggleIME();
            app.ime_on = false;
        }
    } else if (ch[0] == ACT_MODIFY[0]) {
        uint32_t new_cp = (app.mode == MODE_ABC)
            ? modifyABC(app.text)
            : modifyHIRAGANA(app.text);
        if (new_cp != app.text) {
            app.text = new_cp;
            uinputDev.sendBackspace();
            uinputDev.sendChar(app.text);
        }
    } else {
        app.text = utf8ToCp(ch);
        if (!uinputDev.sendChar(app.text)) {
            GtkClipboard *cb = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
            gtk_clipboard_set_text(cb, ch, -1);
            uinputDev.sendPaste();
        }
    }
}
