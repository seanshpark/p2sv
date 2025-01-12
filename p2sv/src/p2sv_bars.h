#ifndef __P2SV_BARS_H__
#define __P2SV_BARS_H__

namespace p2sv
{

class Bars
{
public:
  void init(int rate, int num, int low, int high);
  void final(void);

public:
  int freq_l(int bar);
  int freq_h(int bar);

  int num(void) { return _num_bars; }
  float freq(int sample) { return _freq_idx[sample]; }

  void reset(void);
  void acc(int bar, float amp) { _amplitudes[bar] += amp; }
  float amp(int bar) { return _amplitudes[bar]; }
  float equalize(int bar) { return _equalize[bar]; }

private:
  int _sample_rate = 0;
  int _num_bars = 0;
  int _cutoff_low = 0;
  int _cutoff_high = 0;
  int *_cut_off_freq = nullptr;
  float *_amplitudes = nullptr;
  float *_freq_idx = nullptr;
  float *_equalize = nullptr;
};

} // namespace p2sv

#endif // __P2SV_BARS_H__
