#ifndef __P2SV_SYSTEM_H__
#define __P2SV_SYSTEM_H__

#include <pico/stdlib.h>
#include <pico/time.h>

#include <hardware/i2c.h>
#include <hardware/dma.h>
#include <hardware/pio.h>
#include <hardware/timer.h>
#include <hardware/clocks.h>
#include <hardware/adc.h>
#include <hardware/dma.h>
#include <hardware/watchdog.h>

namespace p2sv
{

class System
{
public:
  void init(void);

public:
  void info(void);

public:
  void enable_wd(void);
  void kick_wd(void);
};

} // namespace p2sv

#endif // __P2SV_SYSTEM_H__
