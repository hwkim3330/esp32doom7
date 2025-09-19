# 📖 Detailed Setup Guide for ESP32 DOOM

## Prerequisites Checklist

Before starting, ensure you have:
- [ ] Waveshare ESP32-S3-Touch-LCD-7 board
- [ ] USB Type-C cable
- [ ] Windows/Mac/Linux computer
- [ ] 500MB free disk space
- [ ] Internet connection for downloads

## Step-by-Step Installation

### 1️⃣ Arduino IDE Installation

#### Windows
1. Download Arduino IDE from https://www.arduino.cc/en/software
2. Run the installer as Administrator
3. Install to default location: `C:\Program Files\Arduino IDE`

#### Mac
1. Download Arduino IDE .dmg file
2. Drag to Applications folder
3. First run: Right-click → Open to bypass security

#### Linux
```bash
sudo snap install arduino
# or
sudo apt-get install arduino
```

### 2️⃣ ESP32 Board Package Setup

1. Open Arduino IDE
2. **File** → **Preferences** (Windows/Linux) or **Arduino IDE** → **Preferences** (Mac)
3. In "Additional Board Manager URLs" field, add:
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```
4. Click **OK**
5. **Tools** → **Board** → **Boards Manager**
6. Search for "esp32"
7. Install "esp32 by Espressif Systems" (version 3.0.0 or later)
8. Wait for download to complete (may take 5-10 minutes)

### 3️⃣ Library Installation

#### Via Library Manager
1. **Sketch** → **Include Library** → **Manage Libraries**
2. Install these libraries:
   - **LVGL** by LVGL (8.3.x version - IMPORTANT!)
   - **ESP32_Display_Panel** by ESP32_Display
   - **ESP32_IO_Expander** by ESP32

#### Manual Library Installation
1. Download Waveshare library:
   - Go to https://github.com/iamfaraz/Waveshare_ST7262_LVGL
   - Click "Code" → "Download ZIP"
2. In Arduino IDE:
   - **Sketch** → **Include Library** → **Add .ZIP Library**
   - Select downloaded ZIP file

### 4️⃣ Board Configuration

1. Connect ESP32 board to computer via USB
2. **Tools** menu settings:

| Setting | Value |
|---------|-------|
| Board | "ESP32S3 Dev Module" |
| USB CDC On Boot | "Enabled" |
| USB DFU On Boot | "Disabled" |
| Upload Mode | "UART0 / Hardware CDC" |
| USB Mode | "Hardware CDC and JTAG" |
| CPU Frequency | "240MHz (WiFi)" |
| Flash Mode | "QIO 80MHz" |
| Flash Size | "16MB (128Mb)" |
| Partition Scheme | "Huge APP (3MB No OTA/1MB SPIFFS)" |
| PSRAM | "OPI PSRAM" |
| Upload Speed | "921600" |
| Port | Select your COM port (e.g., COM5) |

### 5️⃣ Project Files Setup

1. Create project folder:
   ```
   C:\Users\[YourName]\Documents\Arduino\esp32_doom_waveshare
   ```

2. Copy all project files:
   - `esp32_doom_waveshare.ino`
   - All `.h` and `.cpp` files
   - `data` folder with `doom1.wad`

3. File structure should be:
   ```
   esp32_doom_waveshare/
   ├── esp32_doom_waveshare.ino
   ├── doom_engine.h
   ├── doom_engine.cpp
   ├── doom_graphics.h
   ├── doom_graphics.cpp
   ├── doom_input.h
   ├── doom_input.cpp
   └── data/
       └── doom1.wad
   ```

### 6️⃣ SPIFFS Data Upload

1. Install ESP32 SPIFFS Upload Tool:
   - Download from: https://github.com/me-no-dev/arduino-esp32fs-plugin/releases
   - Extract to `[Arduino IDE folder]/tools/ESP32FS/tool/esp32fs.jar`

2. Upload SPIFFS data:
   - **Tools** → **ESP32 Sketch Data Upload**
   - Wait for upload to complete

### 7️⃣ Compile and Upload

1. Open `esp32_doom_waveshare.ino` in Arduino IDE
2. Click **Verify** button (checkmark) to compile
3. If compilation successful, click **Upload** button (arrow)
4. If upload fails:
   - Hold **BOOT** button on board
   - Click Upload again
   - Release BOOT after upload starts

## Verification Steps

After successful upload:
1. Board should restart automatically
2. Display should show DOOM splash screen
3. Touch controls should be responsive
4. Game should run at ~35 FPS

## Port Detection (Windows)

If COM port not detected:
1. Open Device Manager
2. Look under "Ports (COM & LPT)"
3. Should see "USB Serial Device (COMx)"
4. If not visible:
   - Install CP2102 drivers from Silicon Labs
   - Or CH340 drivers if using that chip

## Memory Configuration

PSRAM is critical for DOOM to run. Verify with this test sketch:

```cpp
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  if (psramFound()) {
    Serial.printf("PSRAM found! Size: %d bytes\n", ESP.getPsramSize());
  } else {
    Serial.println("ERROR: PSRAM not found!");
  }
}

void loop() {}
```

Should output: "PSRAM found! Size: 8388608 bytes"

## Next Steps

- Read [TROUBLESHOOTING.md](TROUBLESHOOTING.md) if you encounter issues
- Check [HARDWARE_GUIDE.md](HARDWARE_GUIDE.md) for pin connections
- Join discussions at project GitHub page

---
*Need help? Open an issue on GitHub!*