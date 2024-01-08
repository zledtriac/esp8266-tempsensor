#include <OneWire.h>
#include "dstemp.hpp"
#include "config.hpp"

float getTemp(OneWire &wire, unsigned char* rom) {
  unsigned char i;
  unsigned char data[9];
  
  wire.reset();
  wire.select(rom);
  wire.write(0x44, 1);
  delay(1000);

  wire.reset();
  wire.select(rom);
  wire.write(0xBE);

  for(i = 0; i < 9; i++) {
    data[i] = wire.read();
  }

  if(OneWire::crc8(data, 8) != data[8]) return 400;

  uint16_t raw = (data[1] << 8) | data[0];

  return (float)raw / 16.0;
}
