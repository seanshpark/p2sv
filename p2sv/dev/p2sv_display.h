#ifndef __P2SV_DISPLAY_H__
#define __P2SV_DISPLAY_H__

#include "p2sv_i2c.h"

namespace p2sv
{

class Display
{
public:
  virtual void init(I2C *i2c) = 0;
  virtual void splash(void) = 0;

  virtual void level_init(void) = 0;
  virtual void level_loop(uint16_t *) = 0;

public:
  virtual uint16_t num_bars_per_chn(void) = 0;
  virtual uint16_t num_levels(void) = 0;
};

} // namespace p2sv

#endif // __P2SV_DISPLAY_H__
