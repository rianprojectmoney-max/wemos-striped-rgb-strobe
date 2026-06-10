#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <EEPROM.h>

// LED Configuration
#define NUM_STRIPS 4
#define STRIP1_PIN D4
#define STRIP2_PIN D6
#define STRIP3_PIN D7
#define STRIP4_PIN D5

#define STRIP1_LEDS 9
#define STRIP2_LEDS 4
#define STRIP3_LEDS 4
#define STRIP4_LEDS 9
#define TOTAL_LEDS (STRIP1_LEDS + STRIP2_LEDS + STRIP3_LEDS + STRIP4_LEDS)

// LED Arrays
CRGB leds1[STRIP1_LEDS];
CRGB leds2[STRIP2_LEDS];
CRGB leds3[STRIP3_LEDS];
CRGB leds4[STRIP4_LEDS];

// WiFi & Web Server
const char* ssid = "STRIPED-RGB-STROBE";
const char* password = "admin123";
const char* webPassword = "admin123";
ESP8266WebServer server(80);

// Global Variables
uint16_t currentMode = 1;
uint8_t brightness = 255;
uint8_t speed = 128;
uint8_t selectedColor = 0;
bool isAnimating = true;
unsigned long lastUpdate = 0;
unsigned long updateInterval = 50;

// Color Palette (avoiding yellow and rainbow)
const CRGB colorPalette[] = {
  CRGB::Red,          // 0
  CRGB::Blue,         // 1
  CRGB::Green,        // 2
  CRGB::Cyan,         // 3
  CRGB::Magenta,      // 4
  CRGB::Purple,       // 5
  CRGB(255, 20, 147), // Deep Pink - 6
  CRGB::White,        // 7
  CRGB(255, 69, 0),   // Orange Red - 8
  CRGB(139, 0, 139),  // Dark Magenta - 9
  CRGB(255, 192, 203),// Pink - 10
  CRGB(30, 144, 255), // Dodger Blue - 11
  CRGB(0, 206, 209),  // Dark Turquoise - 12
  CRGB(138, 43, 226), // Blue Violet - 13
  CRGB(220, 20, 60),  // Crimson - 14
  CRGB(75, 0, 130),   // Indigo - 15
  CRGB(199, 21, 133), // Medium Violet Red - 16
  CRGB(0, 255, 127),  // Spring Green - 17
  CRGB(50, 205, 50),  // Lime Green - 18
  CRGB(127, 255, 212) // Aquamarine - 19
};
const uint8_t PALETTE_SIZE = sizeof(colorPalette) / sizeof(colorPalette[0]);

// 5-Color Strobe Patterns (no yellow, no 2-color flows)
const CRGB strobePattern5Color1[] = {CRGB::Red, CRGB::Blue, CRGB::Green, CRGB::Magenta, CRGB::Cyan};
const CRGB strobePattern5Color2[] = {CRGB::Purple, CRGB(255, 20, 147), CRGB(30, 144, 255), CRGB(138, 43, 226), CRGB(220, 20, 60)};
const CRGB strobePattern5Color3[] = {CRGB::Magenta, CRGB(0, 206, 209), CRGB(199, 21, 133), CRGB(75, 0, 130), CRGB(255, 69, 0)};
const CRGB strobePattern5Color4[] = {CRGB::Cyan, CRGB(50, 205, 50), CRGB(255, 192, 203), CRGB(30, 144, 255), CRGB::Red};
const CRGB strobePattern5Color5[] = {CRGB(138, 43, 226), CRGB::Green, CRGB(220, 20, 60), CRGB(0, 206, 209), CRGB::Blue};

struct Preset {
  uint16_t mode;
  uint8_t brightness;
  uint8_t speed;
  uint8_t color;
  char name[32];
};

Preset presets[20];

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // Initialize LEDs
  FastLED.addLeds<WS2812B, STRIP1_PIN, GRB>(leds1, STRIP1_LEDS);
  FastLED.addLeds<WS2812B, STRIP2_PIN, GRB>(leds2, STRIP2_LEDS);
  FastLED.addLeds<WS2812B, STRIP3_PIN, GRB>(leds3, STRIP3_LEDS);
  FastLED.addLeds<WS2812B, STRIP4_PIN, GRB>(leds4, STRIP4_LEDS);
  FastLED.setBrightness(brightness);
  
  // Initialize SPIFFS
  if (!LittleFS.begin()) {
    Serial.println("LittleFS initialization failed!");
  }
  
  // Load presets
  loadPresetsFromFS();
  
  // Setup WiFi AP
  setupWiFi();
  
  // Setup Web Server
  setupWebServer();
  
  Serial.println("\nWemos D1 Mini - Striped RGB Strobe Module");
  Serial.println("Ready for control!");
}

