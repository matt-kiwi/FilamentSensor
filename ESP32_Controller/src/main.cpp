#include <Arduino.h>
#include <WiFi.h>  // Add WiFi header for macAddress()

// ESP32-C3 Super Mini LED pin
// Note: LED_BUILTIN is already defined in the variant, so we shouldn't redefine it
// Most ESP32-C3 boards have LED on GPIO8
// If not, try: 2, 7, 21, or check your board documentation

void setup() {
  // For ESP32-C3 with USB CDC, use Serial.begin() directly
  // No need to wait for Serial connection on ESP32-C3 USB
  Serial.begin(115200);
  
  // Small delay to allow serial monitor to connect
  delay(2000);
  
  // Initialize LED pin (use the built-in definition)
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);  // Start with LED off
  
  // Print welcome message
  Serial.println("\n\n=================================");
  Serial.println("ESP32-C3 Super Mini Controller");
  Serial.println("=================================");
  
  // Print system info
  Serial.printf("Chip Model: %s\n", ESP.getChipModel());
  Serial.printf("Chip Revision: %d\n", ESP.getChipRevision());
  Serial.printf("CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("Flash Size: %d MB\n", ESP.getFlashChipSize() / (1024 * 1024));
  Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
  Serial.printf("LED Pin: GPIO%d\n", LED_BUILTIN);
  Serial.println("System initialized successfully!");
  Serial.println("LED should be blinking...");
  Serial.println("Press Ctrl+C to stop monitor");
  Serial.println("=================================\n");
}

void loop() {
  // Blink LED
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED ON");
  delay(500);
  
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED OFF");
  delay(500);
  
  // Additional debug info every 10 cycles
  static int cycleCount = 0;
  cycleCount++;
  
  if (cycleCount % 10 == 0) {
    Serial.printf("[Cycle %d] Free Heap: %d bytes\n", 
                  cycleCount, ESP.getFreeHeap());
    Serial.printf("WiFi MAC: %s\n", WiFi.macAddress().c_str());
    Serial.printf("LED State: %s\n", digitalRead(LED_BUILTIN) ? "ON" : "OFF");
  }
}