#include "p2sv_conf.h"
#include "p2sv_sys.h"
#include "p2sv_i2c.h"

namespace p2sv
{

void I2C::init(uint8_t addr)
{
  // I2C init at 400KHz
  i2c_init(LCD1602_I2C_PORT, 400 * 1000);

  gpio_set_function(LCD1602_I2C_SDA, GPIO_FUNC_I2C);
  gpio_set_function(LCD1602_I2C_SCL, GPIO_FUNC_I2C);
  gpio_pull_up(LCD1602_I2C_SDA);
  gpio_pull_up(LCD1602_I2C_SCL);

  _addr = addr;
}

void I2C::write(uint8_t one)
{
  i2c_write_blocking(i2c_default, _addr, &one, 1, false);
}

} // namespace p2sv
