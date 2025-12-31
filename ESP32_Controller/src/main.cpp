#include <Arduino.h>
#include <WiFi.h>
#include "LittleFS.h"
#include "stdint.h"
#define ECONODE_FILE_LOG_LEVEL 4
#include <Adafruit_NeoPixel.h>
#include "econode_logging.h"
// Set per-file log level to DEBUG
#define ECONODE_FILE_LOG_LEVEL 4
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
    delay(1000); // Wait for serial to be ready
    Serial.println("LOG TEST: before LOG_I");
    LOG_I("MAIN", "LOG TEST: inside LOG_I");
    Serial.println("LOG TEST: after LOG_I");
    pinMode(PIN_MODE_SWITCH, INPUT_PULLUP);
    readModeSwitch();
    // Initialize NeoPixel strip
    strip.begin();
    strip.setBrightness(25);
    setPixelColor(NEOPIXEL_ALARM, COLOR_PRE_BOOT);
    strip.show();
    player.begin();
    player.setDebug(false);
    player.play(RTTTLTunes::marioPowerUp);
    while( player.isPlaying() ){
        player.loop();
        delay(10);
    }
    delay(2000); // Wait for serial
    LOG_I("MAIN", "Initializing system...");
    
    // Initialize sensor pins
    // Seesaw sensors (1 & 2) require pull-up
    pinMode(PIN_SENSOR_1, INPUT_PULLUP); // Seesaw
    pinMode(PIN_SENSOR_2, INPUT_PULLUP); // Seesaw
    // Optical sensors (3 & 4) may also use pull-up if open-collector, else adjust as needed
    pinMode(PIN_SENSOR_3, INPUT_PULLUP); // Optical
    pinMode(PIN_SENSOR_4, INPUT_PULLUP); // Optical
    
    // Print welcome message
    LOG_I("MAIN", "=================================");
    LOG_I("MAIN", "Filament Sensor by Econode");
    LOG_I("MAIN", "=================================");
    
    // Print system info
    LOG_I("MAIN", "Chip Model: %s", ESP.getChipModel());
    LOG_I("MAIN", "Chip Revision: %d", ESP.getChipRevision());
    LOG_I("MAIN", "CPU Frequency: %d MHz", ESP.getCpuFreqMHz());
    LOG_I("MAIN", "Flash Size: %d MB", ESP.getFlashChipSize() / (1024 * 1024));
    LOG_I("MAIN", "Free Heap: %d bytes", ESP.getFreeHeap());
    LOG_I("MAIN", "NeoPixel Pin: GPIO%d", NEOPIXEL_PIN);
    LOG_I("MAIN", "System initialized successfully!");
    LOG_I("MAIN", "=================================");
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
    if( global.modeSwitch.alarm == 0 && player.isPlaying() ) player.stop();
    if( global.modeSwitch.hasChanged ) global.modeSwitch.hasChanged = 0;
    updateSensors();

    // Update sensor LEDs
    setPixelColor(NEOPIXEL_ALARM, global.sensors.alarm?COLOR_SENSOR_FAIL:COLOR_SENSOR_OK);
    setPixelColor(NEOPIXEL_SENSOR1, global.sensors.s1_state?COLOR_SENSOR_FAIL:COLOR_SENSOR_OK);
    setPixelColor(NEOPIXEL_SENSOR2, global.sensors.s2_state?COLOR_SENSOR_FAIL:COLOR_SENSOR_OK);
    setPixelColor(NEOPIXEL_SENSOR3, global.sensors.s3_state?COLOR_SENSOR_FAIL:COLOR_SENSOR_OK);
    setPixelColor(NEOPIXEL_SENSOR4, global.sensors.s4_state?COLOR_SENSOR_FAIL:COLOR_SENSOR_OK);
    
    if( global.sensors.hasChanged ){
        if( global.sensors.alarm ){
            if( global.modeSwitch.alarm ){
                player.stop();
                player.play( RTTTLTunes::error, 255);
            }
            global.sensors.hasChanged = false;
        } else {
            player.stop();
        }
    }
    // Update NeoPixels
    strip.show();
    yield();
}

void loop1minute() {
    LOG_I("MAIN", "[Minute] System uptime: %lu minutes", millis()/60000);
    Serial.println("1 min....");
}

void loop15minute() {
    LOG_I("MAIN", "[15 Min] Periodic system check");
}

void loop60minute() {
    LOG_I("MAIN", "[Hour] Hourly maintenance");
}

void updateSensors(){
    // Seesaw sensors (1 & 2) are logic reversed
    int raw1 = digitalRead(PIN_SENSOR_1);
    int raw2 = digitalRead(PIN_SENSOR_2);
    int raw3 = digitalRead(PIN_SENSOR_3);
    int raw4 = digitalRead(PIN_SENSOR_4);
    uint8_t s1_state = !raw1; // Seesaw: invert logic
    uint8_t s2_state = !raw2; // Seesaw: invert logic
    uint8_t s3_state = raw3;  // Optical: normal logic
    uint8_t s4_state = raw4;  // Optical: normal logic
    LOG_D("SENSOR", "S1:%d (raw:%d) S2:%d (raw:%d) S3:%d (raw:%d) S4:%d (raw:%d)", s1_state, raw1, s2_state, raw2, s3_state, raw3, s4_state, raw4);
    uint8_t hasChanged = 0;
    uint8_t alarm,all_off;
    
    if( global.sensors.s1_state != s1_state ||  global.sensors.s2_state != s2_state || 
        global.sensors.s3_state != s3_state ||  global.sensors.s4_state != s4_state) hasChanged = 1;
    global.sensors.hasChanged = hasChanged;
    if( hasChanged ){
        global.sensors.s1_state = s1_state;
        global.sensors.s2_state = s2_state;
        global.sensors.s3_state = s3_state;
        global.sensors.s4_state = s4_state;
    }
    all_off = s1_state == 0 &&  s2_state == 0 && s3_state == 0 && s4_state == 0;
    global.sensors.all_off = all_off;
    alarm = !all_off;
    global.sensors.alarm = alarm;
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