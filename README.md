# Filament sensor and controller

## Project summary / purpose

Used in the manufacturing process of making and recycling plastic. Specifically to detect if a strand of filament has been broken, then alert the operator with an audio alarm.

![Filament Controller and Sensors](images/FilamentControllerAndSensors.png)

### Dedicated to Bill and Eva

This device is dedicated to Bill and Eva of KiwiFil plastics in New Zealand. And their pioneering work with plastics recycling.

## Usage and wiring

![Filamanet controller with labels](images/FilamentSensorControllerLayout.png)

### Wiring

- Remove top lid with finger screws.
- Pull out switch block assembly / cable entry plate.
- Connect all 4 filament sensors using screw terminals, if a sensor is missing bridge Ground and Sensor input (Yellow).
- Install switch block assembly aligning sensor cables in combe/clamp.
- Install top lid

### Usage

Controller is powered by USB/5v some USB-C PD power supplies may not work.<br/>
Turn on power switch, during boot up the first LED will be blue, and a power up tune will be played.

\*\* One day when the WiFi software has been written the WiFi setup switch will enable configuring the WiFi

The "Alarm mode" switch enables or disables the audible alarm.

The alarm will sound when any of the sensors fail to detect filament.<br >
LEDs

- Alarm: Red in alarm, Green no alarm
- Sensors 1 - 4: Red in alarm, Green no alarm

Adjusting the volume, the underside of the controller has a 2 Watt speaker and volume control, turn clockwise to increase the volume.
![Speaker and volume](images/FilamentSensorControllerUnderSide.png)

## Serial Menu system

Some settings are user settable via the serial menu system, you will need to connect a USB-C cable to the internal USB-C port on the Super Mini, then use your favorite terminal program.

Mine is Cool Term by Roger Meiers [https://freeware.the-meiers.org/](https://freeware.the-meiers.org/)

Press enter twice to see menu options.

```TEXT
Reconnecting to /dev/cu.usbmodem1201 ...         Connected!
LOG TEST: before LOG_I
[I][MAIN] LOG TEST: inside LOG_I
LOG TEST: after LOG_I
[I][MAIN] Initializing system...
[I][MAIN] =================================
[I][MAIN] Filament Sensor by Econode
[I][MAIN] =================================
[I][MAIN] Chip Model: ESP32-C3
[I][MAIN] Chip Revision: 4
[I][MAIN] CPU Frequency: 160 MHz
[I][MAIN] Flash Size: 4 MB
[I][MAIN] Free Heap: 261332 bytes
[I][MAIN] NeoPixel Pin: GPIO2
[I][MAIN] System initialized successfully!
[I][MAIN] =================================


                                                                                                    
                *############*.                                        
             +##################*-                                     
           .######################*:                                   
          .#########################+                                  
          *###########################=                                
          ##################################*                          
          -##############################% ##*.                        
           ###################################=                        
            +###################################+                      
              ########################+#####-=#####-                   
                .##################               ###=                 
                   ##*:=****=::                     .#*.               
                  ##=                                 .*=              
                 ##*                                    *+             
                ###*                                     :=            
              :#####*                                    =*=           
             *##########+                               -#*            
                                                       :#* 

         _  ___          _ ______ _ _     ___   ___ ___   __  
        | |/ (_)        (_)  ____(_) |   |__ \ / _ \__ \ / /  
        | ' / ___      ___| |__   _| |      ) | | | | ) / /_  
        |  < | \ \ /\ / / |  __| | | |     / /| | | |/ / '_ \ 
        | . \| |\ V  V /| | |    | | |    / /_| |_| / /| (_) |
        |_|\_\_| \_/\_/ |_|_|    |_|_|   |____|\___/____\___/ 


Available Tunes:
 0: JingleBells          1: Nokia                2: Mario                3: JamesBond         
 4: PinkPanther          5: ImperialMarch        6: AddamsFamily         7: MissionImpossible 
 8: IndianaJones         9: Tetris              10: HappyBirthday       11: Canon             
12: FurElise            13: GameOfThrones       14: HarryPotter         15: Zelda             
16: MarioPowerUp        17: StarWars            18: Beethoven5th        19: MontyPython       
20: MASH                21: Startup             22: Success             23: Error             
24: Alert               25: AmbientLoop       
Current alarm tune: 13 - GameOfThrones
Startup tune: 22 - Success

To select an alarm, type 'a' then the number (e.g. a15).
To select the startup tune, type 's' then the number (e.g. s21).
Type 'p' then number to play/test a song (e.g. p3), space to stop test.
Type 'c' to show current alarm and startup tune.
a19
Alarm tune selected: 19 - MontyPython

```

## Filament Sensor Optical

Using an IR slot sensor, commonly used for a speed / position sensor, can run at 3.3v or 5v.<br/>
Output is via LM393 voltage comparator producing a stable digital output with +VCC being open and 0V closed.

![Sensor module](images/IR_Sensor_module.png)

STL files to print 3D printed casings can be found here [STL Files](hardware_optical_sensor)<br />
![STL image sensor top](images/SensorPhotoTop_STL.png) ![STL image sensor base](images/SensorPhotoBase_STL.png)<br />

Completed sensors with 3D printed housing<br />
![](images/SensorPhoto1.png)![](images/SensorPhoto2.png)

## Flament Sensor See Saw

The optical sensor had issues with translucent filaments, so a new mechanical design based on a kids see saw.
There is an M10 x 5 Magnet on the underside of the see saw, which biases the weight, and is used to close a reed switch. In the base there is a reed switch.

Closed IE pulled to ground is the Alarm condition, this is the reverse logic to the optical sensor.

![](images/Fillament_seasaw_sensor.png)

STL files are here;

[sensor_seesaw_base_top.stl](hardware_seesaw_sensor/sensor_seesaw_base_top.stl) ,
[sensor_seesaw_base_underside.stl](hardware_seesaw_sensor/sensor_seesaw_base_underside.stl) ,
[sensor_seesaw_seesaw.stl](hardware_seesaw_sensor/sensor_seesaw_seesaw.stl)

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

## Schematic ( Vero Board prototype )

Schematic PDF File here: [FilamentSensorProjectVeroBoard.pdf](docs/FilamentSensorProjectVeroBoard.pdf)<br />
![Board Schematic](images/FilamentControlerVeroBoardSchematic.png)

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
