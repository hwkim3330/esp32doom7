/*
 * Doom Engine Header - ESP32 Arduino Port
 * Based on PrBoom and esp32-doom
 */

#ifndef DOOM_ENGINE_H
#define DOOM_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>  // For size_t

// Key codes for input
#define DOOM_KEY_UP     0x01
#define DOOM_KEY_DOWN   0x02
#define DOOM_KEY_LEFT   0x04
#define DOOM_KEY_RIGHT  0x08
#define DOOM_KEY_CTRL   0x10  // Fire
#define DOOM_KEY_SPACE  0x20  // Open/Use
#define DOOM_KEY_ESC    0x40  // Menu
#define DOOM_KEY_SHIFT  0x80  // Run

// Graphics
extern uint16_t* doom_frame_buffer;

// Core engine functions
void doom_load_wad(uint8_t* wad_data, size_t wad_size);
void doom_load_assets();
void doom_engine_update();

// Graphics functions
void doom_graphics_init(int width, int height);
void doom_graphics_render();

// Input functions  
void doom_input_init();
void doom_input_update();
void doom_input_key_down(uint8_t key);
void doom_input_key_up(uint8_t key);

// Game state
typedef struct {
    int player_x, player_y;
    int player_angle;
    int health;
    bool game_active;
    uint32_t frame_count;
} doom_game_state_t;

extern doom_game_state_t doom_state;

#endif // DOOM_ENGINE_H