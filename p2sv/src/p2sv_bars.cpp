#include "p2sv_conf.h"
#include "p2sv_bars.h"

#include <cmath>
#include <cstdio>

namespace p2sv
{

void Bars::init(int rate, int num, int low, int high)
{
  int num_bars_1 = num + 1;

  _sample_rate = rate;
  _num_bars = num;
  _cutoff_low = low;
  _cutoff_high = high;

  _cut_off_freq = new int[num_bars_1];
  _equalize = new float[num_bars_1];

  float freq_const = log10f((float)_cutoff_low / (float)_cutoff_high) /
                     (1 / ((float)num_bars_1) - 1);
  for (int n = 0; n < num_bars_1; ++n)
  {
    float bar_dist_coeff = - freq_const;
    bar_dist_coeff += ((float)n + 1) / ((float)num_bars_1) * freq_const;

    _cut_off_freq[n] = (int)(_cutoff_high * powf(10, bar_dist_coeff));

    // higher frequecy gets higher equalize value;
    _equalize[n] = powf(_cut_off_freq[n], 1.1f) / 13;
  }

  // cache fft index to frequency
  _freq_idx = new float[N_SAMPLE];
  for (int n = 0; n < N_SAMPLE; ++n)
    _freq_idx[n] = (float)_sample_rate * (float)n / N_SAMPLE;

  // buffer to accumulate powers
  _amplitudes = new float[num_bars_1];
}

void Bars::final(void)
{
  delete _amplitudes;
  delete _freq_idx;
  delete _equalize;
  delete _cut_off_freq;
}

int Bars::freq_l(int bar)
{
  if (0 <= bar && bar < _num_bars)
    return _cut_off_freq[bar];
  return _cutoff_low;
}

int Bars::freq_h(int bar)
{
  if (0 <= bar && bar < _num_bars)
    return _cut_off_freq[bar + 1];
  return _cutoff_high;
}

void Bars::reset(void)
{
  for (int n = 0; n < _num_bars; ++n)
    _amplitudes[n] = 0.0f;
}

} // namespace p2sv
