/* Copyright 2019 Danny Nguyen <danny@keeb.io>
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
#define _CODING 0
#define _DEBUGGING 1
#define _MEDIA 2

#define START_DEBUG KC_F5
#define STOP_DEBUG LSFT(KC_F5)
#define STEP_IN KC_F11
#define STEP_OUT LSFT(KC_F11)
#define STEP_OVER KC_F10
#define COMMAND_P KC_F1

enum custom_keycodes {
    GIT,
    MOD,
    MOD2,
    MOD3
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_CODING] = LAYOUT(
        KC_LEAD, TO(_DEBUGGING), KC_SLEP, \
        GIT    ,    XXXXXXX    , XXXXXXX, \
        MOD    ,     MOD2      ,  MOD3   \
    ),
    /*
        | Knob 1: Brightness Up/Down |               | Knob 2: Vol Dn/Up  |
        |       RESET                |     MEDIA     |    Media Stop      |
        |                            | End           |                    |
        |      CTRL_END              |               |    CTRL_HOME       |
     */
    [_DEBUGGING] = LAYOUT(
        KC_COPY    , TO(_MEDIA), KC_PSTE    , \
        STEP_IN    , STEP_OVER , STEP_OUT   , \
        START_DEBUG, COMMAND_P , STOP_DEBUG  \
    ),
     /*
        | Knob 1: Brightness Up/Down |               | Knob 2: Vol Dn/Up |
        | Press: Play/Pause          |     RANDO     | Press: Mute       |
        |                            |               |                   |
        | Media Previous             |               | Media Next        |
     */
    [_MEDIA] = LAYOUT(
        KC_MPLY, TO(_CODING),   KC_MUTE, \
        XXXXXXX, XXXXXXX   ,    XXXXXXX, \
        KC_MPRV, XXXXXXX   ,    KC_MNXT \
    ),
};

void encoder_update_user(uint8_t index, bool clockwise) {
    if (IS_LAYER_ON(_DEBUGGING)) {
        if (index == 0) {
            if (clockwise) {
                tap_code(KC_PGUP);
            } else {
                tap_code(KC_PGDN);
            }
        }
        else if (index == 1) {
            if (clockwise) {
                tap_code(KC_UP);
            } else {
                tap_code(KC_DOWN);
            }
        }
    } else {
        if (index == 0) {
            if (clockwise) {
                tap_code(KC_BRIU);
            } else {
                tap_code(KC_BRID);
            }
        }
        else if (index == 1) {
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
        }
    }
}

LEADER_EXTERNS();

void matrix_scan_user(void) {
    LEADER_DICTIONARY() {
        leading = false;
        leader_end();

        SEQ_ONE_KEY(GIT) {
            SEND_STRING("git commit -m ");
        }

        SEQ_TWO_KEYS(GIT,GIT) {
            SEND_STRING("git fetch");
            register_code(KC_ENT);
            unregister_code(KC_ENT);
        }

        SEQ_THREE_KEYS(GIT,GIT,GIT) {
            SEND_STRING("git checkout origin/master");
            register_code(KC_ENT);
            unregister_code(KC_ENT);
            SEND_STRING("git checkout -B master");
            register_code(KC_ENT);
            unregister_code(KC_ENT);
        }

        SEQ_THREE_KEYS(GIT,MOD,MOD) {
            SEND_STRING("git rebase origin/master");
            register_code(KC_ENT);
            unregister_code(KC_ENT);
        }
    }
}