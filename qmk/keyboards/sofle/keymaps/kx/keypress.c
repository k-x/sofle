// NumLock control
bool reset_num_lock_on_release = false;

// Tab-related (also see rgb.c)
bool alt_tab = false;
bool single_tab = false;

// Layers-related
bool switch_to_nav = false;

// RGB Lighting timer
deferred_token tab_highlight_token = INVALID_DEFERRED_TOKEN;

// ---------------------------------------------------------------------------

/**
 * Delayed Fn layer RGB lighting
 */
static uint32_t tab_highlight_callback(uint32_t trigger_time, void* cb_arg) {
    single_tab = false; // cancels KC_TAB
    #ifdef RGB_INCLUDED
    rgblight_set_layer_state(_RGB_FN, IS_LAYER_ON(_FN));
    #endif
    screen_output_fn_layer();
    return 0;
}

/**
 * Tap numpad code while forcing NumLock
 */
static void tap_numpad_code(uint16_t keycode) {
    if (!host_keyboard_led_state().num_lock) {
        reset_num_lock_on_release = true;
        tap_code16(KC_NUM);
        tap_code16(keycode);
        tap_code16(KC_NUM);
    } else {
        tap_code16(keycode);
    }
}

/**
 * Number layer press (MY_0 ... MY_9)
 */
static bool tap_num_row(uint16_t numpad_keycode, uint16_t base_layer_keycode) {
    uint8_t mods = get_mods();
    
    if (mods & BIT_LSFT) { // Left Shift --> base layer
        unregister_mods(BIT_LSFT);
        tap_code(base_layer_keycode);
        register_mods(BIT_LSFT);
    } else if (mods) { // Has any mods --> mods + base layer
        tap_code(base_layer_keycode);
    } else { // Just number row --> send number (numpad key)
        tap_numpad_code(numpad_keycode);
    }

    return false;
}

/**
 *
 */
bool process_record_user(uint16_t keycode, keyrecord_t* record) {
    // wake up and reset RGB light timer
    #if defined(RGB_INCLUDED) && defined(MY_RGBLIGHT_TIMEOUT)
    if (record->event.pressed) {
        rgblight_reset_timer();
    }
    #endif

    // If something is pressed after Tab then cancel KC_TAB
    if (single_tab && keycode != KC_TAB) {
        single_tab = false;
    }

    switch (keycode) {
        case KC_TAB:
            // AltGr+Tab = Alt+Tab (on press)
            if (record->tap.count == 0) { // On hold
                if (record->event.pressed) { // press
                    if (alt_tab || HAS_MOD(KC_RALT)) {
                        unregister_mods(BIT_RALT);
                        register_mods(BIT_LALT);
                        alt_tab = true;
                    } else if (HAS_MOD(KC_LALT) || HAS_MOD(KC_LCTL) || HAS_MOD(KC_LSFT)) {
                    } else { // activate FN layer
                        layer_on(_FN);
                        tab_highlight_token = defer_exec(250, tab_highlight_callback, NULL);
                        single_tab = true;
                        return false;
                    }
                } else { // release
                    layer_off(_FN);
                    cancel_deferred_exec(tab_highlight_token);
                    if (single_tab) {
                        tap_code(KC_TAB);
                        single_tab = false;
                    }
                }
            }
            return true;

        case KC_RALT:
            // AltGr+Tab = Alt+Tab (on release)
            if (alt_tab) {
                unregister_mods(BIT_LALT);
                alt_tab = false;
                return false;
            }
            return true;

        // Ctrl that activates base layer
        case MY_BCTL:
            if (record->tap.count == 0) { // On hold
                if (record->event.pressed) { // On press
                    register_mods(BIT_LCTL);
                    if (IS_LAYER_ON(_NAV)) {
                        layer_off(_NAV);
                        switch_to_nav = true;
                    }
                } else { // release
                    unregister_mods(BIT_LCTL);
                    if (switch_to_nav) {
                        layer_on(_NAV);
                        switch_to_nav = false;
                    }
                }
            }
            return false;

        // Numpad keys: force NumLock
        case KC_KP_1 ... KC_KP_0: // Full range is KC_KP_SLASH ... KC_KP_DOT, but only numbers are dependent on NumLock
            if (record->event.pressed) {
                tap_numpad_code(keycode);
                return false;
            }
            return true;

        case MY_VOLD:
            for (int i = 0; i < 5; i++) {
                tap_code(KC_VOLD);
            }
            return false;

        case MY_VOLU:
            for (int i = 0; i < 5; i++) {
                tap_code(KC_VOLU);
            }
            return false;

        // Top row numbers layer
        case MY_0: return record->event.pressed && tap_num_row(KC_P0, KC_0);
        case MY_1: return record->event.pressed && tap_num_row(KC_P1, KC_1);
        case MY_2: return record->event.pressed && tap_num_row(KC_P2, KC_2);
        case MY_3: return record->event.pressed && tap_num_row(KC_P3, KC_3);
        case MY_4: return record->event.pressed && tap_num_row(KC_P4, KC_4);
        case MY_5: return record->event.pressed && tap_num_row(KC_P5, KC_5);
        case MY_6: return record->event.pressed && tap_num_row(KC_P6, KC_6);
        case MY_7: return record->event.pressed && tap_num_row(KC_P7, KC_7);
        case MY_8: return record->event.pressed && tap_num_row(KC_P8, KC_8);
        case MY_9: return record->event.pressed && tap_num_row(KC_P9, KC_9);

        // Code layer keys and macros:
        case MY_RBL:  if (record->event.pressed) { SEND_STRING("("); } return false;
        case MY_RBR:  if (record->event.pressed) { SEND_STRING(")"); } return false; 
        case MY_SBL:  if (record->event.pressed) { SEND_STRING("["); } return false; 
        case MY_SBR:  if (record->event.pressed) { SEND_STRING("]"); } return false; 
        case MY_CBL:  if (record->event.pressed) { SEND_STRING("{"); } return false; 
        case MY_CBR:  if (record->event.pressed) { SEND_STRING("}"); } return false; 

    }

    return true;  // Continue default handling.
}

//        // Switch to the NAV layer with Control and Shift applied on hold, KC_Q on tap.
//        case MY_QNAV:
//            if (record->tap.count == 0) {             // On hold.
//                if (record->event.pressed) {          // On press.
//                    register_mods(MOD_BIT(KC_LSFT) | MOD_BIT(KC_LCTL));   // Hold left Ctrl & Shift.
//                } else {                              // On release.
//                    unregister_mods(MOD_BIT(KC_LSFT) | MOD_BIT(KC_LCTL)); // Release left Ctrl & Shift.
//                }
//            }
//            // Continue default handling, which switches to NAV on hold and performs KC_Q when tapped.
//            return true;
//
//        case MY_ANAV:
//            if (record->tap.count == 0) {             // On hold.
//                if (record->event.pressed) {          // On press.
//                    register_mods(MOD_BIT(KC_LCTL));   // Hold
//                } else {                              // On release.
//                    unregister_mods(MOD_BIT(KC_LCTL)); // Release
//                }
//            }
//            return true;
//
//        case MY_ZNAV:
//            if (record->tap.count == 0) {             // On hold.
//                if (record->event.pressed) {          // On press.
//                    register_mods(MOD_BIT(KC_LSFT));   // Hold
//                } else {                              // On release.
//                    unregister_mods(MOD_BIT(KC_LSFT)); // Release
//                }
//            }
//            return true;
//
