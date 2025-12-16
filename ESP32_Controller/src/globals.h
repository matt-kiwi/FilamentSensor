#ifndef GLOBALS_h
#define GLOBALS_h
#include "stdint.h"

struct global_epoch_t {
  unsigned long lastSecondEpoch = 0;
  unsigned long uptimeSeconds = 0;
};

struct global_mode_switch_t {
  uint16_t raw = 0;
  uint8_t setup = 0;
  uint8_t alarm = 0;
  uint8_t hasChanged = 0;
};

struct global_sensors_t {
    uint8_t s1_state;
    uint8_t s2_state;
    uint8_t s3_state;
    uint8_t s4_state;
    uint8_t all_off;
    uint8_t alarm;
    uint8_t hasChanged;
    uint32_t lastChangeEpoch;
};

// Global variable storage
struct global_t {
    // global_nvs_t nvs;
    global_epoch_t epoch;
    global_sensors_t sensors;
    global_mode_switch_t modeSwitch;
    char idfVersion[32] = {0};
    char elfFileSha256[18] = {0};
};
#endif