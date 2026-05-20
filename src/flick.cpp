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

static void sendModifierResult(const std::string& old_text) {
    if (app.text == old_text) return;
    uinputSendBackspace();
    size_t pos = utf8LastCharStart(app.text);
    uinputSendChar(utf8ToCp(app.text, pos));
}

void applyKey(int row, int col, FlickDir dir) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return;
    const char *ch = keysForMode(app.mode)[row][col].chars[dir];
    if (!ch) return;

    std::string old_text = app.text;

    if (ch[0] == ACT_BACKSPACE[0]) {
        app.text = utf8RemoveLast(app.text);
        uinputSendBackspace();
    } else if (ch[0] == ACT_DAKUTEN[0]) {
        app.text = applyDakuten(app.text);
        sendModifierResult(old_text);
    } else if (ch[0] == ACT_HANDAKUTEN[0]) {
        app.text = applyHandakuten(app.text);
        sendModifierResult(old_text);
    } else if (ch[0] == ACT_SMALL[0]) {
        app.text = applySmall(app.text);
        sendModifierResult(old_text);
    } else if (ch[0] == ACT_MODE_HIRA[0]) {
        app.mode = MODE_HIRAGANA;
        if (!app.ime_on) {
            uinputToggleIME();
            app.ime_on = true;
        }
    } else if (ch[0] == ACT_MODE_ABC[0]) {
        app.mode = MODE_ABC;
        if (app.ime_on) {
            uinputToggleIME();
            app.ime_on = false;
        }
    } else if (ch[0] == ACT_MODE_123[0]) {
        app.mode = MODE_123;
        if (app.ime_on) {
            uinputToggleIME();
            app.ime_on = false;
        }
    } else {
        app.text = ch;
        if (!uinputSendChar(utf8ToCp(std::string(ch)))) {
            GtkClipboard *cb = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
            gtk_clipboard_set_text(cb, ch, -1);
            uinputSendPaste();
        }
    }
}
