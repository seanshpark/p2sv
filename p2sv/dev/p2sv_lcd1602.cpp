#include "p2sv_lcd1602.h"

namespace
{

// clang-format off
#define HD44780_LCD_CMD_DDRAMADDR     0b10000000
#define HD44780_LCD_CMD_CGRAMADDR     0b01000000
#define HD44780_LCD_CMD_FUNCSET       0b00100000
#define HD44780_LCD_CMD_CURSOR        0b00010000
#define HD44780_LCD_CMD_DISPLAY       0b00001000
#define HD44780_LCD_CMD_ENTRY         0b00000100
#define HD44780_LCD_CMD_RETHOME       0b00000010
#define HD44780_LCD_CMD_CLEAR         0b00000001

// HD44780_LCD_CMD_FUNCSET
#define HD44780_LCD_FUNCSET_8BIT      0b00010000
#define HD44780_LCD_FUNCSET_4BIT      0b00000000
#define HD44780_LCD_FUNCSET_2LINES    0b00001000
#define HD44780_LCD_FUNCSET_1LINES    0b00000000
#define HD44780_LCD_FUNCSET_5x11      0b00000100
#define HD44780_LCD_FUNCSET_5x8       0b00000000

// HD44780_LCD_CMD_CURSOR
#define HD44780_LCD_CURSOR_SHIFT_DIS  0b00001000 // display shift
#define HD44780_LCD_CURSOR_SHIFT_CUR  0b00000000 // cursor move
#define HD44780_LCD_CURSOR_RIGHT      0b00000100 // shift to the right
#define HD44780_LCD_CURSOR_LEFT       0b00000000 // shift to the left

// HD44780_LCD_CMD_DISPLAY
#define HD44780_LCD_DISPLAY_ON        0b00000100
#define HD44780_LCD_DISPLAY_OFF       0b00000000
#define HD44780_LCD_DISPLAY_CUR       0b00000010
#define HD44780_LCD_DISPLAY_CUR_OFF   0b00000000
#define HD44780_LCD_DISPLAY_BLINK     0b00000001
#define HD44780_LCD_DISPLAY_BLINK_OFF 0b00000000

// HD44780_LCD_CMD_ENTRY
#define HD44780_LCD_ENTRY_INC         0b00000010 // increment vs decrement
#define HD44780_LCD_ENTRY_SHIFT       0b00000001 // entire shift on vs off

#define PCF8574_LCD1604_RS 0b00000001 // LCD160x_RS (Register Select: Inst / Data)
#define PCF8574_LCD1604_RW 0b00000010 // LCD160x_RW (R or /W)
#define PCF8574_LCD1604_EN 0b00000100 // LCD160x_EN (Enable)
#define PCF8574_LCD1604_BL 0b00001000 // LCD160x_BL (BackLight)

// clang-format on

#define LCD_I2C_ADDR 0x27

} // namespace

