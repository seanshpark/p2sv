#ifndef __P2SV_I2C_H__
#define __P2SV_I2C_H__

#include <pico/stdlib.h>

namespace p2sv
{

class I2C
{
public:
  void init(uint8_t addr);

public:
  void write(uint8_t one);

private:
  uint8_t _addr = 0x00;

};

} // namespace p2sv

#endif // __P2SV_I2C_H__
