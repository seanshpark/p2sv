#ifndef __P2SV_LCD1602_H__
#define __P2SV_LCD1602_H__

#include "p2sv_sys.h"
#include "p2sv_i2c.h"

namespace p2sv
{

class LCD1602
{
public:
  void init(I2C *i2c);

public:
  void clear();
  void home();
  void display(bool enable); // display on/off
  void cursor(bool enable);  // cursor on/off
  void blink(bool enable);   // blinking on/off
  void puts(const char *str);
  void putch(uint8_t ch);
  void move(uint8_t row, uint8_t col);
  void cgram(uint8_t ch, uint8_t *data, size_t leng);

  void level(uint16_t *bars);

private:
  void function_set(uint8_t data);
  void cursor_set(uint8_t data);
  void display_set(void);
  void entrymode_set(uint8_t data);
  void putc(const char c);

private:
  void init_4bit(void);
  void send_4bits(uint8_t lcddata, uint32_t delay);
  void send_data(uint8_t data);
  void send_ctrl(uint8_t data);

private:
  void io_send_byte(uint8_t one);

private:
  I2C *_i2c = nullptr;
  bool _initalized = false;
  bool _back_light = false;
  bool _display = false;
  bool _cursor = false;
  bool _blink = false;

  char _lev_ch[9];
};

} // namespace p2sv

#endif// __P2SV_LCD1602_H__