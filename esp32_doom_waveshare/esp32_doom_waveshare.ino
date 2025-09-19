/*
 * ESP32 Doom for Waveshare ESP32-S3-Touch-LCD-7
 * Version 1.0 - Complete Implementation
 * 
 * Hardware: Waveshare ESP32-S3-Touch-LCD-7 (800x480, GT911 Touch, 16MB Flash, 8MB PSRAM)
 * 
 * Features:
 * - Full DOOM gameplay at 35 FPS
 * - Touch screen controls
 * - PSRAM optimization
 * - SPIFFS WAD file storage
 */

#include <Arduino.h>
#include <Waveshare_ST7262_LVGL.h>
#include <lvgl.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <stddef.h>  // For size_t

// Doom engine includes
#include "doom_engine.h"
#include "doom_graphics.h"
#include "doom_input.h"

// Display constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480
#define DOOM_WIDTH 320
#define DOOM_HEIGHT 240

// Game state
static bool doom_initialized = false;
static bool game_running = false;
static uint32_t last_frame_time = 0;
static const uint32_t target_fps = 35;
static const uint32_t frame_interval = 1000 / target_fps;

// Touch input variables
static lv_indev_t* touch_indev;
static lv_point_t last_touch_point = {0, 0};
static bool touch_pressed = false;

// Virtual controls
typedef struct {
    lv_area_t area;
    const char* label;
    uint8_t key_code;
} virtual_button_t;

// Virtual gamepad layout
virtual_button_t virtual_buttons[] = {
    {{10, 350, 80, 420}, "↑", DOOM_KEY_UP},
    {{10, 430, 80, 470}, "↓", DOOM_KEY_DOWN}, 
    {{90, 390, 160, 430}, "←", DOOM_KEY_LEFT},
    {{170, 390, 240, 430}, "→", DOOM_KEY_RIGHT},
    {{640, 350, 710, 420}, "FIRE", DOOM_KEY_CTRL},
    {{720, 350, 790, 420}, "OPEN", DOOM_KEY_SPACE},
    {{640, 280, 710, 330}, "MENU", DOOM_KEY_ESC},
    {{720, 280, 790, 330}, "RUN", DOOM_KEY_SHIFT}
};

// Function prototypes
void doom_init();
void doom_update();
void doom_render();
void handle_touch_input();
void draw_virtual_controls();
bool load_doom_wad();

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32 Doom for Waveshare LCD Starting...");
    
    // Check PSRAM
    if (!psramFound()) {
        Serial.println("ERROR: PSRAM not found! Cannot run DOOM.");
        while(1) delay(1000);
    }
    Serial.printf("PSRAM found: %d bytes\n", ESP.getPsramSize());
    
    // Initialize SPIFFS for WAD file
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    
    // Initialize display and touchscreen
    Serial.println("Initialize display and touchscreen");
    lcd_init();
    
    // Get touch input device
    touch_indev = lv_indev_get_next(NULL);
    
    Serial.println("Loading Doom...");
    
    // Lock LVGL mutex
    lvgl_port_lock(-1);
    
    // Load Doom WAD file
    if (!load_doom_wad()) {
        Serial.println("Failed to load Doom WAD file");
        lv_obj_t* label = lv_label_create(lv_scr_act());
        lv_label_set_text(label, "Doom WAD file not found!\nUpload doom1.wad to SPIFFS");
        lv_obj_center(label);
        lvgl_port_unlock();
        return;
    }
    
    // Initialize Doom engine
    doom_init();
    doom_initialized = true;
    game_running = true;
    
    Serial.println("Doom initialized successfully!");
    
    lvgl_port_unlock();
    
    last_frame_time = millis();
}

void loop() {
    if (!doom_initialized || !game_running) {
        delay(100);
        return;
    }
    
    uint32_t current_time = millis();
    
    // Frame rate limiting
    if (current_time - last_frame_time >= frame_interval) {
        lvgl_port_lock(-1);
        
        // Handle input
        handle_touch_input();
        
        // Update game logic
        doom_update();
        
        // Render frame
        doom_render();
        
        // Draw virtual controls
        draw_virtual_controls();
        
        lvgl_port_unlock();
        
        last_frame_time = current_time;
    }
    
    delay(1); // Small delay to prevent watchdog reset
}

void doom_init() {
    Serial.println("Initializing Doom engine...");
    
    // Initialize Doom subsystems
    doom_graphics_init(DOOM_WIDTH, DOOM_HEIGHT);
    doom_input_init();
    
    // Load game assets
    doom_load_assets();
    
    Serial.println("Doom engine initialized");
}

void doom_update() {
    // Update game state
    doom_engine_update();
    
    // Process input events
    doom_input_update();
}

