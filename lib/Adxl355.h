#ifndef ADXL355_H
#define ADXL355_H

#include <Arduino.h>
#include <SPI.h>

namespace Adxl355 {

  const char SPI_W = 0;
  const char SPI_R = 1;

  enum Reg {
    DEVID_AD = 0x00,
    DEVID_MST = 0x01,
    DEVID = 0x02,  
    REVID = 0x03,
    STATUS = 0x04,
    FIFO_ENTRIES = 0x05,
    TEMP2 = 0x06,
    TEMP1 = 0x07,
    XDATA3 = 0x08,
    XDATA2 = 0x09,
    XDATA1 = 0x0A,
    YDATA3 = 0x0B,
    YDATA2 = 0x0C,
    YDATA1 = 0x0D,
    ZDATA3 = 0x0E,
    ZDATA2 = 0x0F,
    ZDATA1 = 0x10,
    FIFO_DATA = 0x11,
    OFFSET_X_H = 0x1E,
    OFFSET_X_L = 0x1F,
    OFFSET_Y_H = 0x20,
    OFFSET_Y_L = 0x21,
    OFFSET_Z_H = 0x22,
    OFFSET_Z_L = 0x23,
    ACT_EN = 0x24,
    ACT_THRESH_H = 0x25,
    ACT_THRESH_L = 0x26,
    ACT_COUNT =  0x27,
    FILTER = 0x28,
    FIFO_SAMPLES = 0x29,
    INT_MAP = 0x2A,
    SYNC = 0x2B,
    RANGE = 0x2C,
    POWER_CTL = 0x2D,
    SELF_TEST = 0x2E,
    RESET = 0x2F
  };

  namespace Range {
    struct type {
      uint8_t code;
      double coef;
    };
    const type _2G = { 0b01, 256000.0 };
    const type _4G = { 0b10, 128000.0 };
    const type _8G = { 0b11, 64000.0  };
  }


  namespace ODR {
    struct type {
      uint8_t code;
    };
    const type ODR_4000_Hz   = { 0 };  // odr = 4000 Hz and lpf = 1000 Hz
    const type ODR_2000_Hz   = { 1 };  // odr = 2000 Hz and lpf = 500 Hz
    const type ODR_1000_Hz   = { 2 };  // odr = 1000 Hz and lpf = 250 Hz
    const type ODR_500_Hz    = { 3 };  // odr = 500 Hz and lpf = 125 Hz
    const type ODR_250_Hz    = { 4 };  // odr = 250 Hz and lpf = 62.5 Hz
    const type ODR_125_Hz    = { 5 };  // odr = 125 Hz and lpf = 31.25 Hz
    const type ODR_62_5_Hz   = { 6 };  // odr = 62.5 Hz and lpf = 15.625 Hz
    const type ODR_31_25_Hz  = { 7 };  // odr = 31.25 Hz and lpf = 7.813 Hz
    const type ODR_15_625_Hz = { 8 };  // odr = 15.625 Hz and lpf = 3.906
    const type ODR_7_813_Hz  = { 9 };  // odr = 7.813 Hz and lpf = 1.953 Hz
    const type ODR_3_906_Hz  = { 10 }; // odr = 3.906 Hz and lpf = 0.977 Hz
  }

  namespace HPF {
    struct type {
      uint8_t code;
    };
    const type NO_HPF      = { 0 }; // no high pass filter
    const type _247_ODR    = { 1 }; // corner freq = 247 × 10^3 × ODR
    const type _62_084_ODR = { 2 }; // corner freq = 62.048 × 10^3 × ODR
    const type _15_545_ODR = { 3 }; // corner freq = 15.454 × 10^3 × ODR
    const type _3_862_ODR  = { 4 }; // corner freq = 3.862 × 10^3 × ODR
    const type _0_954_ODR  = { 5 }; // corner freq = 0.954 × 10^3 × ODR
    const type _0_238_ODR  = { 6 }; // corner freq = 0.238 × 10^3 × ODR
  }


  class Adxl355 {
  public:
    Adxl355(SPIClass *spi, uint32_t cs, uint32_t spiFreq = 10000000)
    :spi(spi), cs(cs), spiFreq(spiFreq) {}

    void begin();
    void resetSpi(); // reset spi settings to this objects config
    void wakeup();
    void sleep();
    void setRange(Range::type range);
    void setODR(ODR::type odr);
    void setHPF(HPF::type hpf);

    uint8_t readByte(uint32_t addr);
    void readBytes(uint32_t addr, uint8_t *bytes, size_t len);
    void writeByte(uint32_t addr, uint8_t byte);
    void writeBytes(uint32_t addr, uint8_t *bytes, size_t len);
    void updateT();
    void updateXyz();
    void updateTxyz();

