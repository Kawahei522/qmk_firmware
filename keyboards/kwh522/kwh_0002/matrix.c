#include "quantum.h"
#ifdef SPLIT_KEYBOARD
#  include "split_util.h"
#endif

// 左右でピン定義を分ける
static const pin_t row_pins_left[]  = { C6, D7 };         // 左手 ROW
static const pin_t col_pins_left[]  = { F5 };             // 左手 COL

static const pin_t row_pins_right[] = { C6, D7 };         // 右手 ROW
static const pin_t col_pins_right[] = { F5 };             // 右手 通常COL
static const pin_t col_duplex[]     = { C6 };             // 右手 Duplex対象（COLとして読む）

#define MATRIX_ROWS 4  // 左2 + 右2
#define MATRIX_COLS 2  // 通常1 + Duplex1

#define ROW_LEFT(row)      (row)
#define ROW_RIGHT(row)     ((row) + ARRAY_SIZE(row_pins_left))
#define COL_NORMAL          0
#define COL_DUPLEX(col)     ((col) + ARRAY_SIZE(col_pins_right))

// プロトタイプ宣言
void matrix_init_custom(void);
bool matrix_scan_custom(matrix_row_t current_matrix[]);

matrix_row_t matrix[MATRIX_ROWS];

// 1. 初期化関数
void matrix_init(void) {
    matrix_init_custom();  // すでにあるカスタム初期化呼び出し
}

// 2. スキャン関数
uint8_t matrix_scan(void) {
    matrix_scan_custom(matrix);  // すでにあるカスタムスキャン呼び出し
    return 0;
}

// 3. 行の取得
matrix_row_t matrix_get_row(uint8_t row) {
    return matrix[row];
}

// 4. デバッグ出力（任意）
void matrix_print(void) {
#ifdef CONSOLE_ENABLE
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        xprintf("row %u: %016b\n", row, matrix[row]);
    }
#endif
}

void matrix_init_custom(void) {
    // 左手ピン初期化
    for (uint8_t i = 0; i < ARRAY_SIZE(row_pins_left); i++) setPinInputHigh(row_pins_left[i]);
    for (uint8_t i = 0; i < ARRAY_SIZE(col_pins_left); i++) setPinInputHigh(col_pins_left[i]);

    // 右手ピン初期化
    for (uint8_t i = 0; i < ARRAY_SIZE(row_pins_right); i++) setPinInputHigh(row_pins_right[i]);
    for (uint8_t i = 0; i < ARRAY_SIZE(col_pins_right); i++) setPinInputHigh(col_pins_right[i]);
    for (uint8_t i = 0; i < ARRAY_SIZE(col_duplex); i++) setPinInputHigh(col_duplex[i]);
    if (!isLeftHand) {
        setPinOutput(B6);
        writePinHigh(B6);
    }
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    // スキャン前にマトリクスをクリア
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        current_matrix[i] = 0;
    }

    // 各スキャンサイクルの開始時に、左右で使う可能性のある全てのピンをINPUT_PULLUPに戻す
    for (uint8_t i = 0; i < ARRAY_SIZE(row_pins_left); i++) setPinInputHigh(row_pins_left[i]);
    for (uint8_t i = 0; i < ARRAY_SIZE(col_pins_left); i++) setPinInputHigh(col_pins_left[i]);
    for (uint8_t i = 0; i < ARRAY_SIZE(row_pins_right); i++) setPinInputHigh(row_pins_right[i]);
    for (uint8_t i = 0; i < ARRAY_SIZE(col_pins_right); i++) setPinInputHigh(col_pins_right[i]);
    for (uint8_t i = 0; i < ARRAY_SIZE(col_duplex); i++) setPinInputHigh(col_duplex[i]);

    if (is_keyboard_left()) {
        // ROWピンをHi-Zに戻してからスキャン（最初に1回）
        //for (uint8_t i = 0; i < ARRAY_SIZE(row_pins_left); i++) {
        //    setPinInputHigh(row_pins_left[i]);
        //}

        // 左手スキャン（C6→F5, D7→F5）
        for (uint8_t row = 0; row < ARRAY_SIZE(row_pins_left); row++) {
            setPinOutput(row_pins_left[row]);
            writePinLow(row_pins_left[row]);
            wait_us(30);

            for (uint8_t col = 0; col < ARRAY_SIZE(col_pins_left); col++) {
                if (!readPin(col_pins_left[col])) {
                    current_matrix[ROW_LEFT(row)] |= (1 << col);
                }
            }
            setPinInputHigh(row_pins_left[row]);
        }
    } else {
        // ROWピンをHi-Zに戻してからスキャン（最初に1回）
        //for (uint8_t i = 0; i < ARRAY_SIZE(row_pins_right); i++) {
        //    setPinInputHigh(row_pins_right[i]);
        //}

        // 右手通常スキャン（C6→F5, D7→F5）
        for (uint8_t row = 0; row < ARRAY_SIZE(row_pins_right); row++) {
            setPinOutput(row_pins_right[row]);
            writePinLow(row_pins_right[row]);
            wait_us(30);

            for (uint8_t col = 0; col < ARRAY_SIZE(col_pins_right); col++) {
                if (!readPin(col_pins_right[col])) {
                    current_matrix[ROW_RIGHT(row)] |= (1 << COL_NORMAL);
                }
            }
            setPinInputHigh(row_pins_right[row]);
        }

        // Duplexスキャン（D7 → C6）
        // D7 (row_pins_right[1]) をOutput Lowにする前に、
        // F5 (col_pins_right[0]) がまだOutputになっていないか確認する。（全体リセットで担保される）
        setPinOutput(row_pins_right[1]); // D7をOutput Lowにする (配列アクセスに修正)
        writePinLow(row_pins_right[1]);
        wait_us(30);

        for (uint8_t col = 0; col < ARRAY_SIZE(col_duplex); col++) {
            if (!readPin(col_duplex[col])) {
                current_matrix[ROW_RIGHT(1)] |= (1 << COL_DUPLEX(col));
            }
        }
        setPinInputHigh(row_pins_right[1]); // D7をHi-Zに戻す (配列アクセスに修正)
    }
    return true;
}
