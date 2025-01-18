/*
 * Copyright 2024 saehie.park@gmail.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __P2SV_OLED12864_H__
#define __P2SV_OLED12864_H__

#include "p2sv_i2c.h"
#include "p2sv_display.h"
#include "p2sv_gfxlib.h"

#define OLED12864_NUM_LEVELS 32
#define OLED12864_BARS_CHN 16

#define OLED_BLACK  0
#define OLED_WHITE  1
#define OLED_INVERSE 2

namespace p2sv
{

class OLED12864 : public Display, public Gfx
{
public:
  OLED12864();
  virtual ~OLED12864() = default;

public:
  void init(I2C *i2c) override;
  void release(void);
  void splash(void) override;

public:
  uint16_t num_bars_per_chn(void) override { return OLED12864_BARS_CHN; }
  uint16_t num_levels(void) override { return OLED12864_NUM_LEVELS; };

public:
  bool initialized(void) { return _initalized; }

public:
  void clear_display(void);
  void flush_display(void);
  void draw_pixel(int16_t x, int16_t y, uint16_t color) override;

protected:
  void draw_fast_vline(int16_t x, int16_t y, int16_t w, uint16_t color) override;
  void draw_fast_hline(int16_t x, int16_t y, int16_t h, uint16_t color) override;
  void draw_fast_hline_internal(int16_t x, int16_t y, int16_t w, uint16_t color);
  void draw_fast_vline_internal(int16_t x, int16_t y, int16_t h, uint16_t color);

protected:
  uint8_t *_buffer = nullptr;

public:
  // low level SSD1306 commands
  bool cmd(uint8_t v);
  bool data(uint8_t v);
  bool data(uint8_t *b, size_t l);
  bool data(uint8_t *b, uint32_t s, uint32_t l);

  bool addrMode(uint8_t v);
  bool colAddr(uint8_t s, uint8_t e);
  bool pageAddr(uint8_t s, uint8_t e);

  void clear();

private:
  void init_ssd1306(void);
  void chk_resize_temp(size_t s);

private:
  I2C *_i2c = nullptr;
  uint8_t *_temp_buff = nullptr;
  uint32_t _temp_size = 0;
  bool _initalized = false;

public:
  void level_init(void);
  void level_loop(uint16_t *bars);

public:
  void test_init(void);
  void test_loop(void);

private:
  uint16_t _test_c = 0;
  uint16_t _test_x = 0;
  uint16_t _test_y = 0;
  uint16_t _test_xy = 0;
  bool _test_line_x = false;
};

} // namespace p2sv

#endif // __P2SV_OLED12864_H__
