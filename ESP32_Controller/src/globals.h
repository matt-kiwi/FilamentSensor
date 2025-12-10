#ifndef GLOBALS_h
#define GLOBALS_h
#include "stdint.h"

struct global_epoch_t {
  unsigned long lastSecondEpoch = 0;
  unsigned long uptimeSeconds = 0;
};

struct global_sensors_t {
    uint8_t s1_state;
    uint8_t s2_state;
    uint8_t s3_state;
    uint8_t s4_state;
};

// Global variable storage
struct global_t {
    // global_nvs_t nvs;
    global_epoch_t epoch;
    global_sensors_t sensors;
    char idfVersion[32] = {0};
    char elfFileSha256[18] = {0};
};
#endif