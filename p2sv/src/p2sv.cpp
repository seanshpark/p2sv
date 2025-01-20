#include "p2sv_conf.h"
#include "p2sv_sys.h"
#include "p2sv_gpio.h"
#include "p2sv_i2c.h"
#include "p2sv_adc.h"
#include "p2sv_fft.h"
#include "p2sv_proc.h"
#include "p2sv_bars.h"

#if defined(P2SV_DISPLAY_LCD1602)
  #include "p2sv_lcd1602.h"
#elif defined(P2SV_DISPLAY_OLED12864)
  #include "p2sv_oled12864.h"
#elif defined(P2SV_DISPLAY_LEDDOT328)
  #include "p2sv_leddot328.h"
#else
  #error "DISPLAY NOT DEFINED"
#endif

#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <memory>

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

int main(void)
{
  p2sv::System sys;
  p2sv::GPIO gpio;
  p2sv::ADC adc;
  p2sv::FFT fft_l;
  p2sv::FFT fft_r;
  p2sv::Proc proc;
  p2sv::Bars bars;
  p2sv::I2C i2c;

  sys.init();
  gpio.init();

  // to reset display(LCD1602), power off-on
  gpio.display(false);
  sleep_ms(200);
  gpio.display(true);
  sleep_ms(10);

  auto display = p2sv::create_display();
  display->init(&i2c);

  adc.init();
  fft_l.init();
  fft_r.init();
  proc.init();

  display->splash();

  uint16_t bars_chn = display->num_bars_per_chn();
  bars.init(SAMPLE_RATE, N_SAMPLE, bars_chn, 100, 15000);

  display->level_init();

  sys.info();

  // blinking on-board LED to show alive
  repeating_timer_t rt;
  alarm_pool_t *pool = alarm_pool_create(2, 1);
  rcb_ud_t ud;
  ud.pgpio = &gpio;
  ud.led = true;
  alarm_pool_add_repeating_timer_ms(pool, 500, alaram_repeating_cb, &ud, &rt);

  //sys.enable_wd();
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

    bars.fft2amp(fft_l.output());
    bars.equalize(bars.left());
    bars.fft2amp(fft_r.output());
    bars.equalize(bars.right());

    bars.mergeCenter();
    bars.generate(display->num_levels());

    display->level_loop(bars.display());

    //sys.kick_wd();
  }

  return 0;
}
