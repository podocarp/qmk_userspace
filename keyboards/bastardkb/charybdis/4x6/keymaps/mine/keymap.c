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
#include "layout.h"

enum custom_keycodes {
    MACRO_GRAVE = SAFE_RANGE,
    MACRO_QUOTES,
};

enum combo_events {
    COMBO_EVENT_BRACES,
    COMBO_EVENT_BRACKETS,
    COMBO_EVENT_SQ_BRACKETS,
    COMBO_EVENT_ANG_BRACKETS,
};

const uint16_t PROGMEM braces_combo[]      = {KC_LCBR, KC_RCBR, COMBO_END};
const uint16_t PROGMEM brackets_combo[]    = {KC_LPRN, KC_RPRN, COMBO_END};
const uint16_t PROGMEM sqbrackets_combo[]  = {KC_LBRC, KC_RBRC, COMBO_END};
const uint16_t PROGMEM angbrackets_combo[] = {KC_LABK, KC_RABK, COMBO_END};

combo_t key_combos[] = {
    [COMBO_EVENT_BRACES]       = COMBO_ACTION(braces_combo),      //
    [COMBO_EVENT_BRACKETS]     = COMBO_ACTION(brackets_combo),    //
    [COMBO_EVENT_SQ_BRACKETS]  = COMBO_ACTION(sqbrackets_combo),  //
    [COMBO_EVENT_ANG_BRACKETS] = COMBO_ACTION(angbrackets_combo), //
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
    }
    return true;
}
