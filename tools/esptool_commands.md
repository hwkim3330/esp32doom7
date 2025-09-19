# ESPTool Manual Flash Commands

For advanced users who want to flash the ESP32 directly using esptool.

## Prerequisites
- Python installed
- esptool installed: `pip install esptool`

## Flash Commands

### 1. Check ESP32 Connection
```bash
esptool.exe --port COM5 --chip esp32s3 flash_id
```

### 2. Erase Flash (Optional - Clean Install)
```bash
esptool.exe --port COM5 --chip esp32s3 erase_flash
```

### 3. Flash Bootloader
```bash
esptool.exe --port COM5 --chip esp32s3 --baud 921600 write_flash \
  0x0 bootloader.bin
```

### 4. Flash Partition Table
```bash
esptool.exe --port COM5 --chip esp32s3 --baud 921600 write_flash \
  0x8000 partitions.bin
```

### 5. Flash Application
```bash
esptool.exe --port COM5 --chip esp32s3 --baud 921600 write_flash \
  0x10000 esp32_doom_waveshare.ino.bin
```

### 6. Flash SPIFFS with WAD File
```bash
esptool.exe --port COM5 --chip esp32s3 --baud 921600 write_flash \
  0x400000 spiffs.bin
```

## Complete Flash Command
All in one command:
```bash
esptool.exe --port COM5 --chip esp32s3 --baud 921600 write_flash \
  --flash_mode dio --flash_size 16MB --flash_freq 80m \
  0x0 bootloader.bin \
  0x8000 partitions.bin \
  0x10000 esp32_doom_waveshare.ino.bin \
  0x400000 spiffs.bin
```

## Binary File Locations

After Arduino compilation, binaries are located in:
```
%TEMP%\arduino_build_[random]\
```

Or if using Arduino CLI:
```
build\esp32.esp32.esp32s3\
```

## Creating SPIFFS Image

### Install mkspiffs
Download from: https://github.com/igrr/mkspiffs/releases

### Create SPIFFS Binary
```bash
mkspiffs -c data -b 4096 -p 256 -s 0x100000 spiffs.bin
```

Where:
- `-c data`: Source folder containing doom1.wad
- `-b 4096`: Block size
- `-p 256`: Page size
- `-s 0x100000`: SPIFFS partition size (1MB)

## Troubleshooting

### If Upload Fails
1. Hold BOOT button while flashing
2. Press RESET while holding BOOT
3. Release BOOT after "Connecting..." appears

### Wrong Chip Detection
Force chip type:
```bash
esptool.exe --port COM5 --chip esp32s3 --before default_reset flash_id
```

### Permission Denied (Linux/Mac)
```bash
sudo chmod 666 /dev/ttyUSB0
# or add user to dialout group
sudo usermod -a -G dialout $USER
```

## Monitor Serial Output
```bash
esptool.exe --port COM5 --chip esp32s3 --baud 115200 read_flash 0x0 0x10 -
```

Or use Arduino Serial Monitor at 115200 baud.

## Backup Current Flash
```bash
esptool.exe --port COM5 --chip esp32s3 --baud 921600 read_flash \
  0x0 0x1000000 backup.bin
```

This creates a 16MB backup of the entire flash.