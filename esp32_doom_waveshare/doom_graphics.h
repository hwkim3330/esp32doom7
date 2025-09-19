/*
 * Doom Graphics Header - ESP32 Arduino Port
 */

#ifndef DOOM_GRAPHICS_H
#define DOOM_GRAPHICS_H

#include <stdint.h>

// Frame buffer
extern uint16_t* doom_frame_buffer;

// Graphics initialization
void doom_graphics_init(int width, int height);

// Rendering
void doom_graphics_render();
void doom_draw_pixel(int x, int y, uint16_t color);
void doom_draw_line(int x1, int y1, int x2, int y2, uint16_t color);
void doom_draw_rect(int x, int y, int w, int h, uint16_t color);
void doom_fill_rect(int x, int y, int w, int h, uint16_t color);
void doom_clear_screen(uint16_t color);

// Color conversion
uint16_t rgb888_to_rgb565(uint8_t r, uint8_t g, uint8_t b);

#endif // DOOM_GRAPHICS_H