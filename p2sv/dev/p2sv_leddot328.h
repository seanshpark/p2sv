#ifndef __P2SV_LEDDOT328_H__
#define __P2SV_LEDDOT328_H__

// 4 of 8x8 DOT MATRIX LED with MAX7219

#include "p2sv_conf.h"
#include "p2sv_display.h"

#define LEDDOT328_NUM_LEVELS 16
#define LEDDOT328_BARS_CHN 8

namespace p2sv
{

class LEDDOT328 : public Display
{
public:
  void init(I2C *i2c) override;
  void splash(void) override;

public:
  uint16_t num_bars_per_chn(void) override { return LEDDOT328_NUM_LEVELS; }
  uint16_t num_levels(void) override { return LEDDOT328_BARS_CHN; };

public:
  void level_init(void) override;
  void level_loop(uint16_t *) override;
  void clear(void);

private:
  void init_max7219(void);
  void io_send_byte(uint8_t one);
  void command(uint8_t addr, uint8_t data);
  void send(uint8_t addr, uint8_t data);
  void send4(uint8_t addr, uint8_t *d4);
};

} // namespace p2sv

#endif // __P2SV_LEDDOT328_H__
