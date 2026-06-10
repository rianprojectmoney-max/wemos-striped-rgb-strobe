#ifndef ANIMATIONS_ADVANCED_H
#define ANIMATIONS_ADVANCED_H

#include <FastLED.h>

// Advanced Animation Function Declarations
// These extend the basic 210 modes with sophisticated effects

// Wave/Pulse Animations
void waveAnimation(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                   uint8_t strips[4], CRGB color, uint8_t speed, unsigned long cycle);

void pulseWaveAnimation(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                        uint8_t strips[4], CRGB color1, CRGB color2, uint8_t speed, unsigned long cycle);

// Chase/Flowing Animations (5-color only)
void fiveColorChase(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                    uint8_t strips[4], const CRGB* pattern, uint8_t speed, unsigned long cycle);

void rainbowChase5Color(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                        uint8_t strips[4], uint8_t speed, unsigned long cycle);

// Strobe/Flash Animations
void strobeFlash(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                 uint8_t strips[4], CRGB color1, CRGB color2, uint8_t speed, unsigned long cycle);

void doubleStrobeFlash(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                       uint8_t strips[4], CRGB color1, CRGB color2, uint8_t speed, unsigned long cycle);

void tripleStrobeFlash(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                       uint8_t strips[4], CRGB color1, CRGB color2, uint8_t speed, unsigned long cycle);

// Sparkle/Glitter Animations
void sparkleAnimation(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                      uint8_t strips[4], CRGB baseColor, CRGB sparkleColor, uint8_t speed, unsigned long cycle);

void multisparkleAnimation(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                           uint8_t strips[4], const CRGB* pattern, uint8_t speed, unsigned long cycle);

// Gradient/Fade Animations
void smoothGradient(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                    uint8_t strips[4], CRGB color1, CRGB color2, uint8_t speed, unsigned long cycle);

void fiveColorGradient(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                       uint8_t strips[4], const CRGB* pattern, uint8_t speed, unsigned long cycle);

// Collision/Bouncing Animations
void bouncingAnimation(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                       uint8_t strips[4], CRGB color1, CRGB color2, uint8_t speed, unsigned long cycle);

void collisionAnimation(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                        uint8_t strips[4], const CRGB* pattern, uint8_t speed, unsigned long cycle);

// Spinning/Rotating Animations
void spinningRing(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                  uint8_t strips[4], CRGB color, uint8_t speed, unsigned long cycle);

void fiveColorSpinner(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                      uint8_t strips[4], const CRGB* pattern, uint8_t speed, unsigned long cycle);

// Synchronized Strobe Animations
void syncStrobeAll(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                   uint8_t strips[4], CRGB color, uint8_t speed, unsigned long cycle);

void fiveColorSyncStrobe(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                         uint8_t strips[4], const CRGB* pattern, uint8_t speed, unsigned long cycle);

// Helper function to fill strips
void fillAllStrips(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                   uint8_t strips[4], CRGB color);

// Helper function to clear all strips
void clearAllStrips(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4);

// Implementation

void fillAllStrips(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                   uint8_t strips[4], CRGB color) {
  fill_solid(leds1, strips[0], color);
  fill_solid(leds2, strips[1], color);
  fill_solid(leds3, strips[2], color);
  fill_solid(leds4, strips[3], color);
}

void clearAllStrips(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4) {
  fill_solid(leds1, 9, CRGB::Black);
  fill_solid(leds2, 4, CRGB::Black);
  fill_solid(leds3, 4, CRGB::Black);
  fill_solid(leds4, 9, CRGB::Black);
}

// Wave Animation - Smooth undulating effect
void waveAnimation(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                   uint8_t strips[4], CRGB color, uint8_t speed, unsigned long cycle) {
  clearAllStrips(leds1, leds2, leds3, leds4);
  
  uint8_t wave = sin8(cycle * (256 / (257 - speed)));
  uint8_t brightness = 100 + (155 * wave / 255);
  
  CRGB waveColor = color;
  waveColor.nscale8(brightness);
  
  // Strip 1 (top)
  for (int i = 0; i < strips[0]; i++) {
    uint8_t led_brightness = 128 + 127 * sin8(i * 25 + cycle * (256 / (257 - speed)));
    CRGB led_color = color;
    led_color.nscale8(led_brightness);
    leds1[i] = led_color;
  }
  
  // Strip 4 (bottom)
  for (int i = 0; i < strips[3]; i++) {
    uint8_t led_brightness = 128 + 127 * sin8(i * 25 + cycle * (256 / (257 - speed)) + 128);
    CRGB led_color = color;
    led_color.nscale8(led_brightness);
    leds4[i] = led_color;
  }
  
  // Middle strips alternate
  if ((cycle / ((256 - speed) / 2)) % 2) {
    fill_solid(leds2, strips[1], color);
    fill_solid(leds3, strips[2], CRGB::Black);
  } else {
    fill_solid(leds2, strips[1], CRGB::Black);
    fill_solid(leds3, strips[2], color);
  }
}

