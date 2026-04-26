#include QMK_KEYBOARD_H

enum layers {
    _BASE,
    _FN,
};

enum custom_keycodes {
    DRAG_SCROLL = SAFE_RANGE,
};

#define _______ KC_TRNS
#define XXXXXXX KC_NO

static bool  drag_scroll_enabled = false;
static float scroll_accumulated_v = 0;

#ifndef SCROLL_DIVISOR_V
#    define SCROLL_DIVISOR_V 12.0
#endif

/*
 * Assumptions:
 * - Physical layout is 56 main keys plus two 5-way clusters.
 * - Each 5-way cluster is mapped in this order: left, down, center, up, right.
 * - The 5-way clusters are assumed to be the dedicated last matrix row on each half.
 */
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,       KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,       KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
        KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,       KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,       KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
        KC_LGUI, KC_LALT, LT(_FN, KC_ESC), KC_SPC,                 KC_ENT,  LT(_FN, KC_ESC), KC_RALT, KC_RGUI,
        KC_HOME, KC_PGDN, KC_DEL,  KC_PGUP, KC_END,               MS_WHLL, MS_WHLD, MS_BTN1, MS_WHLU, MS_WHLR
    ),

    [_FN] = LAYOUT(
        QK_BOOT, KC_F1,   KC_F2,    KC_F3,        KC_F4,    KC_F5,      KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,
        _______, KC_EXLM, KC_AT,    KC_HASH,      KC_DLR,   KC_PERC,    KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
        _______, _______, MS_BTN1,  DRAG_SCROLL,  MS_BTN2,  _______,    KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,
        _______, _______, _______,  _______,      _______,   _______,    _______, _______, _______, _______, _______, _______,
        _______, _______, _______,  _______,                             _______, _______, _______, _______,
        KC_MPRV, KC_VOLD, KC_MUTE,  KC_VOLU,      KC_MNXT,              MS_LEFT, MS_DOWN, MS_BTN1, MS_UP,   MS_RGHT
    )
};

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (drag_scroll_enabled) {
        scroll_accumulated_v += (float)mouse_report.y / SCROLL_DIVISOR_V;
        mouse_report.v = (int8_t)scroll_accumulated_v;
        scroll_accumulated_v -= (int8_t)mouse_report.v;
        mouse_report.x = 0;
        mouse_report.y = 0;
    }
    return mouse_report;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DRAG_SCROLL:
            drag_scroll_enabled = record->event.pressed;
            if (!drag_scroll_enabled) {
                scroll_accumulated_v = 0;
            }
            return false;
        default:
            return true;
    }
}

void keyboard_post_init_user(void) {
    // Keep runtime debug off for normal operation.
    debug_enable = false;
    debug_matrix = false;
    debug_keyboard = false;
    debug_mouse = false;
}
