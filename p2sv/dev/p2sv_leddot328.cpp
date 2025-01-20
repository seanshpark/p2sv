#include "p2sv_leddot328.h"

namespace p2sv
{

void LEDDOT328::init(I2C *)
{
  init_max7219();
}

void LEDDOT328::splash(void)
{
  // TODO
}

void LEDDOT328::clear(void)
{
  uint8_t c[4] = {0, 0, 0, 0};
  for (uint8_t d = 1; d <= 8; ++d)
    send4(d, c);
}

void LEDDOT328::level_init(void)
{
  clear();
}

void LEDDOT328::level_loop(uint16_t *bars)
{
  // bars have values of 0 ~ 8.
  // fill src pixels from 0 to bar level.
  static const uint8_t lev[9] = {0, 1, 3, 7, 15, 31, 63, 127, 255};
  uint8_t src[32], dst[32];
  for (uint8_t i = 0; i < 32; i++)
  {
    uint8_t val = bars[i] > 8 ? 8 : bars[i];
    src[i] = lev[val];
    dst[i] = 0;
  }

  // rorate 32 vertical src values to horizontal.
  // src 1 byte is one vertical line
  //   MSB [ ][ ][ ][ ]                                [  ]
  //       [ ][ ][ ][ ]
  //    8  [ ][ ][ ][ ]
  //  bits [ ][ ][ ][ ]
  //       [ ][ ][ ][ ]                                [  ]
  //   LSB [0][1][2][3]                                [31]
  //
  // dst 4 bytes are one horizontal line
  //     L8 [         ] [         ] [         ] [         ]
  //     L7 [         ] [         ] [         ] [         ]
  //     L?   LED 0        LED 1      LED 2       LED 3
  //     L2 [         ] [         ] [         ] [         ]
  //     L1 [012345678] [012345678] [012345678] [012345678]
  //          data[3]     data[2]     data[1]     data[0]
  for (uint8_t x = 0; x < 32; ++x)
  {
    static const uint8_t b8[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    for (uint8_t y = 0; y < 8; ++y)
    {
      bool pixel = (src[x] & b8[y]) ? true : false;
      uint8_t xoff = x & 7;
      dst[y * 4 + 3 - (x / 8)] |= pixel ? b8[xoff] : 0;
    }
  }

  // send 4 bytes per level
  uint8_t data[4];
  for (uint8_t d = 1; d <= 8; d++)
  {
    uint8_t os = (d - 1) * 4;
    data[0] = dst[os];
    data[1] = dst[os + 1];
    data[2] = dst[os + 2];
    data[3] = dst[os + 3];
    send4(d, data);
  }
}

//------------------------------------------------------------------------------

void LEDDOT328::init_max7219(void)
{
  gpio_put(MAX7219_CS, true);
  gpio_put(MAX7219_CLK, true);
  gpio_put(MAX7219_DIO, true);

  command(0x0c, 0x01); // shutdown: normal operation
  command(0x0f, 0x00); // test: normal operation
  command(0x09, 0x00); // decode: no decode
  command(0x0a, 0x01); // intensity: 1/32
  command(0x0b, 0x07); // use all digits
}

void LEDDOT328::command(uint8_t addr, uint8_t data)
{
  gpio_put(MAX7219_CS, false);
  for (uint8_t c = 0; c < 4; ++c)
  {
    io_send_byte(addr);
    io_send_byte(data);
  }
  gpio_put(MAX7219_CS, true);
}

void LEDDOT328::send(uint8_t addr, uint8_t data)
{
  gpio_put(MAX7219_CS, false);
  io_send_byte(addr);
  io_send_byte(data);
  gpio_put(MAX7219_CS, true);
}

void LEDDOT328::send4(uint8_t addr, uint8_t *d4)
{
  gpio_put(MAX7219_CS, false);
  io_send_byte(addr);
  io_send_byte(d4[0]);
  io_send_byte(addr);
  io_send_byte(d4[1]);
  io_send_byte(addr);
  io_send_byte(d4[2]);
  io_send_byte(addr);
  io_send_byte(d4[3]);
  gpio_put(MAX7219_CS, true);
}

void LEDDOT328::io_send_byte(uint8_t one)
{
  for (uint8_t b = 8; b; --b)
  {
    gpio_put(MAX7219_CLK, false);
    sleep_us(2);
    gpio_put(MAX7219_DIO, one & 0x80 ? true : false);
    sleep_us(2);
    gpio_put(MAX7219_CLK, true);
    sleep_us(2);
    one = one << 1;
  }
}

//------------------------------------------------------------------------------

std::unique_ptr<p2sv::Display> create_display(void)
{
  return std::make_unique<p2sv::LEDDOT328>();
}

} // namespace p2sv
