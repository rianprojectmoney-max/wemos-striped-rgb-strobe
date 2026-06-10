# Wemos D1 Mini - 210 Animation Striped RGB Strobe Module

A trendy TikTok-style LED strobe module using Wemos D1 Mini with 4 WS2812B LED strips (26 total LEDs) featuring **210 different animations**, web-based control, and **20 customizable welcome presets**.

## 🎨 Hardware Setup

### Wiring Configuration
```
Wemos D1 Mini Pinout:
├─ D4 → Strip 1 (Top 9 LEDs) DIN
├─ D6 → Strip 2 (Middle-Left 4 LEDs) DIN
├─ D7 → Strip 3 (Middle-Right 4 LEDs) DIN
└─ D5 → Strip 4 (Bottom 9 LEDs) DIN
```

**LED Strip Layout:**
```
┌─────────────────────┐
│   Strip 1: 9 LEDs   │  (Top)
├──────┬──────────┬──┤
│ S2:4 │  (Gap)   │S3:4│  (Middle)
├──────┴──────────┴──┤
│   Strip 4: 9 LEDs   │  (Bottom)
└─────────────────────┘
```

### Component List
- 1× Wemos D1 Mini (ESP8266)
- 4× WS2812B LED Strips (9+4+4+9 LEDs)
- 1× 5V Power Supply (3A recommended)
- 4× 470Ω Resistors (data line protection)
- 1× Micro USB cable (programming)

## ✨ Features

### 210 Animation Modes
- **Modes 1-50**: Solid colors (20 colors × variations)
- **Modes 51-80**: Breathing effects
- **Modes 81-110**: Pulsing animations
- **Modes 111-139**: Alternating strip patterns
- **Modes 140-175**: 2-Stripe strobe effects
- **Modes 176-208**: 5-Color multi-stripe patterns (NO yellow, NO 2-color flows)
- **Mode 209**: Interactive color-change mode (customizable)
- **Mode 210**: Auto-cycling 5-stripe rainbow-free pattern

### Color Palette
🎨 **No Yellow | No Rainbow Gradients | 5+ Colors for Flowing Patterns**

- Red, Blue, Green, Cyan, Magenta, Purple
- Deep Pink (#FF147F), Orange Red, Dark Magenta
- Blue Violet, Crimson, Indigo, Spring Green
- Aquamarine, Dodger Blue, and more...

### 🔐 Web Control Interface
- **Password-Protected Dashboard** (default: `admin123`)
- **Real-Time Animation Preview**
- **20 Customizable Welcome Presets** (save/load)
- **Mode Selection** (1-210)
- **Speed Control** (0-255)
- **Brightness Control** (0-255)
- **Color Picker** (from 20-color palette)

### 💾 Persistent Storage
- Presets saved to SPIFFS (survives power loss)
- JSON-based configuration
- Auto-backup on save

## 🚀 Quick Start

### Arduino IDE Setup

1. **Install ESP8266 Board Support**:
   - File → Preferences → Additional Boards URL:
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
   - Tools → Board Manager → Search "ESP8266" → Install

2. **Install Required Libraries**:
   ```
   FastLED 3.5.0+
   ArduinoJson 6.20.0+
   ```
   (Sketch → Include Library → Manage Libraries → Search & Install)

3. **Board Configuration**:
   ```
   Board: LOLIN(WEMOS) D1 R2 & mini
   CPU Frequency: 160 MHz
   Flash Size: 4M (3M SPIFFS)
   Upload Speed: 921600
   ```

4. **Upload Firmware**:
   - Copy `firmware/wemos-striped-rgb-strobe.ino` content
   - Paste into Arduino IDE
   - Connect Wemos D1 Mini via USB
   - Click Upload

### Access Web Interface

1. **Power on the device**
2. **Connect to WiFi**:
   - SSID: `STRIPED-RGB-STROBE`
   - Password: `admin123`
3. **Open Browser**:
   ```
   http://192.168.4.1
   ```
4. **Login**:
   - Password: `admin123`

## 🎮 Web Interface Usage

### Main Controls
- **Mode**: Select animation (1-210)
- **Brightness**: 0-255 (LED intensity)
- **Speed**: 0-255 (animation speed)
- **Color**: Select from 20-color palette

### Preset Management
- **Save as Preset**: Store current settings
- **Load Preset**: Apply saved configuration
- **Quick Presets**: 20 preset buttons for instant access

## 🎨 Animation Categories

### Solid Colors (Modes 1-50)
```
Mode 1:  Red
Mode 2:  Blue
Mode 3:  Green
Mode 4:  Cyan
Mode 5:  Magenta
Mode 6:  Purple
Mode 7:  Deep Pink
Mode 8:  White
... and more
```

### Breathing Effects (Modes 51-80)
Smooth color fading in and out

### Pulsing (Modes 81-110)
On/off pulsing animations

### Alternating Strips (Modes 111-139)
Alternate between two colors across strips

### Strobe Patterns (Modes 140-208)
- **140-175**: 2-Stripe strobes (alternating patterns)
- **176-208**: 5-Color multi-stripe (fast cycling through 5 colors)

### Special Modes
- **209**: User color selector with breathing effect
- **210**: Auto-cycling 5-stripe pattern (TikTok trending style)

## 📋 File Structure

```
wemos-striped-rgb-strobe/
├── firmware/
│   └── wemos-striped-rgb-strobe.ino    # Complete firmware (all features)
├── README.md                            # This file
└── LICENSE                              # MIT License
```

## 🔧 Customization

### Add New Colors
Edit the `colorPalette` array in the firmware:
```cpp
const CRGB colorPalette[] = {
  CRGB::Red,
  CRGB::Blue,
  CRGB(R, G, B),  // Custom RGB color
  // ...
};
```

### Modify Animation Patterns
Edit the `playAnimation()` function to change how each mode works.

### Change WiFi Credentials
```cpp
const char* ssid = "STRIPED-RGB-STROBE";
const char* password = "admin123";
const char* webPassword = "admin123";
```

### Adjust LED Pin Assignments
```cpp
#define STRIP1_PIN D4  // Change to your pin
#define STRIP2_PIN D6
#define STRIP3_PIN D7
#define STRIP4_PIN D5
```

## 🐛 Troubleshooting

### LEDs not lighting up
- Check power supply (5V, 3A recommended)
- Verify data pin connections
- Check resistors on data lines
- Ensure FastLED library is installed

### WiFi won't connect
- Check SSID/password match in code
- Try connecting to `192.168.4.1` directly
- Restart Wemos D1 Mini

### Web interface not loading
- Verify browser is on same WiFi network
- Try `http://192.168.4.1` in address bar
- Check if JavaScript is enabled in browser

### Animations seem slow/choppy
- Reduce brightness to improve refresh rate
- Increase speed value
- Check power supply capacity

## 📡 API Endpoints

```
GET  /api/status                    - Get current settings
GET  /api/control?mode=1&...        - Update animation
GET  /api/presets                   - List all presets
POST /api/preset/save               - Save new preset
GET  /api/preset/load?index=0       - Load preset
```

## 📝 License

MIT License - Feel free to modify and distribute

## 🌟 Credits

Inspired by trending TikTok striped RGB strobe modules. Built with FastLED and ESP8266.

---

**Made with ✨ for LED enthusiasts**
