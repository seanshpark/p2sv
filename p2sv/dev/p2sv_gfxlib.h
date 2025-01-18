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

#ifndef __P2SV_GFXLIB_H__
#define __P2SV_GFXLIB_H__

#include <stdint.h>

namespace p2sv
{

class Gfx {
public:
  Gfx(int16_t w, int16_t h);
  Gfx(const Gfx &other) = delete;
  Gfx() = delete;

public:
  virtual void draw_pixel(int16_t x, int16_t y, uint16_t color) = 0;

  // TRANSACTION API / CORE DRAW API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  virtual void write_pixel(int16_t x, int16_t y, uint16_t color);
  virtual void write_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  virtual void write_fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  virtual void write_fast_vline(int16_t x, int16_t y, int16_t h, uint16_t color);
  virtual void write_fast_hline(int16_t x, int16_t y, int16_t w, uint16_t color);

  // BASIC DRAW API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  virtual void fill_screen(uint16_t color);
  virtual void fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  virtual void draw_fast_vline(int16_t x, int16_t y, int16_t h, uint16_t color);
  virtual void draw_fast_hline(int16_t x, int16_t y, int16_t w, uint16_t color);
  virtual void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

  int16_t width(void) const { return _width; };
  int16_t height(void) const { return _height; }

protected:
  int16_t _width = 0;
  int16_t _height = 0;
};

} // namespace p2sv

#endif // __P2SV_GFXLIB_H__