// Pulse Wave - Two colors oscillating
void pulseWaveAnimation(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                        uint8_t strips[4], CRGB color1, CRGB color2, uint8_t speed, unsigned long cycle) {
  uint8_t pulse = sin8(cycle * (256 / (257 - speed)));
  bool useColor1 = pulse > 127;
  CRGB activeColor = useColor1 ? color1 : color2;
  
  fillAllStrips(leds1, leds2, leds3, leds4, strips, activeColor);
}

// Five Color Chase - Flowing 5-color pattern (no 2-color flows)
void fiveColorChase(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                    uint8_t strips[4], const CRGB* pattern, uint8_t speed, unsigned long cycle) {
  clearAllStrips(leds1, leds2, leds3, leds4);
  
  uint8_t offset = (cycle * (256 - speed) / 256) % 5;
  uint8_t colorIdx = offset % 5;
  
  CRGB chaseColor = pattern[colorIdx];
  
  // Create flowing effect by alternating strips
  uint8_t phase = (cycle / ((256 - speed) / 5 + 1)) % 5;
  
  if (phase == 0) {
    fill_solid(leds1, strips[0], pattern[0]);
    fill_solid(leds2, strips[1], CRGB::Black);
    fill_solid(leds3, strips[2], CRGB::Black);
    fill_solid(leds4, strips[3], CRGB::Black);
  } else if (phase == 1) {
    fill_solid(leds1, strips[0], CRGB::Black);
    fill_solid(leds2, strips[1], pattern[1]);
    fill_solid(leds3, strips[2], CRGB::Black);
    fill_solid(leds4, strips[3], CRGB::Black);
  } else if (phase == 2) {
    fill_solid(leds1, strips[0], CRGB::Black);
    fill_solid(leds2, strips[1], CRGB::Black);
    fill_solid(leds3, strips[2], pattern[2]);
    fill_solid(leds4, strips[3], CRGB::Black);
  } else if (phase == 3) {
    fill_solid(leds1, strips[0], CRGB::Black);
    fill_solid(leds2, strips[1], CRGB::Black);
    fill_solid(leds3, strips[2], CRGB::Black);
    fill_solid(leds4, strips[3], pattern[3]);
  } else {
    fill_solid(leds1, strips[0], CRGB::Black);
    fill_solid(leds2, strips[1], CRGB::Black);
    fill_solid(leds3, strips[2], CRGB::Black);
    fill_solid(leds4, strips[3], pattern[4]);
  }
}

// Rainbow Chase 5-Color
void rainbowChase5Color(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                        uint8_t strips[4], uint8_t speed, unsigned long cycle) {
  const CRGB pattern[] = {CRGB::Red, CRGB::Blue, CRGB::Green, CRGB::Magenta, CRGB::Cyan};
  fiveColorChase(leds1, leds2, leds3, leds4, strips, pattern, speed, cycle);
}

// Strobe Flash - Single flash
void strobeFlash(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                 uint8_t strips[4], CRGB color1, CRGB color2, uint8_t speed, unsigned long cycle) {
  uint16_t period = 256 - speed + 1;
  uint16_t phase = cycle % period;
  uint16_t flashDuration = period / 4;
  
  if (phase < flashDuration) {
    fillAllStrips(leds1, leds2, leds3, leds4, strips, color1);
  } else if (phase < flashDuration * 2) {
    fillAllStrips(leds1, leds2, leds3, leds4, strips, color2);
  } else {
    clearAllStrips(leds1, leds2, leds3, leds4);
  }
}

// Double Strobe - Two quick flashes
void doubleStrobeFlash(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                       uint8_t strips[4], CRGB color1, CRGB color2, uint8_t speed, unsigned long cycle) {
  uint16_t period = 256 - speed + 1;
  uint16_t phase = cycle % period;
  uint16_t flashDuration = period / 8;
  
  if (phase < flashDuration || (phase > flashDuration * 2 && phase < flashDuration * 3)) {
    fillAllStrips(leds1, leds2, leds3, leds4, strips, color1);
  } else if (phase < flashDuration * 2 || (phase > flashDuration * 3 && phase < flashDuration * 4)) {
    fillAllStrips(leds1, leds2, leds3, leds4, strips, color2);
  } else {
    clearAllStrips(leds1, leds2, leds3, leds4);
  }
}

