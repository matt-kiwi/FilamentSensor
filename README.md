# Filament sensor and controller

## Project summary / purpose

Used in the manufacturing process of making and recycling plastic. Specifically to detect if a strand of filament has been broken, then alert the operator with an audio alarm.

![Filament Controller and Sensors](images/FilamentControllerAndSensors.png)

### Dedicated to Bill and Eva

This device is dedicated to Bill and Eva of KiwiFil plastics in New Zealand. And their pioneering work with plastics recycling.

## Filament Sensors

Using an IR slot sensor, commonly used for a speed / position sensor, can run at 3.3v or 5v.<br/>
Output is via LM393 voltage comparator producing a stable digital output wit +VCC being open and 0V closed.

STL files to print 3D printed casings can be found here [STL Files](hardware_optical_sensor)

![Sensor module](images/IR_Sensor_module.png)

Completed sensors with 3D printed housing

![](images/SensorPhoto1.png) ![](images/SensorPhoto2.png)

## Controller

Hardware files, 3D print STL files, and laser cut SVGs can be found here [STL Files](hardware_ESP32_controller)

ESP32 C3 Super Mini

![ESP32 Super mini pinout ](images/pinout_ESP32_supermini.png)

CONNECTIONS:

- 5V → Power Input (USB or external 5V)
- GND → Common Ground
- 3V3 → 3.3V Output (for sensors)
- GPIO4 → Mode Switches (ADC input)
- GPIO3 → Speaker Output
- GPIO2 → NeoPixel Data
- GPIO5 → Sensor 4 Signal
- GPIO10 → Sensor 3 Signal
- GPIO20 → Sensor 1 Signal
- GPIO21 → Sensor 2 Signal
- GPIO8 → Status LED (built-in)

## Sound / Alarm

Alarm will sound when any of the 4 sensors don't detect any filament.<br />
Sound can be disabled by turning the "Alarm" switch to off.<br />
Volume can be adjusted by the volume knob on the underside of the control unit.<br />

The sound library is by Econode using RTTTL / Nokia ringtones.
[https://github.com/matt-kiwi/ESP32-C3-RTTTL_MusicPlayer](https://github.com/matt-kiwi/ESP32-C3-RTTTL_MusicPlayer)<br />

Amplifier and Speaker unit;<br />
![](images/KeyStudio_SC8002B.png)

## Network / WiFi

Not currently implemented, the ESP32-C3 module has WiFi and antenna, there is a "setup mode" switch.

I envisage in the future all the factory alarms etc will be network enabled.
