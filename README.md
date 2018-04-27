# icosahedronPet
A prototype of an interactive digital icosahedron shaped pet using the accelerometer as sensor input

What you need:
  - Arduino. For this tutrial, a wemos is used, but not necessary.
  - GY-85 Sensor
  - Cardboard and lasercutter
  - LED strips lights
  - [FastLED Library](https://github.com/FastLED/FastLED)
  - [Adafruit Unified Sensor Driver](https://github.com/adafruit/Adafruit_Sensor)
  - [Adafruit ADXL345 Driver](https://github.com/adafruit/Adafruit_ADXL345)

Connect the LED strips by 3 wires: 
  - GND -> GND
  - +5v -> 5v
  - DO  -> D6
  
Connect the GY-85 Sensor by 4 wire:
  - SDA -> D2
  - SCL -> D1
  - GND -> GND
  - VCC_IN -> 3V3
  
![](Tutorial_Images/internalPet.png?raw=true)


Download appropriate drivers/lib

Download and then upload the source code to your arduino.

Cut out the icosahedron using the included svg file on a laser cutter and glue it together with a glue gun.

![](Tutorial_Images/icosahedron.png?raw=true?raw=true)


Put your assempled arduino into your cardboard icosahedron.

You pet should now be ready for play.

![](Tutorial_Images/holdingPet.png?raw=true?raw=true?raw=true)