// Triple Strobe - Three quick flashes
void tripleStrobeFlash(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                       uint8_t strips[4], CRGB color1, CRGB color2, uint8_t speed, unsigned long cycle) {
  uint16_t period = 256 - speed + 1;
  uint16_t phase = cycle % period;
  uint16_t flashDuration = period / 12;
  
  bool flash = (phase < flashDuration) || 
               (phase > flashDuration * 2 && phase < flashDuration * 3) ||
               (phase > flashDuration * 4 && phase < flashDuration * 5);
  
  if (flash) {
    fillAllStrips(leds1, leds2, leds3, leds4, strips, color1);
  } else if (phase > flashDuration && phase < flashDuration * 2) {
    fillAllStrips(leds1, leds2, leds3, leds4, strips, color2);
  } else {
    clearAllStrips(leds1, leds2, leds3, leds4);
  }
}

// Sparkle Animation - Random sparkles on base color
void sparkleAnimation(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                      uint8_t strips[4], CRGB baseColor, CRGB sparkleColor, uint8_t speed, unsigned long cycle) {
  fillAllStrips(leds1, leds2, leds3, leds4, strips, baseColor);
  
  // Sparkle effect - random LEDs flash
  uint16_t sparkleRate = map(speed, 0, 255, 50, 5);
  
  // Strip 1
  if ((cycle + 13) % sparkleRate < 2) {
    uint8_t idx = (cycle * 7) % strips[0];
    leds1[idx] = sparkleColor;
  }
  
  // Strip 2
  if ((cycle + 17) % sparkleRate < 2) {
    uint8_t idx = (cycle * 11) % strips[1];
    leds2[idx] = sparkleColor;
  }
  
  // Strip 3
  if ((cycle + 19) % sparkleRate < 2) {
    uint8_t idx = (cycle * 13) % strips[2];
    leds3[idx] = sparkleColor;
  }
  
  // Strip 4
  if ((cycle + 23) % sparkleRate < 2) {
    uint8_t idx = (cycle * 17) % strips[3];
    leds4[idx] = sparkleColor;
  }
}

// Multi-Sparkle - 5-color sparkles
void multisparkleAnimation(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                           uint8_t strips[4], const CRGB* pattern, uint8_t speed, unsigned long cycle) {
  clearAllStrips(leds1, leds2, leds3, leds4);
  
  uint16_t sparkleRate = map(speed, 0, 255, 50, 5);
  
  // Cycle through all strips with 5 colors
  if ((cycle + 13) % sparkleRate < 2) {
    uint8_t idx = (cycle * 7) % 9;
    uint8_t colorIdx = (cycle / 10) % 5;
    leds1[idx] = pattern[colorIdx];
  }
  
  if ((cycle + 17) % sparkleRate < 2) {
    uint8_t idx = (cycle * 11) % 4;
    uint8_t colorIdx = ((cycle + 5) / 10) % 5;
    leds2[idx] = pattern[colorIdx];
  }
  
  if ((cycle + 19) % sparkleRate < 2) {
    uint8_t idx = (cycle * 13) % 4;
    uint8_t colorIdx = ((cycle + 10) / 10) % 5;
    leds3[idx] = pattern[colorIdx];
  }
  
  if ((cycle + 23) % sparkleRate < 2) {
    uint8_t idx = (cycle * 17) % 9;
    uint8_t colorIdx = ((cycle + 15) / 10) % 5;
    leds4[idx] = pattern[colorIdx];
  }
}

// Smooth Gradient - Fade between two colors
void smoothGradient(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                    uint8_t strips[4], CRGB color1, CRGB color2, uint8_t speed, unsigned long cycle) {
  uint8_t gradient = sin8(cycle * (256 / (257 - speed)));
  
  CRGB gradientColor = blend(color1, color2, gradient);
  fillAllStrips(leds1, leds2, leds3, leds4, strips, gradientColor);
}

// Five Color Gradient - Fade through 5 colors
void fiveColorGradient(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                       uint8_t strips[4], const CRGB* pattern, uint8_t speed, unsigned long cycle) {
  uint8_t phase = (cycle / ((256 - speed) / 5 + 1)) % 5;
  uint8_t nextPhase = (phase + 1) % 5;
  uint8_t blend_amount = (cycle / ((256 - speed) / 25 + 1)) % 10;
  blend_amount = blend_amount * 25;
  
  CRGB blendColor = blend(pattern[phase], pattern[nextPhase], blend_amount);
  fillAllStrips(leds1, leds2, leds3, leds4, strips, blendColor);
}

