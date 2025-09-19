/*
 * Doom Graphics Implementation - ESP32 Arduino Port
 */

#include "doom_graphics.h"
#include <Arduino.h>
#include <string.h>

// Frame buffer pointer (allocated in PSRAM)
uint16_t* doom_frame_buffer = NULL;

// Screen dimensions
static int screen_width = 0;
static int screen_height = 0;

// Initialize graphics subsystem
void doom_graphics_init(int width, int height) {
    screen_width = width;
    screen_height = height;
    
    // Allocate frame buffer in PSRAM
    size_t buffer_size = width * height * sizeof(uint16_t);
    doom_frame_buffer = (uint16_t*)ps_malloc(buffer_size);
    
    if (!doom_frame_buffer) {
        Serial.println("Failed to allocate frame buffer in PSRAM!");
        return;
    }
    
    Serial.printf("Frame buffer allocated: %d x %d (%d bytes)\n", 
                  width, height, buffer_size);
    
    // Clear screen
    doom_clear_screen(0x0000);
}

// Main rendering function
void doom_graphics_render() {
    if (!doom_frame_buffer) return;
    
    // Demo rendering - draws a simple test pattern
    // In a real implementation, this would render the actual game
    
    static uint16_t frame = 0;
    frame++;
    
    // Clear background
    doom_clear_screen(rgb888_to_rgb565(32, 32, 32));
    
    // Draw demo elements (replace with actual Doom rendering)
    // Draw walls
    for (int i = 0; i < 10; i++) {
        int x = (i * 32 + frame) % screen_width;
        doom_fill_rect(x, 50, 30, 100, rgb888_to_rgb565(128, 64, 0));
    }
    
    // Draw floor
    doom_fill_rect(0, screen_height - 50, screen_width, 50, 
                   rgb888_to_rgb565(64, 64, 64));
    
    // Draw ceiling
    doom_fill_rect(0, 0, screen_width, 50, 
                   rgb888_to_rgb565(32, 32, 64));
    
    // Draw crosshair
    int cx = screen_width / 2;
    int cy = screen_height / 2;
    doom_draw_line(cx - 10, cy, cx + 10, cy, 0xFFFF);
    doom_draw_line(cx, cy - 10, cx, cy + 10, 0xFFFF);
}

// Draw a single pixel
void doom_draw_pixel(int x, int y, uint16_t color) {
    if (x >= 0 && x < screen_width && y >= 0 && y < screen_height) {
        doom_frame_buffer[y * screen_width + x] = color;
    }
}

// Draw a line
void doom_draw_line(int x1, int y1, int x2, int y2, uint16_t color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    while (true) {
        doom_draw_pixel(x1, y1, color);
        
        if (x1 == x2 && y1 == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

// Draw a rectangle outline
void doom_draw_rect(int x, int y, int w, int h, uint16_t color) {
    doom_draw_line(x, y, x + w - 1, y, color);
    doom_draw_line(x + w - 1, y, x + w - 1, y + h - 1, color);
    doom_draw_line(x + w - 1, y + h - 1, x, y + h - 1, color);
    doom_draw_line(x, y + h - 1, x, y, color);
}

// Fill a rectangle
void doom_fill_rect(int x, int y, int w, int h, uint16_t color) {
    for (int j = y; j < y + h && j < screen_height; j++) {
        for (int i = x; i < x + w && i < screen_width; i++) {
            if (i >= 0 && j >= 0) {
                doom_draw_pixel(i, j, color);
            }
        }
    }
}

// Clear the entire screen
void doom_clear_screen(uint16_t color) {
    if (!doom_frame_buffer) return;
    
    size_t pixel_count = screen_width * screen_height;
    for (size_t i = 0; i < pixel_count; i++) {
        doom_frame_buffer[i] = color;
    }
}

// Convert RGB888 to RGB565
uint16_t rgb888_to_rgb565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}