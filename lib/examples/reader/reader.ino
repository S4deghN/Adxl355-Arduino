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