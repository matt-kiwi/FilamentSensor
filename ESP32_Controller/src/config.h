#pragma once

#define SENSOR_1_PIN     20
#define SENSOR_2_PIN     21
#define SENSOR_3_PIN     10
#define SENSOR_4_PIN     5

// NeoPixel LED Strip (6 LEDs for status indication)
#define NEOPIXEL_PIN      0
#define NUM_NEOPIXELS     6

// Piezo Buzzer
#define BUZZER_PIN        1

// 3-Position Mode Switch (Resistor Ladder to Single ADC)
// Positions: NORMAL, ALARM_OFF, SETUP
//  ** 3-Position Mode Switch: **
// 3.3V ─── 10kΩ ─── GPIO4 (ADC Input)
//             │
//   NORMAL  ─── 10kΩ ───┐
//   ALARM OFF ── 20kΩ ──┤
//   SETUP ────── 5kΩ ───┘
//               │
//              GND
#define MODE_SWITCH_PIN   4  // GPIO4 - ADC1_CH3 (0-3.3V analog input)

// Status LED
#define STATUS_LED_PIN    8  // GPIO8 - Built-in blue LED


