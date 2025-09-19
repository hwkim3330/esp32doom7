# 🔌 Hardware Connection Guide

## Waveshare ESP32-S3-Touch-LCD-7 Pinout

### Display Interface
| Function | GPIO | Notes |
|----------|------|-------|
| LCD_BL | GPIO2 | Backlight control |
| LCD_RST | GPIO3 | Display reset |
| LCD_CS | GPIO6 | Chip select |
| LCD_SDA | GPIO7 | SPI data |
| LCD_SCL | GPIO8 | SPI clock |
| LCD_DC | GPIO9 | Data/Command |

### Touch Controller (GT911)
| Function | GPIO | I2C Address |
|----------|------|-------------|
| TOUCH_SDA | GPIO19 | 0x5D or 0x14 |
| TOUCH_SCL | GPIO20 | - |
| TOUCH_INT | GPIO21 | Interrupt |
| TOUCH_RST | GPIO38 | Reset |

### RGB Interface Pins
| Signal | GPIO |
|--------|------|
| VSYNC | GPIO40 |
| HSYNC | GPIO39 |
| DE | GPIO41 |
| PCLK | GPIO42 |
| R[0-4] | GPIO1,10,11,12,13 |
| G[0-5] | GPIO14,15,16,17,18,45 |
| B[0-4] | GPIO4,5,6,7,8 |

### External Connections

#### USB Programming
```
┌─────────────┐
│   USB-C     │
│  Connector  │
├─────────────┤
│ VBUS → 5V   │
│ D+ → USB_D+ │
│ D- → USB_D- │
│ GND → GND   │
└─────────────┘
```

#### Power Supply Options

##### Option 1: USB Power (Recommended)
- Connect USB-C cable
- Provides 5V @ 500mA
- Sufficient for normal operation

##### Option 2: External 5V Supply
```
External 5V ──→ [5V Pin]
            ──→ [GND Pin]
```
⚠️ **Warning:** Do not exceed 5.5V!

### Boot and Reset Buttons

```
┌──────────────────────┐
│                      │
│  [BOOT]    [RESET]   │
│                      │
└──────────────────────┘
```

| Button | Function | Usage |
|--------|----------|--------|
| BOOT | GPIO0 to GND | Hold for upload mode |
| RESET | EN to GND | System reset |

## Memory Architecture

### Flash Memory Map (16MB)
```
0x000000 - 0x00FFFF : Bootloader (64KB)
0x010000 - 0x3FFFFF : Application (4MB)
0x400000 - 0x4FFFFF : SPIFFS (1MB)
0x500000 - 0xFFFFFF : Reserved (11MB)
```

### PSRAM Map (8MB OPI)
```
0x3C000000 - 0x3C7FFFFF : PSRAM Space
├─ Frame Buffer: 320x240x2 = 153KB
├─ WAD Cache: 4MB
├─ Game State: 2MB
└─ Audio Buffer: 512KB
```

## Display Configuration

### Panel Settings
```cpp
// In ESP_Panel_Board_Custom.h
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480
#define COLOR_DEPTH 16  // RGB565

// Timing parameters
#define HSYNC_POLARITY 0
#define VSYNC_POLARITY 0
#define PCLK_ACTIVE_NEG 1
#define DE_IDLE_HIGH 0
#define PCLK_IDLE_HIGH 0
```

### Touch Calibration
```cpp
// GT911 Configuration
#define TOUCH_MAX_POINTS 5
#define TOUCH_X_MAX 800
#define TOUCH_Y_MAX 480
#define TOUCH_SWAP_XY 0
#define TOUCH_MIRROR_X 0
#define TOUCH_MIRROR_Y 0
```

## Expansion Headers

### GPIO Header (J1)
```
┌───┬───┬───┬───┬───┬───┬───┬───┐
│3V3│GND│IO4│IO5│IO6│IO7│IO8│IO9│
└───┴───┴───┴───┴───┴───┴───┴───┘
```

### I2C/SPI Header (J2)
```
┌───┬───┬───┬───┬───┐
│3V3│GND│SDA│SCL│CS │
└───┴───┴───┴───┴───┘
```

## Test Points

| TP | Signal | Normal Voltage |
|----|--------|---------------|
| TP1 | 3.3V | 3.3V ± 0.1V |
| TP2 | 1.8V | 1.8V ± 0.1V |
| TP3 | VBUS | 5.0V ± 0.25V |
| TP4 | GND | 0V |

## LED Indicators

| LED | Color | Meaning |
|-----|-------|---------|
| PWR | Red | Power ON |
| CHG | Yellow | Battery Charging |
| USER | Blue | User programmable (GPIO48) |

## Troubleshooting Hardware

### No Display
1. Check backlight (GPIO2 HIGH)
2. Verify 3.3V on display connector
3. Test reset pin (GPIO3)

### Touch Not Responding
1. Check I2C communication
2. Verify GT911 power (3.3V)
3. Test interrupt pin (GPIO21)

### Board Not Detected
1. Check USB cable (data capable)
2. Verify USB drivers installed
3. Test with different USB port

## Safety Warnings

⚠️ **IMPORTANT:**
- Never connect 5V to 3.3V pins
- Do not short circuit power pins
- Avoid static discharge (use ESD protection)
- Maximum current per GPIO: 40mA
- Total GPIO current: 200mA

## Recommended Tools

### Essential
- USB-C cable (data + power)
- Computer with Arduino IDE

### Optional
- Multimeter for voltage testing
- Logic analyzer for debugging
- Oscilloscope for signal analysis
- Anti-static wrist strap

## External Peripheral Support

### Audio Output
```cpp
// PWM Audio on GPIO26
#define AUDIO_PIN 26
#define AUDIO_PWM_CHANNEL 0
#define AUDIO_PWM_FREQ 25000
#define AUDIO_PWM_RESOLUTION 8
```

### SD Card (Optional)
```cpp
// SPI SD Card Connection
#define SD_CS   GPIO47
#define SD_MOSI GPIO35
#define SD_MISO GPIO36
#define SD_SCK  GPIO37
```

## Performance Optimization

### Power Management
```cpp
// For maximum performance
setCpuFrequencyMhz(240);
// For power saving
setCpuFrequencyMhz(80);
```

### Display Refresh
- Target: 35 FPS
- Actual: 30-40 FPS
- Buffer: Double buffering in PSRAM

---
*For detailed schematics, visit [Waveshare Wiki](https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-7)*