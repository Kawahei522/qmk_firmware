// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "keymap_japanese.h"

#include "oled_driver.h"
#include "i2c_master.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Layer 0
     * ┌─────┐┌─────┐
     * │  A  ││  C  │
     * ├─────┤├─────┤
     * │  B  ││ TT1 │
     * └─────┘└─────┘
     */
    [0] = LAYOUT(
        JP_A,  JP_C, \
        JP_B,  TT(1) \
    ),
    /* Layer 1
     * ┌─────┐┌─────┐
     * │  1  ││  3  │
     * ├─────┤├─────┤
     * │  2  ││ TT1 │
     * └─────┘└─────┘
     */
    [1] = LAYOUT(
        JP_1,  JP_3, \
        JP_2,  _______ \
    ),
};

// 前回描画したレイヤーを記録
static uint8_t last_layer = 255;

// 起動時に一度だけ実行
void keyboard_post_init_user(void) {
    oled_clear();
    oled_write("Layer: Base", false);  // 1行で表示
    last_layer = 0;
}

// レイヤー変更を検知してOLEDを更新
bool oled_task_user(void) {
    uint8_t current_layer = get_highest_layer(layer_state | default_layer_state);

    if (current_layer != last_layer) {
        oled_clear();
        switch (current_layer) {
            case 0:
                oled_write("Layer: Base", false);
                break;
            case 1:
                oled_write("Layer: Fn", false);
                break;
            default:
                oled_write("Layer: ?", false);
                break;
        }
        last_layer = current_layer;
    }
    return false;
}
