# Filament sensor and controller

## Filament Sensors

Using an IR slot sensor, commonly used for a speed / position sensor, can run at 3.3v or 5v.<br/>
Output is via LM393 voltage comparator producing a stable digital output wit +VCC being open and 0V closed.

STL files to print 3D printed casings can be found here [STL Files](hardware_optical_sensor)

![Sensor module](images/IR_Sensor_module.png)

## Controller

Hardware files, 3D print STL files, and laser cut SVGs can be found here [STL Files](hardware_ESP32_controller)

ESP32 C3 Super Mini

![ESP32 Super mini pinout ](images/pinout_ESP32_supermini.png)

CONNECTIONS:

- 5V → Power Input (USB or external 5V)
- GND → Common Ground
- 3V3 → 3.3V Output (for sensors)
- GPIO4 → Mode Switch (ADC input)
- GPIO1 → Buzzer Output
- GPIO0 → NeoPixel Data
- GPIO5 → Sensor 4 Signal
- GPIO10 → Sensor 3 Signal
- GPIO20 → Sensor 1 Signal
- GPIO21 → Sensor 2 Signal
- GPIO8 → Status LED (built-in)
