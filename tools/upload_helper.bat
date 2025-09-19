@echo off
echo ========================================
echo    ESP32 DOOM - Upload Helper v1.0
echo ========================================
echo.

set PROJECT_PATH=..\esp32_doom_waveshare
set PORT=COM5
set BOARD=esp32:esp32:esp32s3

echo [1] Checking Arduino IDE...
where arduino-cli >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: arduino-cli not found!
    echo Please install Arduino CLI first.
    pause
    exit /b 1
)

echo [2] Board Configuration:
echo    Board: ESP32S3 Dev Module
echo    Port: %PORT%
echo    PSRAM: OPI PSRAM
echo    Flash: 16MB
echo    Partition: Huge APP
echo.

echo [3] Compiling...
arduino-cli compile --fqbn %BOARD%:PSRAM=opi,FlashSize=16M,PartitionScheme=huge_app "%PROJECT_PATH%"
if %errorlevel% neq 0 (
    echo.
    echo ERROR: Compilation failed!
    echo Check error messages above.
    pause
    exit /b 1
)

echo.
echo [4] Uploading to ESP32...
echo    Hold BOOT button if upload fails!
arduino-cli upload -p %PORT% --fqbn %BOARD%:PSRAM=opi,FlashSize=16M,PartitionScheme=huge_app "%PROJECT_PATH%"
if %errorlevel% neq 0 (
    echo.
    echo ERROR: Upload failed!
    echo Try:
    echo 1. Hold BOOT button and try again
    echo 2. Check USB connection
    echo 3. Verify COM port (%PORT%)
    pause
    exit /b 1
)

echo.
echo ========================================
echo    SUCCESS! DOOM is running!
echo ========================================
echo.
echo Touch controls:
echo - D-Pad: Movement
echo - FIRE: Shoot
echo - OPEN: Use/Open doors
echo - RUN: Sprint
echo - MENU: Game menu
echo.
pause