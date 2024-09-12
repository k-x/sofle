// Sets up what the OLED screens display.
#ifdef OLED_ENABLE

// Full screen is 5×16 characters
#define STATUS_LENGTH 80

// Layer icons (5×3)
#define ICON_LAYER_EN    0x85
#define ICON_LAYER_RU    0x8A
#define ICON_LAYER_NAV   0x8F
#define ICON_LAYER_FN    0x80
#define ICON_LAYER_CODE  0x94
#define ICON_LAYER_GAME  0x99

// Modifier icons (2×2)
#define ICON_MOD_ALT     0x01
#define ICON_MOD_CTRL    0x03
#define ICON_MOD_ALTGR   0x05
#define ICON_MOD_SHIFT   0x07
#define ICON_MOD_NUM     0x0B
#define ICON_MOD_WIN     0x0E
#define ICON_MOD_FN      0x9E

#define SCREEN_LAYER_RESET 0xFFFF

// Custom screen off timer
bool is_screen_timeout = false;
deferred_token screen_timeout_token = INVALID_DEFERRED_TOKEN;

// Screen buffer
char screen[STATUS_LENGTH];

// State on previous screen update
bool prev_caps = false;
layer_state_t prev_layers = SCREEN_LAYER_RESET;
uint8_t prev_mods = 0;

/**
 * Fills screen buffer with space characters
 */
void screen_clear(void) {
    for (uint8_t i = 0; i < STATUS_LENGTH; i++) {
        screen[i] = ' ';
    }
}

/**
 * Adds 2×2 block to the status array at specified position
 */
static void screen_add_2x2_icon(char symbol, uint8_t x, uint8_t y) {
    for (uint8_t col = 0; col < 2; col++) {
        for (uint8_t row = 0; row < 2; row++) {
            screen[col + x + 5 * (row + y)] = symbol + col + row * 32;
        }
    }
}

/**
 * Adds 5×3 block
 */
static void screen_add_5x3_icon(char symbol, uint8_t y) {
    uint8_t index = y * 5;
    for (uint8_t row = 0; row < 3; row++) {
        for (uint8_t col = 0; col < 5; col++) {
            screen[index++] = symbol + col + row * 32;
        }
    }
}

/**
 * Outputs screen buffer
 */
static void screen_output(void) {
    for (uint8_t i = 0; i < STATUS_LENGTH; i++) {
        oled_write_char(screen[i], false);
    }
}

/**
 * OLED timeout callback
 */
static uint32_t screen_timeout_callback(uint32_t trigger_time, void* cb_arg) {
    screen_clear();
    screen_output();
    // oled_off();
    is_screen_timeout = true;
    return 0;
}

/**
 *
 */
static void screen_reset_timer(void) {
    cancel_deferred_exec(screen_timeout_token);
    screen_timeout_token = defer_exec(MY_OLED_TIMEOUT, screen_timeout_callback, NULL);
    is_screen_timeout = false;
}               

/**
 * Outputs int32 (can be used for debugging)
 */
/*
void oled_write_int(char *caption, uint32_t num) {
    oled_write(caption, false);

    char str[11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    sprintf(str, "%ld", num);
    oled_write_ln(str, false);
}
*/

/**
 * Outputs Fn layer screen
 */
static void screen_output_fn_layer(void) {
    screen_clear();

    screen_add_5x3_icon(ICON_LAYER_FN, 0); // "Fn"

    for (uint8_t i = 15; i < 20; i++) {
        screen[i] = 0x10; // line at row 3
    }

    // Hints about modifiers applied to each keyboard row:
    screen_add_2x2_icon(ICON_MOD_FN,    1,  5);  //  [no mod, "F" icon]
    screen_add_2x2_icon(ICON_MOD_ALT,   1,  8);  //  Alt
    screen_add_2x2_icon(ICON_MOD_CTRL,  1, 11);  //  Ctrl
    screen_add_2x2_icon(ICON_MOD_SHIFT, 1, 14);  //  Shift

    screen_output();
    screen_reset_timer();

    prev_layers = SCREEN_LAYER_RESET;
}

/**
 * Outputs current mode and modifier key status
 */
bool oled_task_user(void) {
    // Do not update the screen for Fn layer if Tab was just pressed (see keypress.c)
    if (IS_LAYER_ON(_FN)) {
        return false;
    }

    uint8_t mods = get_mods();
    bool is_caps_lock_on = host_keyboard_led_state().caps_lock;

    // Don’t do anything if nothing has changed
    if (mods == prev_mods && is_caps_lock_on == prev_caps && layer_state == prev_layers) {
        return false;
    }

    // Save state
    prev_mods = mods;
    prev_caps = is_caps_lock_on;
    prev_layers = layer_state;
    
    // Prepare new screen buffer
    screen_clear();

    // Print current mode. Topmost layer must be on top here:
    if (IS_LAYER_ON(_CODE)) {
        screen_add_5x3_icon(ICON_LAYER_CODE, 0);
    } else if (IS_LAYER_ON(_NAV)) {
        screen_add_5x3_icon(ICON_LAYER_NAV, 0);
    } else if (IS_LAYER_ON(_GAME)) {
        screen_add_5x3_icon(ICON_LAYER_GAME, 0);
    } else { // "Base layer"
        if (is_caps_lock_on) {
            screen_add_5x3_icon(ICON_LAYER_RU, 0);
        } else {
            screen_add_5x3_icon(ICON_LAYER_EN, 0);
        }
    }

    // Print modifier key status
    if (mods & MOD_MASK_CTRL)  { screen_add_2x2_icon(ICON_MOD_CTRL,   0,  8); }
    if (mods & MOD_MASK_SHIFT) { screen_add_2x2_icon(ICON_MOD_SHIFT,  3,  8); }
    if (mods & BIT_LALT)       { screen_add_2x2_icon(ICON_MOD_ALT,    0, 11); }
    if (mods & BIT_RALT)       { screen_add_2x2_icon(ICON_MOD_ALTGR,  3, 11); }
    if (host_keyboard_led_state().num_lock)         
                               { screen_add_2x2_icon(ICON_MOD_NUM,    0, 14); }
    if (mods & MOD_MASK_GUI)   { screen_add_2x2_icon(ICON_MOD_WIN,    3, 14); }

    screen_output();
    screen_reset_timer();

    return false;
}

/**
 * Screen rotation
 */
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

#endif