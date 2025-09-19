# 🔧 Troubleshooting Guide

## Common Issues and Solutions

### ❌ Compilation Errors

#### Error: `Waveshare_ST7262_LVGL.h: No such file or directory`
**Solution:**
- Install Waveshare library as ZIP
- Download from: https://github.com/iamfaraz/Waveshare_ST7262_LVGL
- Sketch → Include Library → Add .ZIP Library

#### Error: `size_t not declared`
**Solution:**
- Add to the top of affected files:
```cpp
#include <stddef.h>
```

#### Error: `lv_canvas_finish_layer not found`
**Solution:**
- LVGL version mismatch
- Ensure LVGL 8.3.x is installed (not 9.x)
- Remove the line or replace with:
```cpp
lv_obj_invalidate(doom_canvas);
```

#### Error: Multiple `.ino` files in folder
**Solution:**
- Keep only `esp32_doom_waveshare.ino`
- Move other `.ino` files to backup folder
- Or rename them to `.ino.bak`

### ⚠️ Upload Issues

#### Upload Failed: `Timed out waiting for packet header`
**Solution:**
1. Hold BOOT button while uploading
2. Lower upload speed to 460800 or 115200
3. Try different USB cable
4. Use rear USB ports (desktop)

#### Port Not Found
**Windows:**
```cmd
# Check ports
mode
# Install drivers
https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
```

**Linux:**
```bash
# Add user to dialout group
sudo usermod -a -G dialout $USER
# Logout and login again

# Check permissions
ls -l /dev/ttyACM*
```

**Mac:**
```bash
# List ports
ls /dev/tty.*
# May need to install drivers for some boards
```

#### Upload Stuck at "Connecting..."
1. Press and hold BOOT button
2. Press and release RESET button
3. Release BOOT button
4. Try upload again

### 🖥️ Display Problems

#### Black Screen
**Checklist:**
- [ ] PSRAM enabled (OPI PSRAM)
- [ ] Partition: Huge APP
- [ ] Flash: 16MB
- [ ] Power supply adequate (5V, 500mA+)

**Test Code:**
```cpp
void setup() {
  Serial.begin(115200);
  Serial.println("Board starting...");
  
  // Test PSRAM
  if (!psramFound()) {
    Serial.println("FATAL: No PSRAM!");
    while(1);
  }
  
  Serial.printf("PSRAM: %d bytes\n", ESP.getPsramSize());
  
  // Test display init
  // Your display init code here
}
```

#### Corrupted Display
- Wrong display driver settings
- Check `ESP_Panel_Board_Custom.h` configuration
- Verify RGB565 format

#### Touch Not Working
**Test Touch:**
```cpp
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Scan I2C
  Serial.println("I2C Scanner");
  for(byte addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if(Wire.endTransmission() == 0) {
      Serial.printf("Device at 0x%02X\n", addr);
    }
  }
}
```
Should find GT911 at 0x5D or 0x14

### 🎮 Game Issues

#### Low FPS
**Optimizations:**
- Ensure CPU at 240MHz
- PSRAM enabled
- Close Serial Monitor
- Reduce serial output

#### Game Crashes
**Memory Issues:**
```cpp
void printMemory() {
  Serial.printf("Free Heap: %d\n", ESP.getFreeHeap());
  Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());
}
```
If memory < 100KB, reduce buffer sizes

#### WAD File Not Loading
1. Check SPIFFS format:
```cpp
if(!SPIFFS.begin(true)) {
  Serial.println("SPIFFS Mount Failed");
}
File root = SPIFFS.open("/");
File file = root.openNextFile();
while(file) {
  Serial.println(file.name());
  file = root.openNextFile();
}
```

2. Verify WAD file:
- Size: 4,196,020 bytes
- Location: `data/doom1.wad`
- Upload via Tools → ESP32 Sketch Data Upload

### 🔌 Power Issues

#### Board Resets Randomly
- Insufficient power supply
- Use powered USB hub
- Or external 5V supply
- Check current draw (should be <500mA)

#### Brown-out Detector Triggered
Add to setup():
```cpp
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Disable brownout
  // Rest of setup...
}
```

### 🛠️ Advanced Debugging

#### Enable Debug Output
Tools → Core Debug Level → Verbose

#### Serial Debug Messages
```cpp
#define DEBUG 1

#if DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif
```

#### Stack Trace Decoder
1. Copy error with backtrace
2. Tools → ESP Exception Decoder
3. Paste and decode

#### Monitor Free Memory
```cpp
void setup() {
  xTaskCreate(
    memoryTask,
    "memTask",
    2048,
    NULL,
    1,
    NULL
  );
}

void memoryTask(void *pvParameters) {
  while(1) {
    Serial.printf("Free: %d/%d\n", 
      ESP.getFreeHeap(), 
      ESP.getFreePsram()
    );
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
```

### 📞 Getting Help

If issues persist:
1. Check GitHub Issues
2. Post detailed error with:
   - Board settings screenshot
   - Complete error message
   - Arduino IDE version
   - Library versions
3. Try example sketches first
4. Join ESP32 community forums

### Quick Fix Script

Save as `fix_common_issues.bat`:
```batch
@echo off
echo Fixing common ESP32 DOOM issues...
echo.
echo 1. Cleaning build cache...
rmdir /s /q %TEMP%\arduino_build_*
rmdir /s /q %TEMP%\arduino_cache_*

echo 2. Checking drivers...
pnputil /enum-drivers | findstr "USB Serial"

echo 3. Testing COM ports...
mode

echo.
echo Done! Try uploading again.
pause
```

---
*Still having issues? Open a GitHub issue with full error details!*