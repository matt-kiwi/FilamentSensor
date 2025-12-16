#include <Arduino.h>
#include <WiFi.h>
#include "LittleFS.h"
#include "stdint.h"
#include <Adafruit_NeoPixel.h>
#include "RTTTLPlayer.h"
#include <RTTTLTunes.h>

// Custom includes
#include "config.h"
#include "utils.h"
#include "globals.h"

// Forward declarations
void loop100ms();
void loop1second();
void loop1minute();
void loop15minute();
void loop60minute();
void updateSensors();
void readModeSwitch();

// Initialize the Neopixel strip with Adafruit NeoPixel
Adafruit_NeoPixel strip(NUM_NEOPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
global_t global;
RTTTLPlayer player(PIN_AUDIO_OUT, 100);

// Helper functions for NeoPixels
void setPixelColor(uint8_t pixel, uint8_t r, uint8_t g, uint8_t b) {
    strip.setPixelColor(pixel, strip.Color(r, g, b));
}

void setPixelColor(uint8_t pixel, uint32_t color) {
    strip.setPixelColor(pixel, color);
}

void setAllPixels(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t color = strip.Color(r, g, b);
    for(uint8_t i = 0; i < NUM_NEOPIXELS; i++) {
        strip.setPixelColor(i, color);
    }
}

void clearPixels() {
    strip.clear();
}

void setup() {
    global.epoch.lastSecondEpoch = millis();
    Serial.begin(115200);
    pinMode(PIN_MODE_SWITCH, INPUT_PULLUP);
    readModeSwitch();
    player.begin();
    player.setDebug(false);
    player.play(RTTTLTunes::marioPowerUp);
    while( player.isPlaying() ){
        player.loop();
        delay(10);
    }
    delay(2000); // Wait for serial
    
    // Initialize NeoPixel strip
    strip.begin();
    strip.setBrightness(25);
    strip.show();
    
    // Initial test pattern
    Serial.println("Initializing system...");
    setPixelColor(NEOPIXEL_NETWORK, 255, 0, 0); // Red for boot
    strip.show();
    delay(1000);
    
    // Initialize sensor pins
    pinMode(SENSOR_1_PIN, INPUT_PULLUP);
    pinMode(SENSOR_2_PIN, INPUT_PULLUP);
    pinMode(SENSOR_3_PIN, INPUT_PULLUP);
    pinMode(SENSOR_4_PIN, INPUT_PULLUP);
    

    // Clear NeoPixels and set initial status
    clearPixels();
    // Network LED: Blue pulsing (searching for network)
    setPixelColor(NEOPIXEL_NETWORK, 0, 0, 100);
    strip.show();
    
    // Print welcome message
    Serial.println("\n\n=================================");
    Serial.println("ESP32-C3 Super Mini Controller");
    Serial.println("Using Adafruit NeoPixel Library");
    Serial.println("=================================");
    
    // Print system info
    Serial.printf("Chip Model: %s\n", ESP.getChipModel());
    Serial.printf("Chip Revision: %d\n", ESP.getChipRevision());
    Serial.printf("CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("Flash Size: %d MB\n", ESP.getFlashChipSize() / (1024 * 1024));
    Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("Built-in LED Pin: GPIO%d (using PWM)\n", STATUS_LED_PIN);
    Serial.printf("NeoPixel Pin: GPIO%d\n", NEOPIXEL_PIN);
    Serial.println("System initialized successfully!");
    Serial.println("Press Ctrl+C to stop monitor");
    Serial.println("=================================\n");
}

void loop() {
    unsigned long currentMillis = millis();
    // 100ms tasks
    static unsigned long last100ms = 0;
    if (currentMillis - last100ms >= 100) {
        last100ms = currentMillis;
        loop100ms();
    }
    
    // 1 second tasks
    static unsigned long last1second = 0;
    if (currentMillis - last1second >= 1000) {
        last1second = currentMillis;
        loop1second();
        global.epoch.lastSecondEpoch++;
        global.epoch.uptimeSeconds = global.epoch.lastSecondEpoch;
    }
    
    // 1 minute tasks
    static unsigned long last1minute = 0;
    if (currentMillis - last1minute >= 60000) {
        last1minute = currentMillis;
        loop1minute();
    }
    
    // 15 minute tasks
    static unsigned long last15minute = 0;
    if (currentMillis - last15minute >= 900000) {
        last15minute = currentMillis;
        loop15minute();
    }
    
    // 60 minute tasks
    static unsigned long last60minute = 0;
    if (currentMillis - last60minute >= 3600000) {
        last60minute = currentMillis;
        loop60minute();
    }
}

void loop100ms() {
    player.loop();

}

void loop1second() {
    readModeSwitch();
    Serial.printf("modeSwitchRaw: %i  alarm:%d setup:%d hasChanged:%d\n", global.modeSwitch.raw, global.modeSwitch.alarm, global.modeSwitch.setup, global.modeSwitch.hasChanged);
    if( global.modeSwitch.hasChanged ) global.modeSwitch.hasChanged = 0;
    updateSensors();

    // Update sensor LEDs
    setPixelColor(NEOPIXEL_SENSOR1, global.sensors.s1_state?COLOR_SENSOR_FAIL:COLOR_SENSOR_OK);
    setPixelColor(NEOPIXEL_SENSOR2, global.sensors.s2_state?COLOR_SENSOR_FAIL:COLOR_SENSOR_OK);
    setPixelColor(NEOPIXEL_SENSOR3, global.sensors.s3_state?COLOR_SENSOR_FAIL:COLOR_SENSOR_OK);
    setPixelColor(NEOPIXEL_SENSOR4, global.sensors.s4_state?COLOR_SENSOR_FAIL:COLOR_SENSOR_OK);

    // Update NeoPixels
    strip.show();
}

void loop1minute() {
    Serial.printf("[Minute] System uptime: %lu minutes\n", millis()/60000);
}

void loop15minute() {
    Serial.println("[15 Min] Periodic system check");
}

void loop60minute() {
    Serial.println("[Hour] Hourly maintenance");
}

void updateSensors(){
    global.sensors.s1_state = digitalRead(SENSOR_1_PIN);
    global.sensors.s2_state = digitalRead(SENSOR_2_PIN);
    global.sensors.s3_state = digitalRead(SENSOR_3_PIN);
    global.sensors.s4_state = digitalRead(SENSOR_4_PIN);
}

void readModeSwitch(){
/*

 Two switches on a resistor ladder as follows

3.3V ─── 47kΩ ─── GPIO4 ──┬── 22kΩ ── SWITCH A ── GND
                           │
                          10kΩ
                           │
                           + ── SWITCH B ── GND
None: 4095
Switch A (22kΩ): 1434
Switch B (10kΩ): 780
Both: 570
*/
    uint16_t modeSwitchRaw = analogRead(PIN_MODE_SWITCH);
    bool modeSwitchSetup = false, modeSwitchAlarm = false;
    // Switch 1 logic - Alarm switch
    if( (modeSwitchRaw>1200 && modeSwitchRaw<1600) || (modeSwitchRaw>500 && modeSwitchRaw<600) ) modeSwitchAlarm = true;
    // Switch 2 logic - Setup switch
    if( (modeSwitchRaw>720 && modeSwitchRaw<820) || (modeSwitchRaw>500 && modeSwitchRaw<600) ) modeSwitchSetup = true;
    global.modeSwitch.hasChanged = global.modeSwitch.alarm != modeSwitchAlarm || global.modeSwitch.setup != modeSwitchSetup;
    global.modeSwitch.raw = modeSwitchRaw;
    global.modeSwitch.alarm = modeSwitchAlarm;
    global.modeSwitch.setup = modeSwitchSetup;
}