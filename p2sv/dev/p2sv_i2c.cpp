#include "p2sv_conf.h"
#include "p2sv_sys.h"
#include "p2sv_i2c.h"

namespace p2sv
{

void I2C::init(uint8_t addr)
{
  // I2C init at 400KHz
  i2c_init(DISPLAY_I2C_PORT, 400 * 1000);

  gpio_set_function(DISPLAY_I2C_SDA, GPIO_FUNC_I2C);
  gpio_set_function(DISPLAY_I2C_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(DISPLAY_I2C_SDA);
  gpio_pull_up(DISPLAY_I2C_SCL);

  _addr = addr;
}

void I2C::write(uint8_t one)
{
  i2c_write_blocking(i2c_default, _addr, &one, 1, false);
}

bool I2C::write_buffer(uint8_t *b, size_t s)
{
  /*
  while (s > 256)
  {
    i2c_write_blocking(i2c_default, _addr, b, 256, true);
    b += 256;
    s -= 256;
  }
  */
  i2c_write_blocking(i2c_default, _addr, b, s, false);
  return true;
}

} // namespace p2sv
