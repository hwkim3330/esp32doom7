/*
 * Doom Input Implementation - ESP32 Arduino Port
 */

#include "doom_input.h"
#include "doom_engine.h"
#include <Arduino.h>

// Current input state
static uint8_t key_state = 0;
static int touch_x = -1;
static int touch_y = -1;

// Initialize input system
void doom_input_init() {
    key_state = 0;
    touch_x = -1;
    touch_y = -1;
    
    Serial.println("Input system initialized");
}

// Update input state
void doom_input_update() {
    // Process input and update game state
    if (key_state & DOOM_KEY_UP) {
        doom_state.player_y -= 2;
    }
    if (key_state & DOOM_KEY_DOWN) {
        doom_state.player_y += 2;
    }
    if (key_state & DOOM_KEY_LEFT) {
        doom_state.player_angle -= 5;
    }
    if (key_state & DOOM_KEY_RIGHT) {
        doom_state.player_angle += 5;
    }
    
    // Keep angle in 0-360 range
    if (doom_state.player_angle < 0) doom_state.player_angle += 360;
    if (doom_state.player_angle >= 360) doom_state.player_angle -= 360;
    
    // Handle fire button
    if (key_state & DOOM_KEY_CTRL) {
        // Fire weapon
    }
    
    // Handle use button
    if (key_state & DOOM_KEY_SPACE) {
        // Open doors, activate switches
    }
    
    // Handle menu
    if (key_state & DOOM_KEY_ESC) {
        // Open/close menu
    }
}

// Key press handler
void doom_input_key_down(uint8_t key) {
    key_state |= key;
}

// Key release handler
void doom_input_key_up(uint8_t key) {
    key_state &= ~key;
}

// Get current key state
uint8_t doom_input_get_keys() {
    return key_state;
}

// Set touch position
void doom_input_set_touch(int x, int y) {
    touch_x = x;
    touch_y = y;
}