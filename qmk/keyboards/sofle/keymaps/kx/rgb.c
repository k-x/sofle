// Used in keypress.c for lighting timeout
#define RGB_INCLUDED 1

// Colors
#define HSV_CONTROL HSV_GOLDENROD   // Base color
#define HSV_EN HSV_AZURE            // Latin layer
#define HSV_RU 0, 200, 150          // Cyrillic layer
#define HSV_LIGHTMAGENTA 0, 50, 80  // Nav supplementary
#define HSV_FN0 128, 255, 255     // Fn col 0   (H 128 == HSV_CYAN)
#define HSV_FN1 85, 255, 255     // Fn col 1   (H 85 == HSV_GREEN)
#define HSV_FN2 128, 255, 255     // Fn col 2
#define HSV_FN3 85, 255, 255     // Fn col 3
#define HSV_FN4 128, 255, 255     // Fn co. 4
#define HSV_FN5 85, 255, 255     // Fn col 5
#define HSV_GAME  88, 255, 255      // Game Green 
#define HSV_GAME1 HSV_ORANGE        // Game ASDW  

/*
#define HSV_GREEN1 56, 255, 255     // Fn col 1
#define HSV_GREEN2 72, 255, 255     // Fn col 2
#define HSV_GREEN3 88, 255, 255     // Fn col 3
#define HSV_GREEN4 104, 255, 255    // Fn col 4
#define HSV_GREEN5 104, 255, 255    // Fn col 4

*/

// RGB indices aliases
// (R = RGB, K = key)
enum LED {
    // Left side:
         RES, RK1, RK2, RK3, RK4, RK5,    // ES = Escape        
         RTB, RKQ, RKW, RKE, RKR, RKT,    // TB = Tab        
         RC1, RKA, RKS, RKD, RKF, RKG,    // C1 = Left Ctrl        
         RFN, RKZ, RKX, RKC, RKV, RKB,    // FN = row 4 k1        
              RA1, RA2, RN1, R_1, RS1,    // A1 = Left Alt     A2 = AltGr       N1 = Left Nav      _1 = Space          S1 = Left Shift  
    // Right side:
         RK6, RK7, RK8, RK9, RK0, RMN,    // MN = Minus
         RKY, RKU, RKI, RKO, RKP, REQ,    // EQ = Equals
         RKH, RKJ, RKK, RKL, RCL, RQT,    // CL = Colon        QT = Quote
         RKN, RKM, RLT, RGT, RSL, RS2,    // LT = <            GT = >           SL = Slash         S2 = Right Shift
    REN, R_2, RBS, RN2, RW2               // EN = Enter        _2 = Space       BS = Backspace     N2 = Right Nav      W2 = Windows
};

// Highlight one key with specified color
#define ONE(k1, hsv)       { k1, 1, hsv }
// Highlight range of keys with specified color
#define RNG(k1, k2, hsv)   { k1, k2 - k1 + 1, hsv }
// Highlight column (first 4 rows, without bottom row)
#define COL(k1, hsv)       { k1, 1, hsv }, { k1 + 6, 1, hsv }, { k1 + 12, 1, hsv }, { k1 + 18, 1, hsv } 

const rgblight_segment_t PROGMEM my_base_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    // Lert side                Right side
    RNG(RK1, RK5, HSV_EN),      RNG(RK6, RMN, HSV_EN),
    RNG(RKQ, RKT, HSV_EN),      RNG(RKY, REQ, HSV_EN),
    RNG(RKA, RKG, HSV_EN),      RNG(RKH, RQT, HSV_EN),
    RNG(RKZ, RKB, HSV_EN),      RNG(RKN, RSL, HSV_EN),
    // Bottom row: highlight NAV only
    ONE(RN1, HSV_PURPLE),
    ONE(RN2, HSV_PURPLE)
);

const rgblight_segment_t PROGMEM my_rus_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    // Lert side                Right side
    RNG(RK1, RK5, HSV_RU),      RNG(RK6, RMN, HSV_RU),
    RNG(RKQ, RKT, HSV_RU),      RNG(RKY, REQ, HSV_RU),
    RNG(RKA, RKG, HSV_RU),      RNG(RKH, RQT, HSV_RU),
    RNG(RKZ, RKB, HSV_RU),      RNG(RKN, RSL, HSV_RU)
);

const rgblight_segment_t PROGMEM my_nav_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    RNG(RK1, RK3, HSV_ORANGE), // Mouse buttons
    RNG(RK4, RK5, HSV_LIGHTMAGENTA), // NumPad / *
    RNG(RTB, RKQ, HSV_CONTROL),
    ONE(RKW, HSV_MAGENTA), // Home
    ONE(RKE, HSV_BLUE), // Up
    ONE(RKR, HSV_MAGENTA), // PgUp
    ONE(RKT, HSV_LIGHTMAGENTA), // NumPad —
    RNG(RC1, RKA, HSV_CONTROL),
    ONE(RKS, HSV_BLUE), // Left
    ONE(RKD, HSV_LIGHTMAGENTA), // NumPad 5
    ONE(RKF, HSV_BLUE), // Right
    ONE(RKG, HSV_GREEN), // Ins
    RNG(RFN, RKZ, HSV_CONTROL),
    ONE(RKX, HSV_MAGENTA), // End
    ONE(RKC, HSV_BLUE), // Down
    ONE(RKV, HSV_MAGENTA), // PgDn
    ONE(RKB, HSV_RED), // Del
    // Right side
    ONE(RK6, HSV_WHITE),          // Calc
    ONE(RK7, HSV_RED),            // Esc
    RNG(RK8, RK9, HSV_MAGENTA),   // /  *
    ONE(RK0, HSV_CORAL),          // −
    ONE(RMN, HSV_LIGHTMAGENTA),   // F21
    ONE(RKY, HSV_LIGHTMAGENTA),   // F18
    RNG(RKU, RKO, HSV_BLUE),      // 7  8  9
    ONE(RKY, HSV_LIGHTMAGENTA),   // F18
    ONE(RKP, HSV_SPRINGGREEN),    // +
    ONE(REQ, HSV_LIGHTMAGENTA),   // F22
    ONE(RKH, HSV_RU),             // F19
    RNG(RKJ, RCL, HSV_BLUE),      // 4  5  6  0
    ONE(RQT, HSV_LIGHTMAGENTA),   // F23
    ONE(RKN, HSV_ORANGE),         // ShareX
    RNG(RKM, RGT, HSV_BLUE),      // 1  2  3
    ONE(RSL, HSV_MAGENTA),        // .
    ONE(RS2, HSV_CONTROL),        // Shift
    ONE(RN1, HSV_EN),
    ONE(RN2, HSV_EN)
);

