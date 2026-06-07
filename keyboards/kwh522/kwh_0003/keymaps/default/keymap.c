// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "keymap_japanese.h"

#include "oled_driver.h"
#include "i2c_master.h"
// For split keyboards: detect which half is the master (usually the USB-powered side)
#include "split_util.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Layer 0
     * ┌─────┬─────┬─────┬─────┬─────┬─────┐  ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
     * │ ESC │  1  │  2  │  3  │  4  │  5  │  │  6  │  7  │  8  │  9  │  0  │  -  │  ^  │  \  │ BS  │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤  ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┘
     * │ TAB │  Q  │  W  │  E  │  R  │  T  │  │  Y  │  U  │  I  │  O  │  P  │  @  │  [  │ ENT │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤  ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┘
     * │ CTL │  A  │  S  │  D  │  F  │  G  │  │  H  │  J  │  K  │  L  │  ;  │  :  │  ]  │
     * ├─────┼─────┼─────┼─────┼─────┼─────┘  ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┐
     * │ SHF │  Z  │  X  │  C  │  V  │        │  B  │  N  │  M  │  ,  │  .  │  /  │  \  │ SHF │
     * ├─────┼─────┼─────┼─────┼─────┼─────┐  ├─────┼─────┼─────┼─────┼─────┼─────┼─────┴─────┘
     * │ Fn2 │ Fn  │ ALT │ H/Z │MHEN │ SPS │  │ SPS │HENK │KANA │ ALT │ CTR │ Fn  │
     * └─────┴─────┴─────┴─────┴─────┴─────┘  └─────┴─────┴─────┴─────┴─────┴─────┘
     */
    [0] = LAYOUT(  
        KC_ESC,  JP_1,  JP_2,    JP_3,    JP_4,    JP_5,   JP_6,   JP_7,    JP_8,    JP_9,    JP_0,    JP_MINS, JP_CIRC, JP_YEN,  KC_BSPC, \
        KC_TAB,  JP_Q,  JP_W,    JP_E,    JP_R,    JP_T,   JP_Y,   JP_U,    JP_I,    JP_O,    JP_P,    JP_AT,   JP_LBRC, KC_ENT, \
        KC_LCTL, JP_A,  JP_S,    JP_D,    JP_F,    JP_G,   JP_H,   JP_J,    JP_K,    JP_L,    JP_SCLN, JP_COLN, JP_RBRC, \
        KC_LSFT, JP_Z,  JP_X,    JP_C,    JP_V,            JP_B,   JP_N,    JP_M,    JP_COMM, JP_DOT,  JP_SLSH, JP_BSLS, KC_RSFT, \
        MO(2),   TT(1), KC_LALT, JP_ZKHK, JP_MHEN, KC_SPC, KC_SPC, JP_HENK, JP_KANA, KC_RALT, KC_RCTL, TT(1) \
    ),
    /* Layer 1
     * ┌─────┬─────┬─────┬─────┬─────┬─────┐  ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
     * │     │ F1  │ F2  │ F3  │ F4  │ F5  │  │ F6  │ F7  │ F8  │ F9  │ F10 │ F11 │ F12 │ Ins │ DEL │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤  ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┘
     * │ CAP │     │     │     │     │     │  │ScLk │Home │  ↑  │PrSc │PsBr │     │     │     │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤  ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┘
     * │     │     │     │     │     │     │  │Pgup │  ←  │  ↓  │  →  │     │     │     │
     * ├─────┼─────┼─────┼─────┼─────┼─────┘  ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┐
     * │     │     │     │     │     │        │     │PgDw │ End │     │     │     │     │     │
     * ├─────┼─────┼─────┼─────┼─────┼─────┐  ├─────┼─────┼─────┼─────┼─────┼─────┼─────┴─────┘
     * │     │     │ WIN │     │     │     │  │     │     │ APP │     │     │     │
     * └─────┴─────┴─────┴─────┴─────┴─────┘  └─────┴─────┴─────┴─────┴─────┴─────┘
     */
    [1] = LAYOUT(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_INS,  KC_DEL, \
        JP_CAPS, _______, _______, _______, _______, _______, KC_SCRL, KC_HOME, KC_UP,   KC_PSCR, KC_PAUS, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______, KC_PGUP, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______, _______, \
        _______, _______, _______, _______, _______,          KC_PGDN, KC_END,  _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, KC_LGUI, _______, _______, _______, _______, KC_APP,  _______, _______, _______ \
    ),
    /* Layer 2
     * ┌─────┬─────┬─────┬─────┬─────┬─────┐  ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
     * │     │     │     │     │TOGG │NEXT │  │     │     │     │     │     │     │     │     │     │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤  ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┘
     * │     │     │     │     │HUEU │SATU │  │     │ BT1 │ MS↑ │ BT2 │     │     │     │     │
     * ├─────┼─────┼─────┼─────┼─────┼─────┤  ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┘
     * │     │     │     │     │VALU │SPDU │  │ WhU │ MS← │ MS↓ │ MS→ │     │     │     │      
     * ├─────┼─────┼─────┼─────┼─────┼─────┘  ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┐
     * │     │     │     │     │     │        │     │ WhD │     │     │     │     │     │     │
     * ├─────┼─────┼─────┼─────┼─────┼─────┐  ├─────┼─────┼─────┼─────┼─────┼─────┼─────┴─────┘
     * │     │     │     │     │     │ Snp │  │     │     │     │     │     │     │
     * └─────┴─────┴─────┴─────┴─────┴─────┘  └─────┴─────┴─────┴─────┴─────┴─────┘
     */
    [2] = LAYOUT(
        KC_DEL,  _______, _______, _______, UG_TOGG, UG_NEXT,   _______, _______, _______, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, UG_HUEU, UG_SATU,   _______, MS_BTN1, MS_UP,   MS_BTN2, _______, _______, _______, _______, \
        _______, _______, _______, _______, UG_VALU, UG_SPDU,   MS_WHLU, MS_LEFT, MS_DOWN, MS_RGHT, _______, _______, _______, \
        _______, _______, _______, _______, _______,            _______, MS_WHLD, _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, LSG(JP_S), _______, _______, _______, _______, _______, _______ \
    ),
};