void doom_render() {
    // Get LVGL framebuffer
    lv_obj_t* screen = lv_scr_act();
    
    // Render Doom frame to LVGL canvas
    static lv_obj_t* doom_canvas = NULL;
    static lv_color_t* canvas_buffer = NULL;
    
    if (doom_canvas == NULL) {
        // Allocate canvas buffer in PSRAM
        canvas_buffer = (lv_color_t*)ps_malloc(LV_CANVAS_BUF_SIZE_TRUE_COLOR(DOOM_WIDTH, DOOM_HEIGHT));
        if (!canvas_buffer) {
            Serial.println("Failed to allocate canvas buffer!");
            return;
        }
        
        doom_canvas = lv_canvas_create(screen);
        lv_canvas_set_buffer(doom_canvas, canvas_buffer, DOOM_WIDTH, DOOM_HEIGHT, LV_IMG_CF_TRUE_COLOR);
        
        // Center and scale the game area
        lv_obj_set_size(doom_canvas, SCREEN_WIDTH - 200, SCREEN_HEIGHT - 100);
        lv_obj_align(doom_canvas, LV_ALIGN_CENTER, 0, -20);
    }
    
    // Update doom frame buffer
    doom_graphics_render();
    
    // Copy doom frame buffer to canvas
    if (doom_frame_buffer && canvas_buffer) {
        memcpy(canvas_buffer, doom_frame_buffer, DOOM_WIDTH * DOOM_HEIGHT * sizeof(lv_color_t));
        lv_obj_invalidate(doom_canvas);  // Mark as dirty for redraw
    }
}

void handle_touch_input() {
    lv_indev_data_t data;
    _lv_indev_read(touch_indev, &data);  // Using internal LVGL function for compatibility
    
    if (data.state == LV_INDEV_STATE_PRESSED) {
        if (!touch_pressed) {
            touch_pressed = true;
            last_touch_point = data.point;
            
            // Check virtual button presses
            for (int i = 0; i < sizeof(virtual_buttons) / sizeof(virtual_button_t); i++) {
                if (data.point.x >= virtual_buttons[i].area.x1 && 
                    data.point.x <= virtual_buttons[i].area.x2 &&
                    data.point.y >= virtual_buttons[i].area.y1 && 
                    data.point.y <= virtual_buttons[i].area.y2) {
                    
                    doom_input_key_down(virtual_buttons[i].key_code);
                    Serial.printf("Button pressed: %s\n", virtual_buttons[i].label);
                    break;
                }
            }
        }
    } else {
        if (touch_pressed) {
            touch_pressed = false;
            
            // Release all virtual buttons
            for (int i = 0; i < sizeof(virtual_buttons) / sizeof(virtual_button_t); i++) {
                doom_input_key_up(virtual_buttons[i].key_code);
            }
        }
    }
}

void draw_virtual_controls() {
    static bool controls_created = false;
    
    if (!controls_created) {
        // Create virtual control buttons
        for (int i = 0; i < sizeof(virtual_buttons) / sizeof(virtual_button_t); i++) {
            lv_obj_t* btn = lv_btn_create(lv_scr_act());
            lv_obj_set_pos(btn, virtual_buttons[i].area.x1, virtual_buttons[i].area.y1);
            lv_obj_set_size(btn, 
                virtual_buttons[i].area.x2 - virtual_buttons[i].area.x1,
                virtual_buttons[i].area.y2 - virtual_buttons[i].area.y1);
            
            lv_obj_t* label = lv_label_create(btn);
            lv_label_set_text(label, virtual_buttons[i].label);
            lv_obj_center(label);
            
            // Style the button
            lv_obj_set_style_bg_opa(btn, LV_OPA_70, 0);
            lv_obj_set_style_bg_color(btn, lv_color_hex(0x404040), 0);
            lv_obj_set_style_border_width(btn, 2, 0);
            lv_obj_set_style_border_color(btn, lv_color_hex(0x808080), 0);
            lv_obj_set_style_text_color(label, lv_color_white(), 0);
        }
        controls_created = true;
    }
}

bool load_doom_wad() {
    // Check if doom WAD file exists in SPIFFS
    if (!SPIFFS.exists("/doom1.wad")) {
        Serial.println("doom1.wad not found in SPIFFS");
        Serial.println("Please upload data folder using Arduino IDE");
        return false;
    }
    
    File wad_file = SPIFFS.open("/doom1.wad", "r");
    if (!wad_file) {
        Serial.println("Failed to open doom1.wad");
        return false;
    }
    
    size_t wad_size = wad_file.size();
    Serial.printf("WAD file size: %d bytes\n", wad_size);
    
    // Load WAD data into PSRAM
    uint8_t* wad_data = (uint8_t*)ps_malloc(wad_size);
    if (!wad_data) {
        Serial.println("Failed to allocate PSRAM for WAD data");
        wad_file.close();
        return false;
    }
    
    wad_file.read(wad_data, wad_size);
    wad_file.close();
    
    // Initialize Doom with WAD data
    doom_load_wad(wad_data, wad_size);
    
    Serial.println("WAD file loaded successfully");
    return true;
}