namespace p2sv
{

namespace
{

uint8_t _cgfont1[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f};
uint8_t _cgfont2[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f};
uint8_t _cgfont3[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f};
uint8_t _cgfont4[8] = {0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f};
uint8_t _cgfont5[8] = {0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f};
uint8_t _cgfont6[8] = {0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f};
uint8_t _cgfont7[8] = {0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f};
uint8_t _cgfont8[8] = {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f};

}

void LCD1602::init(I2C *i2c)
{
  _i2c = i2c;
  _i2c->init(LCD_I2C_ADDR);

  _display = true;
  _cursor = false;
  _blink = false;

  // turn on back-light
  _back_light = true;

  init_4bit();
  sleep_us(100);

  function_set(HD44780_LCD_FUNCSET_4BIT | HD44780_LCD_FUNCSET_2LINES | HD44780_LCD_FUNCSET_5x8);
  sleep_us(100);

  cursor_set(HD44780_LCD_CURSOR_SHIFT_CUR | HD44780_LCD_CURSOR_RIGHT);
  sleep_us(100);

  display_set();
  sleep_us(100);

  entrymode_set(HD44780_LCD_ENTRY_INC);
  sleep_us(100);

  // level custom fonts
  cgram(0, _cgfont1, 8);
  cgram(1, _cgfont2, 8);
  cgram(2, _cgfont3, 8);
  cgram(3, _cgfont4, 8);
  cgram(4, _cgfont5, 8);
  cgram(5, _cgfont6, 8);
  cgram(6, _cgfont7, 8);
  cgram(7, _cgfont8, 8);

  _lev_ch[0] = 0x20;
  _lev_ch[1] = 8;
  _lev_ch[2] = 9;
  _lev_ch[3] = 10;
  _lev_ch[4] = 11;
  _lev_ch[5] = 12;
  _lev_ch[6] = 13;
  _lev_ch[7] = 14;
  _lev_ch[8] = 15;

  _initalized = true;

  clear();
}

void LCD1602::splash(void)
{
  puts("RPi pico 2");
  move(1, 0);
  puts("SpecVis ");
  puts(P2SV_VERSION_STRING);
  sleep_ms(2000);
}

void LCD1602::clear()
{
  uint8_t cmd = HD44780_LCD_CMD_CLEAR;
  send_ctrl(cmd);
  sleep_us(5000); // wait 5ms
}

void LCD1602::home()
{
  uint8_t cmd = HD44780_LCD_CMD_RETHOME;
  send_ctrl(cmd);
  sleep_us(1600); // spec says 1.52ms
}

void LCD1602::display(bool enable)
{
  _display = enable;
  display_set();
  sleep_us(10);
}

void LCD1602::cursor(bool enable)
{
  _cursor = enable;
  display_set();
  sleep_us(10);
}

void LCD1602::blink(bool enable)
{
  _blink = enable;
  display_set();
  sleep_us(10);
}

void LCD1602::putc(const char c)
{
  send_data(c);
}

void LCD1602::puts(const char *str)
{
  while (*str != '\x0')
  {
    putc(*str++);
  }
}

void LCD1602::putch(uint8_t ch)
{
  send_data(ch);
}

void LCD1602::move(uint8_t row, uint8_t col)
{
  uint8_t cmd = HD44780_LCD_CMD_DDRAMADDR;
  uint8_t ram_offset;

  // row 0 : 0x00 ~ 0x27
  // row 1 : 0x40 ~ 0x67
  if (row > 0x01)
    row = 0x01; // use provide 2 lines
  if (col > 0x27)
    col = 0x27;

  ram_offset = row * 0x40 + col;
  ram_offset &= 0b01111111;

  send_ctrl(cmd + ram_offset);
}

void LCD1602::cgram(uint8_t ch, uint8_t *data, size_t leng)
{
  uint8_t cmd = HD44780_LCD_CMD_CGRAMADDR;
  // NOTE CGRAM address is mapped as 8 bytes per character
  // << 3 (== *8) to jump to address of ch
  cmd |= (ch << 3) & 0x3f;
  send_ctrl(cmd);
  sleep_us(10);

  for (size_t p = 0; p < leng; ++p)
  {
    send_data(data[p]);
    sleep_us(10);
  }
}

void LCD1602::level_init(void)
{
  // for one time init
}

/*
  Draw bars to LCD1602 
  'bars' must have 16 items with value of 0 ~ 16
*/
  void LCD1602::level_loop(uint16_t *bars)
  {
  uint16_t num = 16;
  char row0[20];
  char row1[20];

  row0[16] = 0;
  row1[16] = 0;
  for (uint16_t x = 0; x < num; ++x)
  {
    uint16_t lev = bars[x];
    if (lev >= 9)
    {
      if (lev > 16)
        lev = 16;
      row0[x] = _lev_ch[lev - 8];
      row1[x] = _lev_ch[8];
    }
    else
    {
      row0[x] = _lev_ch[0];
      row1[x] = _lev_ch[lev];;
    }
  }
  move(0, 0);
  puts(row0);
  move(1, 0);
  puts(row1);
}

//------------------------------------------------------------------------------

void LCD1602::init_4bit(void)
{
  uint8_t lcddata;
  uint8_t data;

  // send RS=0, RW=0, DB7~DB4=0011 as 8bit 3 times
  data = HD44780_LCD_CMD_FUNCSET | HD44780_LCD_FUNCSET_8BIT;

  lcddata = data & 0xf0;
  send_4bits(lcddata, 4500);

  lcddata = data & 0xf0;
  send_4bits(lcddata, 150);

  lcddata = data & 0xf0;
  send_4bits(lcddata, 150);

  // send RS=0, RW=0, DB7~DB4=0010 as 4bit 1 time
  data = HD44780_LCD_CMD_FUNCSET;
  lcddata = data & 0xf0;
  send_4bits(lcddata, 150);
}

void LCD1602::send_4bits(uint8_t lcddata, uint32_t delay)
{
  lcddata &= ~PCF8574_LCD1604_BL;

  // this sends 4bits to DB4~DB7 of HD44780
  // lower 4bits are used for control.
  // to write, send bits + EN high, wait 2usec, drop EN low, wait delay
  // data will be written falling edge
  lcddata |= PCF8574_LCD1604_EN;
  lcddata |= _back_light ? PCF8574_LCD1604_BL : 0;
  io_send_byte(lcddata);
  sleep_us(0);

  lcddata &= ~PCF8574_LCD1604_EN;
  lcddata |= _back_light ? PCF8574_LCD1604_BL : 0;
  io_send_byte(lcddata);
  sleep_us(delay);
}

void LCD1602::send_data(uint8_t data)
{
  uint8_t lcddata;

  // RS high is to select DATA
  // bit 7~4
  lcddata = (data & 0xf0) | PCF8574_LCD1604_RS;
  send_4bits(lcddata, 0);

  // bit 3~0
  lcddata = ((data & 0x0f) << 4) | PCF8574_LCD1604_RS;
  send_4bits(lcddata, 0);
}

void LCD1602::send_ctrl(uint8_t data)
{
  uint8_t lcddata;

  // TODO support send data with 8bits

  // RS low is to select CONTROL
  // bit 7~4
  lcddata = (data & 0xf0);
  send_4bits(lcddata, 0);

  // bit 3~0
  lcddata = ((data & 0x0f) << 4);
  send_4bits(lcddata, 0);
}

void LCD1602::function_set(uint8_t data)
{
  uint8_t cmd = HD44780_LCD_CMD_FUNCSET;
  cmd |= data;
  send_ctrl(cmd);
}

void LCD1602::cursor_set(uint8_t data)
{
  uint8_t cmd = HD44780_LCD_CMD_CURSOR;
  cmd |= data;
  send_ctrl(cmd);
}

void LCD1602::display_set(void)
{
  uint8_t cmd = HD44780_LCD_CMD_DISPLAY;
  cmd |= _display ? HD44780_LCD_DISPLAY_ON : 0;
  cmd |= _cursor ? HD44780_LCD_DISPLAY_CUR : 0;
  cmd |= _blink ? HD44780_LCD_DISPLAY_BLINK : 0;
  send_ctrl(cmd);
}

void LCD1602::entrymode_set(uint8_t data)
{
  uint8_t cmd = HD44780_LCD_CMD_ENTRY;
  cmd |= data;
  send_ctrl(cmd);
}

void LCD1602::io_send_byte(uint8_t one)
{
  _i2c->write(one);
}

std::unique_ptr<p2sv::Display> create_display(void)
{
  return std::make_unique<p2sv::LCD1602>();
}

} // namespace p2sv
