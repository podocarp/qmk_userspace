/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    define LAYER_POINTER 7
#    include "timer.h"

static uint16_t auto_pointer_layer_timer = 0;
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

enum custom_keycodes {
    MACRO_GRAVE = SAFE_RANGE,
    MACRO_QUOTES,
    MACRO_TMUX_NEXT,
    MACRO_TMUX_PREV,
};

#include "layout.h"

enum combo_events {
    COMBO_EVENT_BRACES,
    COMBO_EVENT_BRACES_SEMI,
    COMBO_EVENT_BRACKETS,
    COMBO_EVENT_SQ_BRACKETS,
    COMBO_EVENT_SQ_BRACKETS_SEMI,
    COMBO_EVENT_ANG_BRACKETS,
};

const uint16_t PROGMEM braces_combo[]          = {KC_LCBR, KC_RCBR, COMBO_END};
const uint16_t PROGMEM braces_semi_combo[]     = {KC_LCBR, KC_RCBR, KC_RBRC, COMBO_END};
const uint16_t PROGMEM brackets_combo[]        = {KC_LPRN, KC_RPRN, COMBO_END};
const uint16_t PROGMEM sqbrackets_combo[]      = {KC_LBRC, KC_RBRC, COMBO_END};
const uint16_t PROGMEM sqbrackets_semi_combo[] = {KC_LBRC, KC_RBRC, KC_LCBR, COMBO_END};
const uint16_t PROGMEM angbrackets_combo[]     = {KC_LABK, KC_RABK, COMBO_END};

combo_t key_combos[] = {
    [COMBO_EVENT_BRACES]           = COMBO_ACTION(braces_combo),          //
    [COMBO_EVENT_BRACES_SEMI]      = COMBO_ACTION(braces_semi_combo),     //
    [COMBO_EVENT_BRACKETS]         = COMBO_ACTION(brackets_combo),        //
    [COMBO_EVENT_SQ_BRACKETS]      = COMBO_ACTION(sqbrackets_combo),      //
    [COMBO_EVENT_SQ_BRACKETS_SEMI] = COMBO_ACTION(sqbrackets_semi_combo), //
    [COMBO_EVENT_ANG_BRACKETS]     = COMBO_ACTION(angbrackets_combo),     //
};

void process_combo_event(uint16_t combo_index, bool pressed) {
    switch (combo_index) {
        case COMBO_EVENT_BRACES:
            if (pressed) {
                // O is for vim, in other apps this wouldn't do anything
                // sensible
                SEND_STRING("{}" SS_TAP(X_LEFT) SS_TAP(X_ENTER) SS_TAP(X_ESC) "O");
            }
            break;
        case COMBO_EVENT_BRACES_SEMI:
            if (pressed) {
                SEND_STRING("{};" SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_ENTER) SS_TAP(X_ESC) "O");
            }
            break;
        case COMBO_EVENT_BRACKETS:
            if (pressed) {
                SEND_STRING("()" SS_TAP(X_LEFT));
            }
            break;
        case COMBO_EVENT_SQ_BRACKETS:
            if (pressed) {
                SEND_STRING("[]" SS_TAP(X_LEFT));
            }
            break;
        case COMBO_EVENT_SQ_BRACKETS_SEMI:
            if (pressed) {
                SEND_STRING("[];" SS_TAP(X_LEFT) SS_TAP(X_LEFT));
            }
            break;
        case COMBO_EVENT_ANG_BRACKETS:
            if (pressed) {
                SEND_STRING("<>" SS_TAP(X_LEFT));
            }
            break;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MACRO_GRAVE:
            if (record->event.pressed) {
                SEND_STRING("``" SS_TAP(X_LEFT));
                break;
            }
        case MACRO_QUOTES:
            if (record->event.pressed) {
                if (get_mods() & MOD_MASK_SHIFT) {
                    SEND_STRING("\"\"" SS_TAP(X_LEFT));
                } else {
                    SEND_STRING("''" SS_TAP(X_LEFT));
                }
                break;
            }
        case MACRO_TMUX_PREV:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTL) SS_TAP(X_SPACE) SS_UP(X_LCTL) "p");
                break;
            }
        case MACRO_TMUX_NEXT:
            if (record->event.pressed) {
                SEND_STRING(SS_DOWN(X_LCTL) SS_TAP(X_SPACE) SS_UP(X_LCTL) "n");
                break;
            }
    }
    return true;
}

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#endif     // POINTING_DEVICE_ENABLE
