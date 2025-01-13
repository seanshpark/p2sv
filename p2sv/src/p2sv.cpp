#include "p2sv_conf.h"
#include "p2sv_sys.h"
#include "p2sv_gpio.h"
#include "p2sv_i2c.h"
#include "p2sv_lcd1602.h"
#include "p2sv_adc.h"
#include "p2sv_fft.h"
#include "p2sv_proc.h"
#include "p2sv_bars.h"

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <cstring>

#include <memory.h>

struct rcb_ud_t
{
  p2sv::GPIO *pgpio;
  bool led;
};

bool alaram_repeating_cb(repeating_timer_t *rt)
{
  rcb_ud_t *ud = (rcb_ud_t *)rt->user_data;

  ud->pgpio->blink0(ud->led);
  ud->led = not ud->led;

  return true;
}

//------------------------------------------------------------------------------

void fft_to_bars(p2sv::FFT &fft, int dim, p2sv::Bars &bars, float *out)
{
  int num_bars = bars.num();
  int sample_half = N_SAMPLE / 2;
  for (int i = 0; i < sample_half; ++i)
  {
    float freq = bars.freq(i);
    float power = fft.output(i);
    for (int b = 0; b < num_bars; ++b)
    {
      if (bars.freq_l(b) <= freq && freq < bars.freq_h(b))
      {
        bars.acc(b, power);
        break;
      }
    }
  }

  for (int b = 0; b < num_bars; ++b)
  {
    float amp = sqrtf(bars.amp(b));
    amp *= bars.equalize(b) / (bars.freq_h(b) - bars.freq_l(b) + 1);
    out[b] = amp * dim;
  }
}

//------------------------------------------------------------------------------

int main(void)
{
  p2sv::System sys;
  p2sv::GPIO gpio;
  p2sv::I2C i2c;
  p2sv::LCD1602 lcd1602;
  p2sv::ADC adc;
  p2sv::FFT fft_l;
  p2sv::FFT fft_r;
  p2sv::Proc proc;
  p2sv::Bars bars;

  sys.init();
  gpio.init();

  // to reset lcd1602, power off-on
  gpio.lcd1602(false);
  sleep_ms(200);
  gpio.lcd1602(true);
  sleep_ms(10);

  i2c.init(0x27);
  lcd1602.init(&i2c);

  adc.init();
  fft_l.init();
  fft_r.init();
  proc.init();
  bars.init(SAMPLE_RATE, N_SAMPLE, LCD1602_BARS_CHN, 100, 15000);
  sys.info();

  // show info
  lcd1602.puts("RPi pico 2");
  lcd1602.move(1, 0);
  lcd1602.puts("SpecVis 0.1.0");
  sleep_ms(2000);

  // blinking on-board LED to show alive
  repeating_timer_t rt;
  alarm_pool_t *pool = alarm_pool_create(2, 1);
  rcb_ud_t ud;
  ud.pgpio = &gpio;
  ud.led = true;
  alarm_pool_add_repeating_timer_ms(pool, 500, alaram_repeating_cb, &ud, &rt);

  while (true)
  {
    // capture ADC left and right
    adc.capture(0, N_SAMPLE);
    adc.capture(1, N_SAMPLE);

    // ADC -> FFT with window
    float *buff_l = fft_l.buff();
    float *buff_r = fft_r.buff();
    for (uint32_t idx = 0; idx < N_SAMPLE; ++idx)
    {
      *(buff_l + idx) = adc.value(0, idx) * proc.window(idx);
      *(buff_r + idx) = adc.value(1, idx) * proc.window(idx);
    }
    fft_l.run();
    fft_r.run();

    bars.reset();
    fft_to_bars(fft_l, LCD1602_NUM_LEVELS, bars, bars.left());
    fft_to_bars(fft_r, LCD1602_NUM_LEVELS, bars, bars.right());

    bars.mergeCenter();
    bars.toDisplay(LCD1602_NUM_LEVELS);

    lcd1602.level(bars.display());
  }

  return 0;
}
