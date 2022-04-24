#include <Arduino.h>
#include <SPI.h>
#include "../lib/Adxl355.h"

using namespace Adxl355;

HardwareSerial Serial1(PA10, PA9);
// #define Serial1 Serial
// #define PB15 8
Adxl355::Adxl355 adxl (&SPI, PB15);

void setup() {
    Serial1.begin(115200);

    adxl.begin();
    adxl.setRange(Range::_2G);
    adxl.setHPF(HPF::NO_HPF);
    adxl.setODR(ODR::ODR_4000_Hz);
    adxl.wakeup();
}

void loop() {
    adxl.updateTxyz();

    Serial1.print(adxl.readByte(Reg::DEVID), HEX); Serial1.print('\t');
    Serial1.print(adxl.x); Serial1.print('\t');
    Serial1.print(adxl.y); Serial1.print('\t');
    Serial1.print(adxl.z); Serial1.print('\t');
    Serial1.print(adxl.temp); Serial1.print('\t');
    Serial1.println();
}