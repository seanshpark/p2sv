#ifndef __P2SV_GPIO_H__
#define __P2SV_GPIO_H__

#include "p2sv_sys.h"

namespace p2sv
{

class GPIO
{
public:
  void init(void);

public:
  void lcd1602(bool on);

public:
  void blink0(bool set);
};

} // namespace p2sv

#endif// __P2SV_GPIO_H__