// ===== 行配置（ずらし版） =====
#define ROW_TITLE 1   // タイトルを1行目へ
#define ROW_INFO  3   // 情報行を3行目へ

// 行クリア（21桁ぶんスペース）
static inline void clear_line(uint8_t row) {
    oled_set_cursor(0, row);
    for (uint8_t i = 0; i < 21; i++) oled_write_P(PSTR(" "), false);
}

// 前回描画した状態
static uint8_t last_layer = 255;
static uint8_t last_caps  = 0xFF;

// 起動時は描画しない（初回はoled_task_userでまとめて描く）
void keyboard_post_init_user(void) {
    rgblight_reload_from_eeprom();
    last_layer = 255;
    last_caps  = 0xFF;
}

void suspend_wakeup_init_user(void) {
    rgblight_reload_from_eeprom();
}

bool oled_task_user(void) {
    static bool inited = false;
    if (!inited) { inited = true; oled_on(); oled_clear(); oled_clear(); }

    // Slave side (usually the half without USB power): show only the title.
    if (!is_keyboard_master()) {
        static bool slave_drawn = false;
        if (!slave_drawn) {
            oled_clear();
            clear_line(ROW_TITLE);
            oled_set_cursor(0, ROW_TITLE);
            oled_write_P(PSTR("Tzolk'in v.2.0"), false);
            slave_drawn = true;
        }
        return false;
    }

    uint8_t layer = get_highest_layer(layer_state | default_layer_state);
    uint8_t caps  = host_keyboard_led_state().caps_lock;

    if (layer != last_layer || caps != last_caps) {
        // タイトル（row1）
        clear_line(ROW_TITLE);
        oled_set_cursor(0, ROW_TITLE);
        oled_write_P(PSTR("Tzolk'in v.2.0"), false);

        // 情報（row3）: "Layer: Base" / "Layer: Fn" ＋ Caps ONなら " / Caps"
        clear_line(ROW_INFO);
        oled_set_cursor(0, ROW_INFO);
        oled_write_P(PSTR("Layer: "), false);
        switch (layer) {
            case 0: oled_write_P(PSTR("Base"), false); break;
            case 1: oled_write_P(PSTR("Fn"),   false); break;
            case 2: oled_write_P(PSTR("Fn2"),   false); break;
            default: oled_write_P(PSTR("?"),    false); break;
        }
        if (caps) oled_write_P(PSTR(" / Caps"), false);

        last_layer = layer;
        last_caps  = caps;
    }
    return false;
}