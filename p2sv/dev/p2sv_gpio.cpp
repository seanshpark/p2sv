#include "p2sv_gpio.h"
#include "p2sv_conf.h"

namespace p2sv
{

void GPIO::init(void)
{
  gpio_init(PICO_DEFAULT_LED_PIN);
  gpio_init(DISPLAY_PWR_CTRL);

  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
  gpio_set_dir(DISPLAY_PWR_CTRL, GPIO_OUT);
}

void GPIO::display(bool on)
{
  gpio_put(DISPLAY_PWR_CTRL, on);
}

void GPIO::blink0(bool set)
{
  gpio_put(PICO_DEFAULT_LED_PIN, set);
}

} // namespace p2sv