void loop() {
  server.handleClient();
  
  if (isAnimating && millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();
    updateInterval = map(speed, 0, 255, 200, 10);
    playAnimation(currentMode);
    FastLED.show();
  }
}

void setupWiFi() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print("AP Started: ");
  Serial.println(ssid);
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());
}

void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/api/status", handleStatus);
  server.on("/api/control", handleControl);
  server.on("/api/presets", handlePresets);
  server.on("/api/preset/save", handleSavePreset);
  server.on("/api/preset/load", handleLoadPreset);
  server.onNotFound(handleNotFound);
  server.begin();
}

void handleRoot() {
  if (!server.hasArg("pass") || server.arg("pass") != webPassword) {
    server.send(401, "text/html", getLoginPage());
    return;
  }
  server.send(200, "text/html", getControlPage());
}

void handleStatus() {
  DynamicJsonDocument doc(256);
  doc["mode"] = currentMode;
  doc["brightness"] = brightness;
  doc["speed"] = speed;
  doc["color"] = selectedColor;
  doc["animating"] = isAnimating;
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleControl() {
  if (!server.hasArg("pass") || server.arg("pass") != webPassword) {
    server.send(401, "text/plain", "Unauthorized");
    return;
  }
  
  if (server.hasArg("mode")) {
    currentMode = server.arg("mode").toInt();
    currentMode = constrain(currentMode, 1, 210);
  }
  if (server.hasArg("brightness")) {
    brightness = server.arg("brightness").toInt();
    FastLED.setBrightness(brightness);
  }
  if (server.hasArg("speed")) {
    speed = server.arg("speed").toInt();
  }
  if (server.hasArg("color")) {
    selectedColor = server.arg("color").toInt() % PALETTE_SIZE;
  }
  
  server.send(200, "text/plain", "OK");
}

void handlePresets() {
  DynamicJsonDocument doc(2048);
  JsonArray presetArray = doc.createNestedArray("presets");
  
  for (int i = 0; i < 20; i++) {
    JsonObject preset = presetArray.createNestedObject();
    preset["index"] = i;
    preset["name"] = presets[i].name;
    preset["mode"] = presets[i].mode;
    preset["brightness"] = presets[i].brightness;
    preset["speed"] = presets[i].speed;
    preset["color"] = presets[i].color;
  }
  
  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

void handleSavePreset() {
  if (!server.hasArg("pass") || server.arg("pass") != webPassword) {
    server.send(401, "text/plain", "Unauthorized");
    return;
  }
  
  DynamicJsonDocument doc(256);
  deserializeJson(doc, server.arg("plain"));
  
  int idx = doc["index"];
  if (idx >= 0 && idx < 20) {
    presets[idx].mode = doc["mode"];
    presets[idx].brightness = doc["brightness"];
    presets[idx].speed = doc["speed"];
    presets[idx].color = doc["color"];
    strncpy(presets[idx].name, doc["name"], 31);
    savePresetsToFS();
    server.send(200, "text/plain", "Preset saved");
  } else {
    server.send(400, "text/plain", "Invalid index");
  }
}

void handleLoadPreset() {
  if (!server.hasArg("pass") || server.arg("pass") != webPassword) {
    server.send(401, "text/plain", "Unauthorized");
    return;
  }
  
  int idx = server.arg("index").toInt();
  if (idx >= 0 && idx < 20) {
    currentMode = presets[idx].mode;
    brightness = presets[idx].brightness;
    speed = presets[idx].speed;
    selectedColor = presets[idx].color;
    FastLED.setBrightness(brightness);
    server.send(200, "text/plain", "Preset loaded");
  } else {
    server.send(400, "text/plain", "Invalid index");
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}

void savePresetsToFS() {
  DynamicJsonDocument doc(2048);
  JsonArray presetArray = doc.createNestedArray("presets");
  
  for (int i = 0; i < 20; i++) {
    JsonObject preset = presetArray.createNestedObject();
    preset["mode"] = presets[i].mode;
    preset["brightness"] = presets[i].brightness;
    preset["speed"] = presets[i].speed;
    preset["color"] = presets[i].color;
    preset["name"] = presets[i].name;
  }
  
  File file = LittleFS.open("/presets.json", "w");
  if (file) {
    serializeJson(doc, file);
    file.close();
  }
}

void loadPresetsFromFS() {
  if (LittleFS.exists("/presets.json")) {
    File file = LittleFS.open("/presets.json", "r");
    if (file) {
      DynamicJsonDocument doc(2048);
      deserializeJson(doc, file);
      file.close();
      
      JsonArray presetArray = doc["presets"];
      for (int i = 0; i < 20 && i < presetArray.size(); i++) {
        presets[i].mode = presetArray[i]["mode"];
        presets[i].brightness = presetArray[i]["brightness"];
        presets[i].speed = presetArray[i]["speed"];
        presets[i].color = presetArray[i]["color"];
        strncpy(presets[i].name, presetArray[i]["name"], 31);
      }
      return;
    }
  }
  
  // Initialize default presets
  for (int i = 0; i < 20; i++) {
    presets[i].mode = 140 + (i % 70);
    presets[i].brightness = 255;
    presets[i].speed = 128 + (i * 5);
    presets[i].color = i % PALETTE_SIZE;
    sprintf(presets[i].name, "Preset %d", i + 1);
  }
  savePresetsToFS();
}

void clearAllLeds() {
  fill_solid(leds1, STRIP1_LEDS, CRGB::Black);
  fill_solid(leds2, STRIP2_LEDS, CRGB::Black);
  fill_solid(leds3, STRIP3_LEDS, CRGB::Black);
  fill_solid(leds4, STRIP4_LEDS, CRGB::Black);
}

void playAnimation(uint16_t mode) {
  static unsigned long animCycle = 0;
  animCycle++;
  
  // MODES 1-50: Solid Colors
  if (mode >= 1 && mode <= 50) {
    uint8_t colorIdx = (mode - 1) % PALETTE_SIZE;
    CRGB color = colorPalette[colorIdx];
    fill_solid(leds1, STRIP1_LEDS, color);
    fill_solid(leds2, STRIP2_LEDS, color);
    fill_solid(leds3, STRIP3_LEDS, color);
    fill_solid(leds4, STRIP4_LEDS, color);
  }
  // MODES 51-80: Breathing Effect
  else if (mode >= 51 && mode <= 80) {
    uint8_t colorIdx = (mode - 51) % PALETTE_SIZE;
    CRGB color = colorPalette[colorIdx];
    uint8_t breathe = 128 + 127 * sin8(animCycle * (256 / (257 - speed)));
    CRGB breatheColor = color;
    breatheColor.nscale8(breathe);
    fill_solid(leds1, STRIP1_LEDS, breatheColor);
    fill_solid(leds2, STRIP2_LEDS, breatheColor);
    fill_solid(leds3, STRIP3_LEDS, breatheColor);
    fill_solid(leds4, STRIP4_LEDS, breatheColor);
  }
  // MODES 81-110: Pulsing
  else if (mode >= 81 && mode <= 110) {
    uint8_t colorIdx = (mode - 81) % PALETTE_SIZE;
    CRGB color = colorPalette[colorIdx];
    uint16_t pulsePhase = animCycle % ((256 - speed) + 1);
    uint8_t pulseVal = (pulsePhase < (256 - speed) / 2) ? 255 : 0;
    CRGB pulseColor = color;
    pulseColor.nscale8(pulseVal);
    fill_solid(leds1, STRIP1_LEDS, pulseColor);
    fill_solid(leds2, STRIP2_LEDS, pulseColor);
    fill_solid(leds3, STRIP3_LEDS, pulseColor);
    fill_solid(leds4, STRIP4_LEDS, pulseColor);
  }
  // MODES 111-139: Alternating Strips
  else if (mode >= 111 && mode <= 139) {
    uint8_t colorIdx1 = (mode - 111) % PALETTE_SIZE;
    uint8_t colorIdx2 = (colorIdx1 + 1) % PALETTE_SIZE;
    CRGB color1 = colorPalette[colorIdx1];
    CRGB color2 = colorPalette[colorIdx2];
    
    uint16_t altPhase = animCycle % ((256 - speed) + 1);
    bool alternate = (altPhase < (256 - speed) / 2);
    
    if (alternate) {
      fill_solid(leds1, STRIP1_LEDS, color1);
      fill_solid(leds2, STRIP2_LEDS, color2);
      fill_solid(leds3, STRIP3_LEDS, color2);
      fill_solid(leds4, STRIP4_LEDS, color1);
    } else {
      fill_solid(leds1, STRIP1_LEDS, color2);
      fill_solid(leds2, STRIP2_LEDS, color1);
      fill_solid(leds3, STRIP3_LEDS, color1);
      fill_solid(leds4, STRIP4_LEDS, color2);
    }
  }
  // MODES 140-175: 2-Stripe Strobe Patterns
  else if (mode >= 140 && mode <= 175) {
    uint8_t patternIdx = (mode - 140) % 8;
    uint16_t strobePhase = animCycle % ((256 - speed) + 1);
    bool strobeState = (strobePhase < (256 - speed) / 2);
    
    CRGB color1, color2;
    selectStripeColors(patternIdx, color1, color2);
    
    if (strobeState) {
      fill_solid(leds1, STRIP1_LEDS, color1);
      fill_solid(leds2, STRIP2_LEDS, color2);
      fill_solid(leds3, STRIP3_LEDS, color2);
      fill_solid(leds4, STRIP4_LEDS, color1);
    } else {
      fill_solid(leds1, STRIP1_LEDS, color2);
      fill_solid(leds2, STRIP2_LEDS, color1);
      fill_solid(leds3, STRIP3_LEDS, color1);
      fill_solid(leds4, STRIP4_LEDS, color2);
    }
  }
  // MODES 176-208: 5-Color Multi-Stripe Patterns
  else if (mode >= 176 && mode <= 208) {
    uint8_t patternType = (mode - 176) % 5;
    const CRGB* pattern = selectFiveColorPattern(patternType);
    
    uint16_t strobePhase = animCycle % ((256 - speed) + 1);
    uint8_t strobeIdx = (strobePhase / ((256 - speed) / 5 + 1)) % 5;
    
    CRGB strobeColor = pattern[strobeIdx];
    fill_solid(leds1, STRIP1_LEDS, strobeColor);
    fill_solid(leds2, STRIP2_LEDS, strobeColor);
    fill_solid(leds3, STRIP3_LEDS, strobeColor);
    fill_solid(leds4, STRIP4_LEDS, strobeColor);
  }
  // MODE 209: Interactive Color-Change Mode
  else if (mode == 209) {
    CRGB color = colorPalette[selectedColor];
    uint8_t breathe = 128 + 127 * sin8(animCycle * 2);
    color.nscale8(breathe);
    fill_solid(leds1, STRIP1_LEDS, color);
    fill_solid(leds2, STRIP2_LEDS, color);
    fill_solid(leds3, STRIP3_LEDS, color);
    fill_solid(leds4, STRIP4_LEDS, color);
  }
  // MODE 210: Auto-Cycling 5-Stripe Rainbow-Free Pattern
  else if (mode == 210) {
    uint16_t cyclePhase = animCycle % ((256 - speed) + 1);
    uint8_t colorIdx = (cyclePhase / ((256 - speed) / 5 + 1)) % 5;
    const CRGB* pattern = strobePattern5Color1;
    CRGB color = pattern[colorIdx];
    
    fill_solid(leds1, STRIP1_LEDS, color);
    fill_solid(leds2, STRIP2_LEDS, color);
    fill_solid(leds3, STRIP3_LEDS, color);
    fill_solid(leds4, STRIP4_LEDS, color);
  }
  else {
    clearAllLeds();
  }
}

const CRGB* selectFiveColorPattern(uint8_t patternIdx) {
  switch (patternIdx) {
    case 0: return strobePattern5Color1;
    case 1: return strobePattern5Color2;
    case 2: return strobePattern5Color3;
    case 3: return strobePattern5Color4;
    case 4: return strobePattern5Color5;
    default: return strobePattern5Color1;
  }
}

void selectStripeColors(uint8_t patternIdx, CRGB& color1, CRGB& color2) {
  switch (patternIdx) {
    case 0:
      color1 = CRGB::Red;
      color2 = CRGB::Blue;
      break;
    case 1:
      color1 = CRGB::Green;
      color2 = CRGB::Magenta;
      break;
    case 2:
      color1 = CRGB::Cyan;
      color2 = CRGB::Purple;
      break;
    case 3:
      color1 = CRGB(255, 20, 147);
      color2 = CRGB(30, 144, 255);
      break;
    case 4:
      color1 = CRGB(0, 206, 209);
      color2 = CRGB(138, 43, 226);
      break;
    case 5:
      color1 = CRGB(220, 20, 60);
      color2 = CRGB(75, 0, 130);
      break;
    case 6:
      color1 = CRGB(199, 21, 133);
      color2 = CRGB::Green;
      break;
    case 7:
      color1 = CRGB::White;
      color2 = CRGB(255, 192, 203);
      break;
    default:
      color1 = CRGB::Red;
      color2 = CRGB::Blue;
  }
}

String getLoginPage() {
  return R"(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Striped RGB Strobe - Login</title>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
          font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
          background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
          min-height: 100vh;
          display: flex;
          align-items: center;
          justify-content: center;
        }
        .login-container {
          background: white;
          padding: 40px;
          border-radius: 10px;
          box-shadow: 0 10px 25px rgba(0, 0, 0, 0.2);
          width: 100%;
          max-width: 400px;
        }
        h1 {
          color: #333;
          margin-bottom: 10px;
          text-align: center;
          font-size: 28px;
        }
        .subtitle {
          text-align: center;
          color: #999;
          margin-bottom: 30px;
          font-size: 14px;
        }
        .form-group {
          margin-bottom: 20px;
        }
        label {
          display: block;
          margin-bottom: 8px;
          color: #333;
          font-weight: 600;
        }
        input[type="password"] {
          width: 100%;
          padding: 12px;
          border: 2px solid #ddd;
          border-radius: 5px;
          font-size: 16px;
          transition: border-color 0.3s;
        }
        input[type="password"]:focus {
          outline: none;
          border-color: #667eea;
        }
        button {
          width: 100%;
          padding: 12px;
          background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
          color: white;
          border: none;
          border-radius: 5px;
          font-size: 16px;
          font-weight: 600;
          cursor: pointer;
          transition: transform 0.2s;
        }
        button:hover {
          transform: translateY(-2px);
        }
        button:active {
          transform: translateY(0);
        }
      </style>
    </head>
    <body>
      <div class="login-container">
        <h1>✨ Striped RGB Strobe</h1>
        <p class="subtitle">Control Your LED Module</p>
        <form method="GET" action="/">
          <div class="form-group">
            <label for="pass">Password:</label>
            <input type="password" id="pass" name="pass" placeholder="Enter password" required autofocus>
          </div>
          <button type="submit">Login</button>
        </form>
      </div>
    </body>
    </html>
  )";
}

String getControlPage() {
  return R"(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Striped RGB Strobe - Control</title>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
          font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
          background: #0f0f0f;
          color: #fff;
          padding: 20px;
        }
        .container {
          max-width: 800px;
          margin: 0 auto;
        }
        h1 {
          text-align: center;
          margin-bottom: 30px;
          font-size: 32px;
          background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
          -webkit-background-clip: text;
          -webkit-text-fill-color: transparent;
          background-clip: text;
        }
        .section {
          background: #1a1a1a;
          border: 2px solid #333;
          border-radius: 10px;
          padding: 25px;
          margin-bottom: 20px;
        }
        .section-title {
          font-size: 18px;
          font-weight: 600;
          margin-bottom: 15px;
          color: #667eea;
        }
        .control-group {
          margin-bottom: 20px;
        }
        label {
          display: block;
          margin-bottom: 8px;
          font-weight: 600;
          color: #ccc;
        }
        input[type="range"],
        input[type="number"],
        select {
          width: 100%;
          padding: 10px;
          background: #2a2a2a;
          border: 1px solid #444;
          border-radius: 5px;
          color: #fff;
          font-size: 14px;
          transition: all 0.3s;
        }
        input[type="range"] {
          padding: 0;
          height: 6px;
        }
        input[type="range"]::-webkit-slider-thumb {
          width: 18px;
          height: 18px;
          border-radius: 50%;
          background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
          cursor: pointer;
          -webkit-appearance: none;
        }
        input[type="number"],
        select {
          padding: 10px;
        }
        input[type="number"]:focus,
        select:focus {
          outline: none;
          border-color: #667eea;
          background: #333;
        }
        .value-display {
          display: inline-block;
          background: #667eea;
          padding: 4px 8px;
          border-radius: 3px;
          margin-left: 10px;
          font-weight: 600;
        }
        .button-group {
          display: grid;
          grid-template-columns: 1fr 1fr;
          gap: 10px;
          margin-bottom: 15px;
        }
        button {
          padding: 12px;
          background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
          color: white;
          border: none;
          border-radius: 5px;
          font-weight: 600;
          cursor: pointer;
          transition: all 0.3s;
          font-size: 14px;
        }
        button:hover {
          transform: translateY(-2px);
          box-shadow: 0 5px 15px rgba(102, 126, 234, 0.4);
        }
        button:active {
          transform: translateY(0);
        }
        .preset-grid {
          display: grid;
          grid-template-columns: repeat(auto-fit, minmax(150px, 1fr));
          gap: 10px;
        }
        .preset-btn {
          padding: 10px;
          background: #2a2a2a;
          border: 2px solid #444;
          color: #fff;
          border-radius: 5px;
          cursor: pointer;
          transition: all 0.3s;
          font-size: 12px;
        }
        .preset-btn:hover {
          border-color: #667eea;
          background: #333;
        }
        .preset-btn.active {
          border-color: #667eea;
          background: #667eea;
        }
        .color-preview {
          display: inline-block;
          width: 30px;
          height: 30px;
          border-radius: 50%;
          vertical-align: middle;
          margin-left: 10px;
          border: 2px solid #667eea;
        }
        .status {
          text-align: center;
          padding: 10px;
          border-radius: 5px;
          margin-bottom: 15px;
          font-size: 14px;
        }
        .status.success {
          background: #2d5016;
          color: #90ee90;
        }
        .status.error {
          background: #5a1a1a;
          color: #ff6b6b;
        }
        @media (max-width: 600px) {
          .button-group {
            grid-template-columns: 1fr;
          }
          h1 {
            font-size: 24px;
          }
        }
      </style>
    </head>
    <body>
      <div class="container">
        <h1>✨ Striped RGB Strobe</h1>
        <div id="status"></div>
        
        <!-- Animation Control Section -->
        <div class="section">
          <div class="section-title">Animation Control</div>
          <div class="control-group">
            <label for="mode">Mode (1-210): <span class="value-display" id="modeValue">1</span></label>
            <input type="number" id="mode" name="mode" min="1" max="210" value="1">
          </div>
          <div class="control-group">
            <label for="brightness">Brightness: <span class="value-display" id="brightnessValue">255</span></label>
            <input type="range" id="brightness" name="brightness" min="0" max="255" value="255">
          </div>
          <div class="control-group">
            <label for="speed">Speed: <span class="value-display" id="speedValue">128</span></label>
            <input type="range" id="speed" name="speed" min="0" max="255" value="128">
          </div>
          <div class="control-group">
            <label for="color">Color Palette: <span class="value-display" id="colorValue">0</span><div class="color-preview" id="colorPreview"></div></label>
            <input type="number" id="color" name="color" min="0" max="19" value="0">
          </div>
          <div class="button-group">
            <button onclick="updateControl()">Apply</button>
            <button onclick="toggleAnimation()">Toggle Animation</button>
          </div>
        </div>
        
        <!-- Presets Section -->
        <div class="section">
          <div class="section-title">Quick Presets</div>
          <div class="preset-grid" id="presetGrid"></div>
        </div>
        
        <!-- Preset Manager -->
        <div class="section">
          <div class="section-title">Preset Manager</div>
          <div class="control-group">
            <label for="presetName">Preset Name:</label>
            <input type="text" id="presetName" placeholder="Enter preset name">
          </div>
          <div class="button-group">
            <button onclick="savePreset()">Save as Preset</button>
            <button onclick="loadPreset()">Load Preset</button>
          </div>
        </div>
      </div>
      
      <script>
        const password = new URLSearchParams(window.location.search).get('pass') || 'admin123';
        let animating = true;
        
        const colorPalette = [
          '#FF0000', '#0000FF', '#00FF00', '#00FFFF', '#FF00FF',
          '#800080', '#FF1493', '#FFFFFF', '#FF4500', '#8B008B',
          '#FFC0CB', '#1E90FF', '#00CED1', '#8A2BE2', '#DC143C',
          '#4B0082', '#C71585', '#32CD32', '#7FFFD4'
        ];
        
        function updateControl() {
          const mode = document.getElementById('mode').value;
          const brightness = document.getElementById('brightness').value;
          const speed = document.getElementById('speed').value;
          const color = document.getElementById('color').value;
          
          fetch(`/api/control?mode=${mode}&brightness=${brightness}&speed=${speed}&color=${color}&pass=${password}`)
            .then(r => r.text())
            .then(d => showStatus('Settings updated!', 'success'))
            .catch(e => showStatus('Error updating settings', 'error'));
        }
        
        function toggleAnimation() {
          animating = !animating;
          showStatus(`Animation ${animating ? 'ON' : 'OFF'}`, 'success');
        }
        
        function savePreset() {
          const name = document.getElementById('presetName').value || 'Unnamed';
          const mode = document.getElementById('mode').value;
          const brightness = document.getElementById('brightness').value;
          const speed = document.getElementById('speed').value;
          const color = document.getElementById('color').value;
          
          fetch('/api/preset/save', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
              index: 0,
              mode, brightness, speed, color, name,
              pass: password
            })
          })
          .then(r => r.text())
          .then(d => showStatus('Preset saved!', 'success'))
          .catch(e => showStatus('Error saving preset', 'error'));
        }
        
        function loadPreset() {
          fetch(`/api/preset/load?index=0&pass=${password}`)
            .then(r => r.text())
            .then(d => {
              showStatus('Preset loaded!', 'success');
              loadStatus();
            })
            .catch(e => showStatus('Error loading preset', 'error'));
        }
        
        function loadStatus() {
          fetch('/api/status')
            .then(r => r.json())
            .then(data => {
              document.getElementById('mode').value = data.mode;
              document.getElementById('brightness').value = data.brightness;
              document.getElementById('speed').value = data.speed;
              document.getElementById('color').value = data.color;
              
              document.getElementById('modeValue').textContent = data.mode;
              document.getElementById('brightnessValue').textContent = data.brightness;
              document.getElementById('speedValue').textContent = data.speed;
              document.getElementById('colorValue').textContent = data.color;
              
              const colorHex = colorPalette[data.color % colorPalette.length];
              document.getElementById('colorPreview').style.background = colorHex;
            });
        }
        
        function loadPresets() {
          fetch('/api/presets')
            .then(r => r.json())
            .then(data => {
              const grid = document.getElementById('presetGrid');
              grid.innerHTML = '';
              data.presets.forEach((preset, idx) => {
                const btn = document.createElement('button');
                btn.className = 'preset-btn';
                btn.textContent = preset.name || `Preset ${idx + 1}`;
                btn.onclick = () => {
                  document.getElementById('mode').value = preset.mode;
                  document.getElementById('brightness').value = preset.brightness;
                  document.getElementById('speed').value = preset.speed;
                  document.getElementById('color').value = preset.color;
                  updateControl();
                };
                grid.appendChild(btn);
              });
            });
        }
        
        function showStatus(msg, type) {
          const status = document.getElementById('status');
          status.textContent = msg;
          status.className = `status ${type}`;
          setTimeout(() => { status.textContent = ''; status.className = ''; }, 3000);
        }
        
        // Update value displays
        document.getElementById('mode').addEventListener('input', (e) => {
          document.getElementById('modeValue').textContent = e.target.value;
        });
        document.getElementById('brightness').addEventListener('input', (e) => {
          document.getElementById('brightnessValue').textContent = e.target.value;
        });
        document.getElementById('speed').addEventListener('input', (e) => {
          document.getElementById('speedValue').textContent = e.target.value;
        });
        document.getElementById('color').addEventListener('input', (e) => {
          document.getElementById('colorValue').textContent = e.target.value;
          const colorHex = colorPalette[e.target.value % colorPalette.length];
          document.getElementById('colorPreview').style.background = colorHex;
        });
        
        // Initialize
        loadStatus();
        loadPresets();
      </script>
    </body>
    </html>
  )";
}