const rgblight_segment_t PROGMEM my_code_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    RNG(RES, RK5, HSV_CONTROL),
    RNG(RTB, RKQ, HSV_CONTROL),
    ONE(RKW, HSV_BLUE), // Mixer
    ONE(RKE, HSV_MAGENTA), // Vol+
    ONE(RKR, HSV_PURPLE), // Vol++
    ONE(RKT, HSV_CONTROL),
    RNG(RC1, RKA, HSV_CONTROL),
    RNG(RKS, RKF, HSV_MAGENTA), // Prev, Pause, Next
    ONE(RKG, HSV_CONTROL),
    RNG(RFN, RKZ, HSV_CONTROL),
    ONE(RKX, HSV_RED), // Mute
    ONE(RKC, HSV_MAGENTA), // Vol−
    ONE(RKV, HSV_PURPLE), // Vol−−
    ONE(RKB, HSV_CONTROL),
    // Right side
    RNG(RK6, RW2, HSV_CONTROL)
);

const rgblight_segment_t PROGMEM my_num_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    RNG(RK1, RK5, HSV_BLUE),
    RNG(RK6, RK0, HSV_BLUE)
);

const rgblight_segment_t PROGMEM my_fn_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    COL(RES, HSV_FN0),
    COL(RK1, HSV_FN1),
    COL(RK2, HSV_FN2),
    COL(RK3, HSV_FN3),
    COL(RK4, HSV_FN4),
    COL(RK5, HSV_FN5),

    ONE(RA1, HSV_CONTROL),
    ONE(RA2, HSV_BLUE),    // Toggle NumRow Layer 
    ONE(RN1, HSV_GAME1),   // Toggle Game Layer
    RNG(R_1, RS1, HSV_CONTROL),

    // Right side
    COL(RK6, HSV_FN5),
    COL(RK7, HSV_FN4),
    COL(RK8, HSV_FN3),
    COL(RK9, HSV_FN2),
    COL(RK0, HSV_FN1),
    COL(RMN, HSV_FN0),

    RNG(REN, RW2, HSV_CONTROL)
);

const rgblight_segment_t PROGMEM my_game_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    RNG(RES, RKQ, HSV_GAME),
    ONE(RKW, HSV_GAME1),
    RNG(RKE, RC1, HSV_GAME),
    RNG(RKA, RKD, HSV_GAME1),
    RNG(RKF, RW2, HSV_GAME)
);

#define _RGB_BASE  0
#define _RGB_RUS   1
#define _RGB_GAME  2
#define _RGB_NAV   3
#define _RGB_CODE  4
#define _RGB_NUM   5
#define _RGB_FN    6

const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    my_base_layer,   // 0
    my_rus_layer,    // 1
    my_game_layer,   // 2
    my_nav_layer,    // 3
    my_code_layer,   // 4
    my_num_layer,    // 5
    my_fn_layer      // 6; index is also used in keypress.c in tab_highlight_callback()
);

/* RGB Light timer */
bool is_rgb_timeout = false;
deferred_token rgblight_timeout_token = INVALID_DEFERRED_TOKEN;

/**
 * RGB lighting timeout callback
 */
static uint32_t rgblight_timeout_callback(uint32_t trigger_time, void* cb_arg) {
    if (!is_rgb_timeout) {
        rgblight_suspend();
        is_rgb_timeout = true;
    }
    return 0;
}

/**
 *
 */
static void rgblight_reset_timer(void) {
    return;
    // wake up
    if (is_rgb_timeout) {
        is_rgb_timeout = false;
        rgblight_wakeup();
    }

    // reset RGB light timer
    cancel_deferred_exec(rgblight_timeout_token);
    rgblight_timeout_token = defer_exec(MY_RGBLIGHT_TIMEOUT, rgblight_timeout_callback, NULL);
}               

// ------- HOOKS INTO QMK -------

void keyboard_post_init_user(void) {
    // Enable LED layers
    rgblight_layers = my_rgb_layers;
    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(HSV_CONTROL);
    rgblight_reset_timer();
}


bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(_RGB_RUS, led_state.caps_lock);
    return true;
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(_RGB_BASE, true);
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(_RGB_GAME, layer_state_cmp(state, _GAME));
    rgblight_set_layer_state(_RGB_NAV, layer_state_cmp(state, _NAV));
    rgblight_set_layer_state(_RGB_CODE, layer_state_cmp(state, _CODE));
    rgblight_set_layer_state(_RGB_NUM, layer_state_cmp(state, _NUM));
    if (!layer_state_cmp(state, _FN)) {
        rgblight_set_layer_state(_RGB_FN, false);
    }
    return state;
}
