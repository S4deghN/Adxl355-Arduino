
# Adxl355-Arduino

A simple comprehensive Adxl355 wrapper library for Arduino framework

## Example 

```c++
#include <Arduino.h>
#include <SPI.h>
#include <Adxl355.h>

using namespace Adxl355;

#define CS 8
Adxl355::Adxl355 adxl (&SPI, CS);

void setup() {
    Serial.begin(115200);

    adxl.begin();
    adxl.setRange(Range::_2G);
    adxl.setHPF(HPF::NO_HPF);
    adxl.setODR(ODR::ODR_4000_Hz);
    adxl.wakeup();
}

void loop() {
    adxl.updateTxyz();

    int id = adxl.readByte(Reg::DEVID);
    Serial.print(id, HEX); Serial.print('\t');
    Serial.print(adxl.x); Serial.print('\t');
    Serial.print(adxl.y); Serial.print('\t');
    Serial.print(adxl.z); Serial.print('\t');
    Serial.print(adxl.temp); Serial.print('\t');
    Serial.println();
}
```

## Installation

Just like other Arduino libraried clone the repo to your `libraried` folder,
or download zip file and either unzip to `libraries` or inside Arduino IDE navigate to Sketch > Include Library > Add .ZIP Library and select the zip file.

### Arduino IDE libraries folder location
- Windows: "C:\Users\\{username}\Documents\Arduino\libraries"  
- macOS:   "/Users/{username}/Documents/Arduino/libraries"  
- Linux:   "/home/{username}/Arduino/libraries"

It's a tiny only header library and you could also just grab the .h file and put it into your project folder.

## Supported platforms

|Platform|Borad|Core lib|Tested|
|:-|:-|:-|:-|
|ST STM32|Blue Pill|[STM32duino](https://github.com/stm32duino/Arduino_Core_STM32)|Yes|
|Atmel AVR|Arduino Uno|[Arduino AVR](https://github.com/arduino/ArduinoCore-avr)|No|

## Contribution

Contribiutions for better Arduino IDE support are welcome.