    float x;
    float y;
    float z;
    float temp;

  private:
    SPIClass *spi;
    uint32_t cs;
    uint32_t spiFreq;
    
    Range::type range = Range::_2G;
    ODR::type odr = ODR::ODR_4000_Hz;
    HPF::type hpf = HPF::NO_HPF;
  };

  void Adxl355::begin() {
    pinMode(cs, OUTPUT);
    digitalWrite(cs, HIGH);

    spi->beginTransaction(SPISettings(spiFreq, MSBFIRST, SPI_MODE0));
  }

  void Adxl355::resetSpi() {
    spi->beginTransaction(SPISettings(spiFreq, MSBFIRST, SPI_MODE0));
  }

  void Adxl355::wakeup() {
    writeByte(Reg::POWER_CTL, 0);
  }

  void Adxl355::sleep() {
    writeByte(Reg::POWER_CTL, 1);
  }

  void Adxl355::setRange(Range::type range) {
    this->range = range;
    writeByte(Reg::RANGE, range.code);
  }

  void Adxl355::setODR(ODR::type odr) {
    this->odr = odr;
    uint8_t byte = (this->hpf.code & 0x07) << 4 | (this->odr.code & 0x0F);
    writeByte(Reg::FILTER, byte);
  }

  void Adxl355::setHPF(HPF::type hpf) {
    this->hpf = hpf;
    uint8_t byte = (this->hpf.code & 0x07) << 4 | (this->odr.code & 0x0F);
    writeByte(Reg::FILTER, byte);
  }

  uint8_t Adxl355::readByte(uint32_t addr) {
    uint8_t bytes[] = {(addr << 1) | SPI_R, 0};
    digitalWrite(cs, LOW);
    spi->transfer(bytes, 2);
    digitalWrite(cs, HIGH);
    return bytes[1];
  }

  void Adxl355::readBytes(uint32_t addr, uint8_t *bytes, size_t len) {
    bytes[0] = (addr << 1) | SPI_R;
    digitalWrite(cs, LOW);
    spi->transfer(bytes, 12);
    digitalWrite(cs, HIGH);
  }

  void Adxl355::writeByte(uint32_t addr, uint8_t byte) {
    uint8_t bytes[] = {(addr << 1) | SPI_W, byte};
    digitalWrite(cs, LOW);
    spi->transfer(bytes, 2);
    digitalWrite(cs, HIGH);
  }

  void Adxl355::writeBytes(uint32_t addr, uint8_t *bytes, size_t len) {
    return;
  }

  void Adxl355::updateTxyz() {

    uint8_t bytes[12];
    readBytes(Reg::TEMP2, bytes, 12);

    int16_t temp = int16_t(bytes[1]) << 8 | bytes[2];
    
    int32_t x = ((int32_t(bytes[3]) << 24) | (int32_t(bytes[4])  << 16) | int32_t(bytes[5]  & 0xF0) << 8) >> 12;
    int32_t y = ((int32_t(bytes[6]) << 24) | (int32_t(bytes[7])  << 16) | int32_t(bytes[8]  & 0xF0) << 8) >> 12;
    int32_t z = ((int32_t(bytes[9]) << 24) | (int32_t(bytes[10]) << 16) | int32_t(bytes[11] & 0xF0) << 8) >> 12;

    this->temp = ((temp - 1852) / (-9.05)) + 25; // below is simplified version of this line
    // this->temp = temp / -9.05 + 229.640883978;
    
    this->x = float(x) / range.coef;
    this->y = float(y) / range.coef;
    this->z = float(z) / range.coef;
  }

  void Adxl355::updateXyz() {

    uint8_t bytes[10];
    readBytes(Reg::XDATA3, bytes, 10);

    int32_t x = ((int32_t(bytes[1]) << 24) | (int32_t(bytes[2]) << 16) | int32_t(bytes[3] & 0xF0) << 8) >> 12;
    int32_t y = ((int32_t(bytes[4]) << 24) | (int32_t(bytes[5]) << 16) | int32_t(bytes[6] & 0xF0) << 8) >> 12;
    int32_t z = ((int32_t(bytes[7]) << 24) | (int32_t(bytes[8]) << 16) | int32_t(bytes[9] & 0xF0) << 8) >> 12;

    this->x = float(x) / range.coef;
    this->y = float(y) / range.coef;
    this->z = float(z) / range.coef;
  }

  void Adxl355::updateT() {

    uint8_t bytes[2];
    readBytes(Reg::TEMP2, bytes, 2);

    int16_t temp = int16_t(bytes[1]) << 8 | bytes[2];

    this->temp = ((temp - 1852) / (-9.05)) + 25; // below is simplified version of this line
    // this->temp = temp / -9.05 + 229.640883978;
  }

} // namespace Adxl355
#endif