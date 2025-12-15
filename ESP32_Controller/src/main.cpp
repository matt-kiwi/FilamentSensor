#include <Arduino.h>
#include <WiFi.h>
#include "LittleFS.h"
#include "stdint.h"
#include <Adafruit_NeoPixel.h>
#include "RTTTLPlayer.h"
#include <RTTTLTunes.h>

// Custom includes
#include "config.h"
#include "globals.h"

// Forward declarations
void loop100ms();
void loop1second();
void loop1minute();
void loop15minute();
void loop60minute();
void updateSensors();

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
    player.begin();
    player.setDebug(true);
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
    

    // Initialize mode switch (ADC input)
    pinMode(MODE_SWITCH_PIN, INPUT);
    
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
    uint8_t s1 = digitalRead(SENSOR_1_PIN);
    Serial.printf(" %i ", s1);
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