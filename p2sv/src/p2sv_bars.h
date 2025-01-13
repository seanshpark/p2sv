#ifndef __P2SV_BARS_H__
#define __P2SV_BARS_H__

#include <cstdint>

namespace p2sv
{

class Bars
{
public:
  void init(int rate, int samples, int num, int low, int high);
  void final(void);

public:
  void fft2amp(float *fft);
  void equalize(float *out);
  void mergeCenter(void);
  void toDisplay(uint16_t num_levels);

public:
  int freqLo(int bar);
  int freqHi(int bar);

  int num(void) { return _num_bars; }
  int total(void) { return _total_bars; }

  float *left(void) { return _bars_left; }
  float *right(void) { return _bars_right; }
  float *merged(void) { return _bars_raw; }
  uint16_t *display(void) { return _bars_val; }

private:
  int _sample_rate = 0;
  int _num_bars = 0;
  int _cutoff_low = 0;
  int _cutoff_high = 0;
  int *_cut_off_freq = nullptr;
  float *_amplitudes = nullptr;
  float *_freq_idx = nullptr;
  float *_equalize = nullptr;

  int _total_bars = 0;
  float *_bars_left = nullptr;
  float *_bars_right = nullptr;
  float *_bars_raw = nullptr;

  // for display
  uint16_t *_bars_val = nullptr;
};

} // namespace p2sv

#endif // __P2SV_BARS_H__