// Bouncing Animation - "ball" bounces between strips
void bouncingAnimation(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                       uint8_t strips[4], CRGB color1, CRGB color2, uint8_t speed, unsigned long cycle) {
  clearAllStrips(leds1, leds2, leds3, leds4);
  
  uint16_t period = 256 - speed + 1;
  uint16_t phase = cycle % (period * 4);
  
  if (phase < period) {
    fill_solid(leds1, strips[0], color1);
  } else if (phase < period * 2) {
    fill_solid(leds4, strips[3], color1);
  } else if (phase < period * 3) {
    fill_solid(leds1, strips[0], color2);
  } else {
    fill_solid(leds4, strips[3], color2);
  }
  
  fill_solid(leds2, strips[1], color2);
  fill_solid(leds3, strips[2], color2);
}

// Collision Animation - Two 5-color patterns colliding
void collisionAnimation(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                        uint8_t strips[4], const CRGB* pattern, uint8_t speed, unsigned long cycle) {
  clearAllStrips(leds1, leds2, leds3, leds4);
  
  uint8_t colorIdx = (cycle / ((256 - speed) / 5 + 1)) % 5;
  
  // Strips move toward each other
  fill_solid(leds1, strips[0], pattern[colorIdx]);
  fill_solid(leds4, strips[3], pattern[(colorIdx + 2) % 5]);
  fill_solid(leds2, strips[1], pattern[(colorIdx + 1) % 5]);
  fill_solid(leds3, strips[2], pattern[(colorIdx + 3) % 5]);
}

// Spinning Ring - Rotating animation
void spinningRing(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                  uint8_t strips[4], CRGB color, uint8_t speed, unsigned long cycle) {
  clearAllStrips(leds1, leds2, leds3, leds4);
  
  uint8_t position = (cycle * (256 - speed) / 256) % 4;
  
  if (position == 0) {
    fill_solid(leds1, strips[0], color);
  } else if (position == 1) {
    fill_solid(leds4, strips[3], color);
  } else if (position == 2) {
    fill_solid(leds2, strips[1], color);
  } else {
    fill_solid(leds3, strips[2], color);
  }
}

// Five Color Spinner
void fiveColorSpinner(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                      uint8_t strips[4], const CRGB* pattern, uint8_t speed, unsigned long cycle) {
  clearAllStrips(leds1, leds2, leds3, leds4);
  
  uint8_t position = (cycle / ((256 - speed) / 4 + 1)) % 4;
  uint8_t colorIdx = (cycle / ((256 - speed) / 5 + 1)) % 5;
  
  if (position == 0) {
    fill_solid(leds1, strips[0], pattern[colorIdx]);
  } else if (position == 1) {
    fill_solid(leds4, strips[3], pattern[(colorIdx + 1) % 5]);
  } else if (position == 2) {
    fill_solid(leds2, strips[1], pattern[(colorIdx + 2) % 5]);
  } else {
    fill_solid(leds3, strips[2], pattern[(colorIdx + 3) % 5]);
  }
}

// Synchronized Strobe - All LEDs flash together
void syncStrobeAll(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                   uint8_t strips[4], CRGB color, uint8_t speed, unsigned long cycle) {
  uint16_t period = 256 - speed + 1;
  uint16_t phase = cycle % period;
  
  if (phase < period / 2) {
    fillAllStrips(leds1, leds2, leds3, leds4, strips, color);
  } else {
    clearAllStrips(leds1, leds2, leds3, leds4);
  }
}

// Five Color Synchronized Strobe
void fiveColorSyncStrobe(CRGB* leds1, CRGB* leds2, CRGB* leds3, CRGB* leds4,
                         uint8_t strips[4], const CRGB* pattern, uint8_t speed, unsigned long cycle) {
  uint16_t period = 256 - speed + 1;
  uint16_t phase = cycle % period;
  uint8_t colorIdx = (cycle / ((256 - speed) / 5 + 1)) % 5;
  
  if (phase < period / 2) {
    fillAllStrips(leds1, leds2, leds3, leds4, strips, pattern[colorIdx]);
  } else {
    clearAllStrips(leds1, leds2, leds3, leds4);
  }
}

#endif // ANIMATIONS_ADVANCED_H
