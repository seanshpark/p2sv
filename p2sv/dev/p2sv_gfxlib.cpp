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
/*
Software License Agreement (BSD License)

Copyright (c) 2012 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include "p2sv_gfxlib.h"

#include <cstdlib>

namespace p2sv
{

namespace
{

template <typename T> inline void _swap(T &a, T&b)
{
  T t = a;
  a = b;
  b = t;
}

inline void _swap_int16_t(int16_t &a, int16_t &b)
{
  _swap<int16_t>(a, b);
}

}

Gfx::Gfx(int16_t w, int16_t h)
{
  _width = w;
  _height = h;
}

void Gfx::write_pixel(int16_t x, int16_t y, uint16_t color)
{
  draw_pixel(x, y, color);
}

void Gfx::write_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  int16_t steep = std::abs(y1 - y0) > std::abs(x1 - x0);

  if (steep)
  {
    _swap_int16_t(x0, y0);
    _swap_int16_t(x1, y1);
  }

  if (x0 > x1)
  {
    _swap_int16_t(x0, x1);
    _swap_int16_t(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = std::abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1)
  {
    ystep = 1;
  }
  else
  {
    ystep = -1;
  }

  for (; x0 <= x1; x0++)
  {
    if (steep)
    {
      write_pixel(y0, x0, color);
    }
    else
    {
      write_pixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0)
    {
      y0 += ystep;
      err += dx;
    }
  }
}

void Gfx::write_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  fill_rect(x, y, w, h, color);
}

void Gfx::write_fast_vline(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  draw_fast_vline(x, y, h, color);
}

void Gfx::write_fast_hline(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  draw_fast_hline(x, y, w, color);
}

void Gfx::fill_screen(uint16_t color)
{
  fill_rect(0, 0, _width, _height, color);
}

void Gfx::fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  /*
  for (int16_t i = x; i < x + w; i++)
  {
    draw_fast_vline(i, y, h, color);
  }
  */
  for (int16_t i = y; i < y + h; i++)
  {
    write_fast_hline(x, i, w, color);
  }
}

void Gfx::draw_fast_vline(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  write_line(x, y, x, y + h - 1, color);
}

void Gfx::draw_fast_hline(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  write_line(x, y, x + w - 1, y, color);
}

void Gfx::draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  // Update in subclasses if desired!
  if (x0 == x1)
  {
    if (y0 > y1)
    {
      _swap_int16_t(y0, y1);
    }
    draw_fast_vline(x0, y0, y1 - y0 + 1, color);
  }
  else if (y0 == y1)
  {
    if (x0 > x1)
    {
      _swap_int16_t(x0, x1);
    }
    draw_fast_hline(x0, y0, x1 - x0 + 1, color);
  }
  else
  {
    write_line(x0, y0, x1, y1, color);
  }
}

} // namespace p2sv
