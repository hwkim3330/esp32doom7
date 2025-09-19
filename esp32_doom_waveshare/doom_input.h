/*
 * Doom Input Header - ESP32 Arduino Port
 */

#ifndef DOOM_INPUT_H
#define DOOM_INPUT_H

#include <stdint.h>

// Initialize input system
void doom_input_init();

// Update input state
void doom_input_update();

// Key press/release handlers
void doom_input_key_down(uint8_t key);
void doom_input_key_up(uint8_t key);

// Get current input state
uint8_t doom_input_get_keys();

// Touch position
void doom_input_set_touch(int x, int y);

#endif // DOOM_INPUT_H