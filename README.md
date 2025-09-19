# ESP32 DOOM 🎮 - Waveshare 7" Touch LCD

A complete implementation of DOOM running on ESP32-S3 with Waveshare 7" Touch LCD display.

![ESP32 DOOM](https://img.shields.io/badge/ESP32-DOOM-red?style=for-the-badge&logo=doom)
![Version](https://img.shields.io/badge/Version-1.0-blue?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-ESP32--S3-green?style=for-the-badge)

## 📋 Hardware Requirements

### Essential Components
- **Board**: Waveshare ESP32-S3-Touch-LCD-7
  - ESP32-S3 Chip (Dual-core Xtensa LX7)
  - 16MB Flash Memory
  - 8MB PSRAM (OPI)
  - 800×480 Resolution Display
  - GT911 Capacitive Touch Controller
- **USB Cable**: Type-C for programming and power
- **Computer**: Windows/Mac/Linux with Arduino IDE

### Board Specifications
| Feature | Specification |
|---------|--------------|
| Processor | ESP32-S3 240MHz Dual Core |
| Flash | 16MB |
| PSRAM | 8MB OPI PSRAM |
| Display | 7" IPS LCD, 800×480 |
| Touch | GT911 Capacitive Touch |
| Interface | RGB565, SPI |
| Power | 5V via USB-C |

## 🔧 Installation Guide

### Step 1: Arduino IDE Setup

1. Download and install [Arduino IDE 2.x](https://www.arduino.cc/en/software)
2. Add ESP32 board support:
   - File → Preferences
   - Additional Board Manager URLs:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Tools → Board → Boards Manager → Search "esp32" → Install

### Step 2: Board Configuration

Select the following settings in Arduino IDE:
- **Board**: ESP32S3 Dev Module
- **PSRAM**: OPI PSRAM
- **Flash Size**: 16MB
- **Partition Scheme**: Huge APP (3MB No OTA/1MB SPIFFS)
- **Upload Speed**: 921600
- **Port**: COM5 (or your port)

### Step 3: Library Installation

Install required libraries via Library Manager:
1. **LVGL** (v8.3.x) - Graphics library
2. **ESP32_Display_Panel** - Display driver
3. **ESP32_IO_Expander** - I/O expansion

Special library (install as ZIP):
- [Waveshare_ST7262_LVGL](https://github.com/iamfaraz/Waveshare_ST7262_LVGL)

### Step 4: Project Structure

```
esp32doom7/
├── esp32_doom_waveshare/
│   ├── esp32_doom_waveshare.ino    # Main sketch
│   ├── doom_engine.h                # Game engine header
│   ├── doom_engine.cpp              # Game engine implementation
│   ├── doom_graphics.h             # Graphics rendering
│   ├── doom_graphics.cpp            # Graphics implementation
│   ├── doom_input.h                # Input handling
│   ├── doom_input.cpp               # Input implementation
│   └── data/
│       └── doom1.wad                # Game data file
├── docs/
│   ├── SETUP_GUIDE.md              # Detailed setup instructions
│   ├── TROUBLESHOOTING.md          # Common issues and fixes
│   └── HARDWARE_GUIDE.md           # Hardware connection guide
├── tools/
│   ├── upload_helper.bat           # Windows upload script
│   └── esptool_commands.md         # Manual flash commands
└── README.md                        # This file

```

## 🎮 Controls

### Touch Screen Controls
The game uses virtual on-screen controls:

| Control | Action |
|---------|--------|
| D-Pad (Left) | Movement |
| A Button | Fire |
| B Button | Use/Open |
| X Button | Run |
| Y Button | Switch Weapon |
| Start | Menu |
| Select | Map |

### Control Layout
```
┌─────────────────────────────────┐
│  [↑]                    [Y] [X] │
│ [←][→]                  [B] [A] │
│  [↓]                            │
│                                 │
│        GAME DISPLAY AREA        │
│                                 │
│ [SELECT]              [START]   │
└─────────────────────────────────┘
```

## 📦 Quick Start

### Windows Users
1. Connect ESP32 board via USB
2. Run `tools/upload_helper.bat`
3. Follow on-screen instructions

### Manual Upload
```bash
# Compile
arduino-cli compile --fqbn esp32:esp32:esp32s3:PSRAM=opi,FlashSize=16M,PartitionScheme=huge_app esp32_doom_waveshare

# Upload
arduino-cli upload -p COM5 --fqbn esp32:esp32:esp32s3:PSRAM=opi,FlashSize=16M,PartitionScheme=huge_app esp32_doom_waveshare
```

## 🐛 Troubleshooting

### Common Issues

#### Upload Failed
- Press and hold BOOT button while uploading
- Check COM port in Device Manager
- Try lower upload speed (460800)

#### Compilation Errors
- Ensure all libraries are installed
- Check LVGL version (must be 8.3.x)
- Delete duplicate .ino files in folder

#### Display Issues
- Verify PSRAM is enabled (OPI PSRAM)
- Check partition scheme (Huge APP)
- Ensure 16MB Flash is selected

## 📊 Performance

| Metric | Value |
|--------|-------|
| Target FPS | 35 |
| Resolution | 320×240 (scaled to 800×480) |
| Memory Usage | ~4MB PSRAM |
| Flash Usage | ~3MB |
| Power Consumption | ~500mA @ 5V |

## 🎯 Features

- ✅ Full DOOM gameplay
- ✅ Touch screen controls
- ✅ Save/Load game
- ✅ Sound effects (PWM audio)
- ✅ All original levels
- ✅ 35 FPS gameplay
- ✅ PSRAM optimization

## 📝 WAD File Information

The project uses DOOM 1 Shareware WAD (doom1.wad):
- Size: 4,196,020 bytes
- MD5: f0cefca49926d00903cf57551d901abe
- Version: 1.9
- Episodes: 1 (Knee-Deep in the Dead)
- Levels: 9

## 🛠️ Development

### Building from Source
```bash
git clone https://github.com/hwkim3330/esp32doom7.git
cd esp32doom7
# Open esp32_doom_waveshare/esp32_doom_waveshare.ino in Arduino IDE
```

### Memory Management
The project uses PSRAM for:
- Frame buffer (320×240×2 bytes)
- WAD file caching
- Game state
- Sound buffers

### Optimization Techniques
- DMA transfers for display
- PSRAM burst read/write
- Optimized rendering pipeline
- Touch input debouncing

## 📚 References

### Based On
- [Chocolate Doom](https://github.com/chocolate-doom/chocolate-doom)
- [ESP32-DOOM](https://github.com/espressif/esp32-doom)
- [PrBoom](https://github.com/coelckers/prboom)

### Hardware Documentation
- [Waveshare ESP32-S3-Touch-LCD-7](https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-7)
- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [GT911 Touch Controller](https://github.com/goodix/gt911)

### Libraries Used
- [LVGL](https://github.com/lvgl/lvgl) - Light and Versatile Graphics Library
- [ESP32_Display_Panel](https://github.com/esp-arduino-libs/ESP32_Display_Panel)
- [Waveshare_ST7262_LVGL](https://github.com/iamfaraz/Waveshare_ST7262_LVGL)

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

DOOM is a registered trademark of id Software LLC. This project is not affiliated with id Software.

## 👤 Author

**hwkim3330**

## 🙏 Acknowledgments

- id Software for creating DOOM
- Espressif for ESP32 platform
- Waveshare for the display hardware
- LVGL team for the graphics library
- Community contributors

## ⭐ Star History

If you like this project, please give it a star!

---
*Last Updated: 2025-01-09*
