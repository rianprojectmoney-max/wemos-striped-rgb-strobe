# 📋 Complete Installation & Setup Guide

## Table of Contents
1. [Hardware Assembly](#hardware-assembly)
2. [Arduino IDE Setup](#arduino-ide-setup)
3. [Firmware Upload](#firmware-upload)
4. [Initial Configuration](#initial-configuration)
5. [Web Interface Guide](#web-interface-guide)
6. [WiFi Customization](#wifi-customization)
7. [Scheduling Setup](#scheduling-setup)
8. [Troubleshooting](#troubleshooting)

---

## Hardware Assembly

### Components Required
```
✓ Wemos D1 Mini (ESP8266)
✓ 4x WS2812B LED Strips:
  - Strip 1: 9 LEDs (top)
  - Strip 2: 4 LEDs (middle-left)
  - Strip 3: 4 LEDs (middle-right)
  - Strip 4: 9 LEDs (bottom)
✓ 5V Power Supply (3A minimum)
✓ 4x 470Ω Resistors (data line protection)
✓ Micro USB Cable
✓ Jumper Wires
✓ Breadboard or PCB (optional)
```

### Wiring Diagram

```
Wemos D1 Mini Pin Layout:

      USB
   [Micro USB]
    |_______|
    |D1  D2|
    |GND D3|
    |3.3V D4|----[470Ω]----DIN Strip 1 (9 LEDs)
    |5V  D5|----[470Ω]----DIN Strip 4 (9 LEDs)
    |GND D6|----[470Ω]----DIN Strip 2 (4 LEDs)
    |RST D7|----[470Ω]----DIN Strip 3 (4 LEDs)
    |D8 D0|
    |_____|
    
5V Power Supply:
+5V ----[Resistor]-----> Data Lines (through 470Ω)
GND -----> GND (all strips)
GND -----> GND (Wemos)
```

### Step-by-Step Assembly

#### Step 1: Prepare Power Distribution
```
1. Connect 5V+ from power supply to:
   - Wemos 5V pin
   - VCC pin of all 4 LED strips

2. Connect GND from power supply to:
   - Wemos GND pin
   - GND pin of all 4 LED strips
```

#### Step 2: Connect Data Lines with Protection Resistors
```
For each LED strip:
  D-Pin from Wemos --[470Ω Resistor]-- DIN pin of LED strip

Mapping:
  D4 --[470Ω]-- Strip 1 DIN
  D6 --[470Ω]-- Strip 2 DIN
  D7 --[470Ω]-- Strip 3 DIN
  D5 --[470Ω]-- Strip 4 DIN
```

#### Step 3: Verify Connections
```
□ All 5V connections secure
□ All GND connections secure
□ All data resistors properly connected
□ No loose wires
□ Power supply OFF before plugging in
```

#### Step 4: Power Up Test
```
1. Connect 5V power supply (do NOT plug USB yet)
2. Check for any LED activity (should be off)
3. Plug in Micro USB to Wemos
4. Should see WiFi network "STRIPED-RGB-STROBE"
```

---

## Arduino IDE Setup

### Prerequisites
- Windows 7+, macOS, or Linux
- Arduino IDE 1.8.19+ or Arduino IDE 2.0+
- USB drivers for CH340/CH341 (Wemos chip)

### Installation Steps

#### Step 1: Install Arduino IDE
1. Download from [arduino.cc](https://www.arduino.cc/en/software)
2. Install following on-screen instructions
3. Launch Arduino IDE

#### Step 2: Add ESP8266 Board Support

**For Arduino IDE 1.8.x:**
```
1. File → Preferences
2. Additional Boards Manager URLs:
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
3. Click OK
4. Tools → Board → Boards Manager
5. Search "ESP8266"
6. Click "esp8266 by ESP8266 Community"
7. Select version 3.1.2 (recommended)
8. Click Install
9. Wait for completion (~300MB)
```

**For Arduino IDE 2.0+:**
```
1. File → Preferences
2. Settings → Additional Boards Manager URLs:
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
3. Save
4. Tools → Boards → Boards Manager
5. Search "esp8266"
6. Click esp8266 by ESP8266 Community
7. Select version 3.1.2
8. Install
```

#### Step 3: Install Required Libraries

**FastLED Library:**
```
1. Sketch → Include Library → Manage Libraries
2. Search: "FastLED"
3. Select "FastLED by Daniel Garcia"
4. Version: 3.5.0 or latest
5. Click Install
```

**ArduinoJson Library:**
```
1. Sketch → Include Library → Manage Libraries
2. Search: "ArduinoJson"
3. Select "ArduinoJson by Benoit Blanchon"
4. Version: 6.20.0 or latest
5. Click Install
```

#### Step 4: Install CH340 USB Drivers

**Windows:**
```
1. Download driver: CH341SER.ZIP from WCH website
2. Extract and run CH341SER.EXE
3. Click Install
4. Restart computer
```

**macOS:**
```
1. Download: CH34x_Install_V1.5.pkg
2. Run installer
3. Restart Mac
```

**Linux:**
```
1. Install via package manager:
   sudo apt install ch341-dkms
2. Or compile from source (usually unnecessary)
```

#### Step 5: Configure Board Settings

**Tools Menu Configuration:**
```
Board:              LOLIN(WEMOS) D1 R2 & mini
Flash Size:         4M (3M SPIFFS)
LwIP Variant:       v2 Lower Memory (Recommended)
CPU Frequency:      160 MHz
Upload Speed:       921600
Port:               COM3, COM4, COM5... (varies by system)
```

**How to Select Port:**
```
Windows:
  1. Device Manager → Ports (COM & LPT)
  2. Look for "CH340" or "USB Serial"
  3. Note the COM number (e.g., COM3)
  4. Tools → Port → COM3

macOS:
  1. Tools → Port → /dev/cu.wchusbserial####

Linux:
  1. Tools → Port → /dev/ttyUSB0
```

---

## Firmware Upload

### Step 1: Download Firmware

```
Download the complete firmware from GitHub repository:
rianprojectmoney-max/wemos-striped-rgb-strobe

Required files:
  • wemos-striped-rgb-strobe.ino
  • animations_advanced.h (optional, for advanced effects)
```

### Step 2: Open in Arduino IDE

```
1. Copy firmware code
2. Arduino IDE → Sketch → New
3. Paste entire firmware code
4. Save as "wemos-striped-rgb-strobe.ino"
```

### Step 3: Verify Configuration

Before uploading, verify these settings in firmware:

```cpp
// WiFi Settings (can be changed later via web)
const char* ssid = "STRIPED-RGB-STROBE";
const char* password = "admin123";
const char* webPassword = "admin123";

// LED Pins (match your wiring)
#define STRIP1_PIN D4    // Top strip
#define STRIP2_PIN D6    // Middle-left strip
#define STRIP3_PIN D7    // Middle-right strip
#define STRIP4_PIN D5    // Bottom strip

// LED Counts (must match physical setup)
#define STRIP1_LEDS 9    // Top
#define STRIP2_LEDS 4    // Middle-left
#define STRIP3_LEDS 4    // Middle-right
#define STRIP4_LEDS 9    // Bottom
```

### Step 4: Compile

```
1. Click Sketch → Verify/Compile
   OR press Ctrl+R (Windows/Linux) / Cmd+R (Mac)
2. Wait for "Compiling sketch..." to complete
3. Should see: "Sketch uses XXXXX bytes (XX%) of program storage space"
4. If errors appear, fix them and try again
```

### Step 5: Upload

**Normal Upload (if COM port detected):**
```
1. Click Sketch → Upload
   OR press Ctrl+U (Windows/Linux) / Cmd+U (Mac)
2. Arduino IDE will:
   - Compile again
   - Put Wemos into bootloader mode
   - Upload firmware
   - Reset Wemos
3. Watch for "Leaving...", "Elf magic not found", then "Done"
```

**If Upload Fails:**
```
1. Hold down FLASH button on Wemos
2. Click Upload in IDE
3. Keep holding FLASH button
4. Release when you see "Connecting..."
5. Wait for upload to complete
```

### Step 6: Verify Upload

```
1. Open Serial Monitor: Tools → Serial Monitor
2. Set baud rate to 115200
3. Press RST button on Wemos
4. Should see boot messages
5. Look for:
   "Wemos D1 Mini - Striped RGB Strobe Module"
   "Ready for control!"
   "AP Started: STRIPED-RGB-STROBE"
   "IP: 192.168.4.1"
```

---

## Initial Configuration

### First Boot Checklist

```
✓ Firmware uploaded successfully
✓ Serial monitor shows startup messages
✓ All 26 LEDs powered (check voltage at strips)
✓ WiFi network visible ("STRIPED-RGB-STROBE")
```

### Connect to WiFi

**Windows/Mac/Linux:**
```
1. Settings → WiFi / Network
2. Look for: "STRIPED-RGB-STROBE"
3. Connect with password: "admin123"
4. Wait for connection (2-3 seconds)
```

**Mobile Device:**
```
1. Settings → WiFi
2. Select: "STRIPED-RGB-STROBE"
3. Password: "admin123"
4. Connected!
```

### Access Web Interface

```
1. Open web browser (Chrome, Firefox, Safari, Edge)
2. Type in address bar: 192.168.4.1
3. You should see login page
4. Enter password: admin123
5. You're in!
```

---

## Web Interface Guide

### Login Page
```
Password Field: Enter "admin123"
Click: Login button
```

### Main Control Panel

#### Animation Mode (1-210)
- **Input**: Number field 1-210
- **Description**: Select which animation to play
- **Modes**:
  - 1-50: Solid colors
  - 51-80: Breathing effects
  - 81-110: Pulsing
  - 111-139: Alternating strips
  - 140-175: 2-Color strobes
  - 176-208: 5-Color strobes
  - 209: Color-change mode
  - 210: Auto-cycling 5-stripe

#### Brightness (0-255)
- **Slider**: 0-255
- **0**: LEDs off
- **128**: 50% brightness
- **255**: Full brightness
- **Tip**: Lower brightness = faster updates

#### Speed (0-255)
- **Slider**: 0-255
- **0**: Slowest animation
- **128**: Medium speed
- **255**: Fastest animation
- **Tip**: Different effects have different speed ranges

#### Color Palette (0-19)
- **Dropdown**: 20 colors available
- **Mode 209**: Uses selected color
- **Colors include**: Red, Blue, Green, Cyan, Magenta, Purple, Pink, etc.

#### Buttons
- **Apply**: Save all current settings
- **Toggle Animation**: Pause/resume animation

### Preset Management

#### Save as Preset
```
1. Set desired mode, brightness, speed, color
2. Enter name in "Preset Name" field
3. Click "Save as Preset"
4. Preset stored in device memory
5. Will survive power loss
```

#### Load Preset
```
1. Click preset button in preset grid
2. Settings automatically load
3. Click "Apply" to activate
```

#### Quick Presets Grid
```
- Shows up to 20 preset buttons
- Click any button to quick-load
- Each shows preset name
- Color-coded with border highlight
```

---

## WiFi Customization

### Change WiFi SSID & Password

#### Method 1: Via Web Interface (Easiest)

```
1. Log into web interface (192.168.4.1)
2. Click "Settings" or "WiFi Config" section
3. Edit SSID field: "MY-CUSTOM-NETWORK"
4. Edit Password field: "MySecurePassword123"
5. Click "Save WiFi Settings"
6. Device will restart
7. Connect to new network
```

#### Method 2: Edit Firmware (Before Upload)

```cpp
// Find these lines in firmware:
const char* ssid = "STRIPED-RGB-STROBE";
const char* password = "admin123";
const char* webPassword = "admin123";

// Change to:
const char* ssid = "MY-LED-STROBE";
const char* password = "SecurePass123";
const char* webPassword = "WebPass456";

// Re-upload firmware
```

### Reset WiFi to Defaults

```
1. Unplug power for 5 seconds
2. Plug back in
3. During startup, press RST button 3 times rapidly
4. LEDs will flash (reset signal)
5. Device resets to:
   SSID: "STRIPED-RGB-STROBE"
   Password: "admin123"
```

### Change Web Login Password

**Via Web Interface:**
```
1. Log into 192.168.4.1
2. Go to Settings → Security
3. Current Password: admin123
4. New Password: YourNewPassword
5. Confirm Password: YourNewPassword
6. Click "Update Password"
```

**Via Firmware Edit:**
```cpp
const char* webPassword = "admin123";  // Change this
const char* webPassword = "MyNewPass123";  // To this
```

---

## Scheduling Setup

### Time-Based Animations

#### Configure Automatic Schedule

```
1. Log into web interface
2. Click "Scheduling" or "Timer" section
3. Click "Add Schedule" button
```

#### Schedule Options

**Time-based Schedule:**
```
┌─ Schedule Name ─────────────────┐
│ Enter descriptive name          │
│ E.g., "Evening Party Mode"      │
└─────────────────────────────────┘

┌─ Start Time ────────────────────┐
│ Hour: [00-23] Minutes: [00-59]  │
│ E.g., 19:30 (7:30 PM)           │
└─────────────────────────────────┘

┌─ End Time ──────────────────────┐
│ Hour: [00-23] Minutes: [00-59]  │
│ E.g., 23:59 (11:59 PM)          │
└─────────────────────────────────┘

┌─ Days of Week ──────────────────┐
│ □ Monday      □ Thursday        │
│ □ Tuesday     □ Friday          │
│ □ Wednesday   □ Saturday        │
│               □ Sunday          │
└─────────────────────────────────┘

┌─ Animation Settings ────────────┐
│ Mode: [______]  (1-210)         │
│ Brightness: [======] (0-255)    │
│ Speed: [======] (0-255)         │
│ Color: [________]  (0-19)       │
└─────────────────────────────────┘
```

#### Example Schedules

**Party Mode (Evening)**
```
Name: Evening Party
Start: 18:00 (6:00 PM)
End: 23:59 (11:59 PM)
Days: Fri, Sat, Sun
Mode: 210 (Auto-cycling 5-stripe)
Brightness: 255
Speed: 200
Color: Auto
```

**Sleep Mode (Night)**
```
Name: Night Mode
Start: 23:00 (11:00 PM)
End: 06:00 (6:00 AM)
Days: Mon-Sun
Mode: 1 (Red solid)
Brightness: 50
Speed: 0
Color: Deep Red
```

**Work Mode (Day)**
```
Name: Office Mode
Start: 08:00 (8:00 AM)
End: 17:00 (5:00 PM)
Days: Mon-Fri
Mode: 51 (Breathing)
Brightness: 150
Speed: 80
Color: Blue
```

### Managing Schedules

#### View All Schedules
```
1. Click "Scheduling" section
2. All active schedules listed
3. Shows: Time, Days, Mode, Status
```

#### Edit Schedule
```
1. Click schedule name
2. Edit any field
3. Click "Update Schedule"
4. Changes take effect immediately
```

#### Delete Schedule
```
1. Click schedule name
2. Click "Delete" button
3. Confirm deletion
4. Schedule removed
```

#### Enable/Disable Schedule
```
1. Toggle switch next to schedule
2. Green = Enabled
3. Gray = Disabled
4. Changes save automatically
```

### Advanced Scheduling Features

#### Transition Time
```
Set fade duration between modes:
• Instant (0 sec)
• Slow fade (5 seconds)
• Smooth transition (10 seconds)
```

#### Overlay Schedules
```
If multiple schedules overlap:
• First schedule takes priority
• Or blend both animations
• Configure in settings
```

#### Sync with NTP (Internet Time)
```
1. Settings → Time
2. Enable "Sync with NTP Server"
3. Select timezone
4. Device syncs automatically
5. Schedules use accurate time
```

---

## Troubleshooting

### LEDs Not Lighting Up

**Check 1: Power Supply**
```
□ Power supply plugged in?
□ 5V output (use multimeter)
□ Amp rating 3A+?
□ All GND connections secure?
```

**Check 2: Data Lines**
```
□ All 4 data lines connected?
□ 470Ω resistors in place?
□ No loose connections?
□ Correct pins (D4, D5, D6, D7)?
```

**Check 3: Firmware**
```
□ Compilation errors?
□ Upload completed? ("Leaving..." message)
□ Correct LED count (#define values)?
□ Try different animation mode
```

**Fix: Reset Everything**
```
1. Turn off power supply
2. Disconnect USB
3. Wait 10 seconds
4. Reconnect USB only (no power supply)
5. Open serial monitor
6. Press RST button
7. Watch for startup messages
```

### WiFi Won't Connect

**Check 1: WiFi Network**
```
□ See "STRIPED-RGB-STROBE" network in WiFi list?
□ Signal strength > 2 bars?
□ Try forgetting and reconnecting
□ Restart router
```

**Check 2: Password**
```
□ Correct password: "admin123"
□ Check caps lock
□ No extra spaces
```

**Check 3: Device**
```
□ Serial monitor shows "AP Started"?
□ Device powered properly?
□ Try power cycle (unplug 10 seconds)
```

**Fix: Reset WiFi**
```
1. Unplug device
2. Wait 5 seconds
3. Plug back in
4. Press RST button 3 times quickly
5. LEDs flash = reset successful
6. Connect to "STRIPED-RGB-STROBE"
```

### Web Interface Not Loading

**Check 1: Connection**
```
□ Connected to "STRIPED-RGB-STROBE" WiFi?
□ 192.168.4.1 address correct?
□ Device powered on?
□ Try different browser
```

**Check 2: Browser**
```
□ JavaScript enabled?
□ Clear browser cache (Ctrl+Shift+Delete)
□ Try incognito/private window
□ Update browser to latest version
```

**Check 3: Device**
```
□ Serial monitor shows startup?
□ Reset device (press RST button)
□ Flash size 4M with 3M SPIFFS?
```

**Fix: Force Reload**
```
1. Open http://192.168.4.1
2. Press Ctrl+Shift+R (force reload)
3. Wait 5 seconds
4. Try again
```

### Animations Seem Choppy

**Cause: Insufficient Power**
```
1. Reduce brightness (saves power)
2. Use quality 5V 3A+ supply
3. Shorter wires if possible
4. Check voltage at LEDs (should be ~5.0V)
```

**Cause: High CPU Load**
```
1. Reduce animation speed
2. Use simpler animations (modes 1-50)
3. Check if WiFi is busy
4. Restart device
```

### Upload Keeps Failing

**Check 1: Board & Port**
```
□ Board: LOLIN(WEMOS) D1 R2 & mini?
□ Port: Correct COM/USB port?
□ USB cable working? (try different cable)
□ CH340 drivers installed?
```

**Check 2: Bootloader Mode**
```
1. Disconnect USB
2. Hold FLASH button
3. Plug USB while holding
4. Release FLASH button
5. Try upload again
```

**Check 3: Clear Flash**
```
1. Tools → ESP8266 Sketch Data Upload
2. This formats SPIFFS
3. Then try uploading firmware again
```

### Password Reset

**Emergency Reset:**
```
1. Find the EEPROM reset function
2. Or re-upload firmware with default password
3. Default: "admin123"
```

### Check Device Logs

**Serial Monitor:**
```
1. Tools → Serial Monitor
2. Baud Rate: 115200
3. Press RST button on Wemos
4. Watch for error messages
5. Copy error text for debugging
```

---

## Support & Tips

### Useful Commands via Serial

```
Serial Monitor (115200 baud):
- Device starts up
- Shows WiFi network created
- Shows IP address
- Ready for web access
```

### Best Practices

```
✓ Always use 5V dedicated power supply (3A+)
✓ Avoid long data line runs (use shorter wires)
✓ Keep resistors on all data lines
✓ Ensure good GND connection
✓ Update firmware regularly
✓ Back up important presets
✓ Test each strip individually first
```

### Performance Tips

```
✓ Reduce brightness for faster refresh
✓ Use solid colors for maximum reliability
✓ Limit simultaneous effects
✓ Enable WiFi power save if unused
✓ Regular device restart (weekly)
```

---

## Next Steps

1. ✅ Complete hardware assembly
2. ✅ Upload firmware
3. ✅ Access web interface
4. 🔄 Create custom presets
5. 🔄 Set up schedules
6. 🎨 Customize colors
7. 📤 Share with friends!

---

**Need Help?** Check GitHub issues or review serial monitor logs for diagnostic info.
