#include <ibus.h>
#include <glib.h>

// original
#include "ibus_client.h"


bool detectImeOn()
{
    ibus_init();

    IBusBus *bus = ibus_bus_new();
    if (!bus || !ibus_bus_is_connected(bus)) {
        if (bus) g_object_unref(bus);
        return false;
    }

    IBusInputContext *ctx = ibus_bus_create_input_context(bus, "linflick");
    if (!ctx) {
        g_object_unref(bus);
        return false;
    }

    ibus_input_context_set_capabilities(ctx, IBUS_CAP_PREEDIT_TEXT | IBUS_CAP_FOCUS);
    ibus_input_context_focus_in(ctx);
    while (g_main_context_pending(nullptr)) {
        g_main_context_iteration(nullptr, FALSE);
    }

    gboolean handled = ibus_input_context_process_key_event(ctx, IBUS_KEY_a, 0, 0);
    while (g_main_context_pending(nullptr)) {
        g_main_context_iteration(nullptr, FALSE);
    }

    if (handled) {
        ibus_input_context_process_key_event(ctx, IBUS_KEY_Escape, 0, 0);
        while (g_main_context_pending(nullptr)) {
            g_main_context_iteration(nullptr, FALSE);
        }
    }

    ibus_input_context_focus_out(ctx);
    g_object_unref(ctx);
    g_object_unref(bus);

    return handled == TRUE;
}
