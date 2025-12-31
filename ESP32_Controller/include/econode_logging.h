// ============================================================================
// ECONODE SIMPLE LOGGING SYSTEM (from gen6_blinky)
// ============================================================================
//
// AI GUIDELINES:
// 1. Include: #include "econode_logging.h"
// 2. Use: LOG_E("TAG", "Error: %d", err);
// 3. Use: LOG_W, LOG_I, LOG_D for warn/info/debug
// 4. Use IF_DEBUG for debug-only code
// 5. Tags: MAIN, LORA, RTC, SENSOR, BATT, CONFIG, NVRAM
// ============================================================================

#ifndef ECONODE_LOGGING_H
#define ECONODE_LOGGING_H

#include <Arduino.h>
#include "config.h"

// ============================================================================
// CONFIGURATION
// ============================================================================
#ifndef ECONODE_LOG_LEVEL
  #define ECONODE_LOG_LEVEL 2  // WARN by default
#endif

#ifndef ECONODE_ENABLE_DEBUG_CODE
  #define ECONODE_ENABLE_DEBUG_CODE 0
#endif

// ============================================================================
// LOG LEVELS
// ============================================================================
#define LOG_NONE  0
#define LOG_ERROR 1
#define LOG_WARN  2
#define LOG_INFO  3
#define LOG_DEBUG 4

// ============================================================================
// PER-FILE LOG LEVEL OVERRIDES
// ============================================================================
#ifdef ECONODE_FILE_LOG_LEVEL
  #define CURRENT_LOG_LEVEL ECONODE_FILE_LOG_LEVEL
#else
  #define CURRENT_LOG_LEVEL ECONODE_LOG_LEVEL
#endif

// ============================================================================
// SIMPLE LOG MACROS
// ============================================================================
#if CURRENT_LOG_LEVEL >= LOG_ERROR
  #define LOG_E(tag, fmt, ...) do { \
    Serial.printf("[E][%s] " fmt "\n", tag, ##__VA_ARGS__); \
  } while(0)
#else
  #define LOG_E(tag, fmt, ...)
#endif

#if CURRENT_LOG_LEVEL >= LOG_WARN
  #define LOG_W(tag, fmt, ...) do { \
    Serial.printf("[W][%s] " fmt "\n", tag, ##__VA_ARGS__); \
  } while(0)
#else
  #define LOG_W(tag, fmt, ...)
#endif

#if CURRENT_LOG_LEVEL >= LOG_INFO
  #define LOG_I(tag, fmt, ...) do { \
    Serial.printf("[I][%s] " fmt "\n", tag, ##__VA_ARGS__); \
  } while(0)
#else
  #define LOG_I(tag, fmt, ...)
#endif

#if CURRENT_LOG_LEVEL >= LOG_DEBUG
  #define LOG_D(tag, fmt, ...) do { \
    Serial.printf("[D][%s] " fmt "\n", tag, ##__VA_ARGS__); \
  } while(0)
#else
  #define LOG_D(tag, fmt, ...)
#endif

// ============================================================================
// DEBUG-ONLY CODE
// ============================================================================
#if ECONODE_ENABLE_DEBUG_CODE && CURRENT_LOG_LEVEL >= LOG_DEBUG
  #define IF_DEBUG(...) __VA_ARGS__
#else
  #define IF_DEBUG(...)
#endif

// ============================================================================
// HELPER FUNCTION
// ============================================================================
inline void econode_log_init(unsigned long baud = 115200) {
  static bool s_initialized = false;
  if (!s_initialized) {
    Serial.begin(baud);
    #if ECONODE_ENABLE_DEBUG_CODE
      uint32_t start = millis();
      while (!Serial && (millis() - start < 3000)) {
        delay(10);
      }
    #endif
    s_initialized = true;
    LOG_I("LOG", "Econode logging started at %lu baud", baud);
    LOG_I("LOG", "Log level: %d", ECONODE_LOG_LEVEL);
  }
}

#endif // ECONODE_LOGGING_H
