#include "p2sv_conf.h"
#include "p2sv_bars.h"

#include <cmath>
#include <cstdio>
#include <cstring>

namespace p2sv
{

void Bars::init(int rate, int samples, int num, int low, int high)
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
  _freq_idx = new float[samples];
  for (int n = 0; n < samples; ++n)
    _freq_idx[n] = (float)_sample_rate * (float)n / samples;

  // buffer to accumulate powers
  _amplitudes = new float[num_bars_1];

  // bars for level
  int output_channels = 2;
  int num_bar_per_chn = _num_bars;

  _total_bars = num_bar_per_chn * output_channels;

  _bars_left = (float *)malloc(sizeof(float) * num_bar_per_chn);
  _bars_right = (float *)malloc(sizeof(float) * num_bar_per_chn);
  memset(_bars_left, 0, sizeof(float) * num_bar_per_chn);
  memset(_bars_right, 0, sizeof(float) * num_bar_per_chn);

  _bars_raw = (float *)malloc(sizeof(float) * _total_bars);
  memset(_bars_raw, 0, sizeof(float) * _total_bars);

  _bars_val = (uint16_t *)malloc(sizeof(uint16_t) * _total_bars);
  memset(_bars_val, 0, sizeof(uint16_t) * _total_bars);
}

void Bars::final(void)
{
  delete _bars_val;
  delete _bars_raw;
  delete _bars_right;
  delete _bars_left;
  delete _amplitudes;
  delete _freq_idx;
  delete _equalize;
  delete _cut_off_freq;
}

int Bars::freqLo(int bar)
{
  if (0 <= bar && bar < _num_bars)
    return _cut_off_freq[bar];
  return _cutoff_low;
}

int Bars::freqHi(int bar)
{
  if (0 <= bar && bar < _num_bars)
    return _cut_off_freq[bar + 1];
  return _cutoff_high;
}

void Bars::fft2amp(float *fft)
{
  for (int b = 0; b < _num_bars; ++b)
    _amplitudes[b] = 0.0f;

  int sample_half = N_SAMPLE / 2;
  for (int i = 0; i < sample_half; ++i)
  {
    float freq = _freq_idx[i];
    float power = fft[i];
    for (int b = 0; b < _num_bars; ++b)
    {
      if (freqLo(b) <= freq && freq < freqHi(b))
      {
        _amplitudes[b] += power;
        break;
      }
    }
  }
}

void Bars::equalize(float *out)
{
  for (int b = 0; b < _num_bars; ++b)
  {
#if defined(P2SV_DISPLAY_LCD1602)
  float amp = sqrtf(_amplitudes[b]);
#elif defined(P2SV_DISPLAY_OLED12864)
  float amp = logf(sqrtf(_amplitudes[b]) + 1.0f);
#endif
    out[b] = amp * _equalize[b] / (freqHi(b) - freqLo(b) + 1);
  }
}

// merge left+right as low freq in center
// - Left(H,M,L)(L,M,H)Right
// - H:High, M:Middle, L:Low
void Bars::mergeCenter(void)
{
  for (int n = 0; n < _num_bars; n++)
    _bars_raw[n] = _bars_left[_num_bars - n - 1];
  for (int n = _num_bars; n < _total_bars; n++)
    _bars_raw[n] = _bars_left[n - _num_bars];
}

void Bars::generate(uint16_t num_levels)
{
  for (int n = 0; n < _total_bars; n++)
  {
    if (_bars_raw[n] < 0.0)
      _bars_val[n] = 0;
    else
    {
      _bars_val[n] = (uint16_t)(_bars_raw[n] * num_levels);
      if (_bars_val[n] > num_levels)
        _bars_val[n] = num_levels;
    }
  }
}

} // namespace p2sv
