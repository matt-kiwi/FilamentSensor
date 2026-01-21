#pragma once

#define PIN_SENSOR_1     20
#define PIN_SENSOR_2     21
#define PIN_SENSOR_3     10
#define PIN_SENSOR_4     5

#define PIN_ALARM_RELAY   7

// Alarm threshold in milliseonds, IE delay to start alarm
#define ALARM_THRESHOLD  3000

// NeoPixel LED Strip (6 LEDs for status indication)
#define NEOPIXEL_PIN      2
#define NUM_NEOPIXELS     6

// Colors
#define COLOR_SENSOR_OK   strip.Color(0, 255, 0)   // Green
#define COLOR_SENSOR_FAIL strip.Color(255, 0, 0)   // Red
#define COLOR_PRE_BOOT    strip.Color(0, 0, 255)   // Blue
#define COLOR_BOOT_OK     strip.Color(0, 255, 0)   // Green

#define NEOPIXEL_ALARM    0
#define NEOPIXEL_NETWORK  1
#define NEOPIXEL_SENSOR1  2
#define NEOPIXEL_SENSOR2  3
#define NEOPIXEL_SENSOR3  4
#define NEOPIXEL_SENSOR4  5

// Audio out / RTTTL pin
#define PIN_AUDIO_OUT        3

/*
Setup and Alarm switches (Resistor Ladder to Single ADC)

3.3V ─── 47kΩ ─── GPIO4 ──┬── 22kΩ ── SWITCH A ── GND
                          │
                         10kΩ
                          │
                          + ── SWITCH B ── GND
*/
#define PIN_MODE_SWITCH  4  // GPIO4 - ADC1_CH3 (0-3.3V analog input)
