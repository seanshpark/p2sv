#include "p2sv_sys.h"

#include <cstdio>

namespace p2sv
{

void System::init(void)
{
  // do system related initialization
  stdio_init_all();
}

void System::info(void)
{
  printf("System Clock Frequency is %d MHz\n", clock_get_hz(clk_sys) / 1000000);
  printf("USB Clock Frequency is %d MHz\n", clock_get_hz(clk_usb) / 1000000);
  // For more examples of clocks use see https://github.com/raspberrypi/pico-examples/tree/master/clocks
}

void System::enable_wd(void)
{
  watchdog_enable(100, 1);
}

void System::kick_wd(void)
{
  watchdog_update();
}

} // namespace p2sv
