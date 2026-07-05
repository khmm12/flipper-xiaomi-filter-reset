// SPDX-License-Identifier: GPL-3.0-or-later
/**
 * @file xiaomi_filter_reset_scene_about.c
 * @brief About / help scene.
 */
#include "../xiaomi_filter_reset_i.h"

#define XIAOMI_FILTER_RESET_ABOUT_TEXT                                            \
    "\e#Xiaomi Filter Reset\n"                                                    \
    "Clears the filter-life counter on Xiaomi air purifier filter tags, so the\n" \
    "appliance reports 100% again.\n"                                             \
    "\n"                                                                          \
    "\e#How to use\n"                                                             \
    "1. Choose \"Reset filter life\".\n"                                          \
    "2. Hold the filter's NFC tag against the back of the Flipper.\n"             \
    "3. Wait for the confirmation.\n"                                             \
    "\n"                                                                          \
    "\e#How it works\n"                                                           \
    "The filter carries an NTAG213 tag. Its password is derived from the tag\n"   \
    "UID; the app authenticates and writes zeros to the usage-counter page.\n"    \
    "\n"                                                                          \
    "\e#Notes\n"                                                                  \
    "Resetting does not clean the filter. Replace or clean the media for air\n"   \
    "quality; use this only to silence a premature \"replace filter\" warning.\n" \
    "Use on filters you own.\n"                                                   \
    "\n"                                                                          \
    "\e#Credits\n"                                                                \
    "Reverse engineering: flamingo-tech and unethical.info.\n"                    \
    "\n"                                                                          \
    "License: GPL-3.0-or-later"

void xiaomi_filter_reset_scene_about_on_enter(void* context) {
    XiaomiFilterResetApp* app = context;
    Widget* widget = app->widget;

    widget_reset(widget);
    widget_add_text_scroll_element(widget, 0, 0, 128, 64, XIAOMI_FILTER_RESET_ABOUT_TEXT);
    view_dispatcher_switch_to_view(app->view_dispatcher, XiaomiFilterResetViewWidget);
}

bool xiaomi_filter_reset_scene_about_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void xiaomi_filter_reset_scene_about_on_exit(void* context) {
    XiaomiFilterResetApp* app = context;
    widget_reset(app->widget);
}
