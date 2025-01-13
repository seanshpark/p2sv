#ifndef __P2SV_FFT_H__
#define __P2SV_FFT_H__

#include <kiss_fft.h>
#include <kiss_fftr.h>

namespace p2sv
{

class FFT
{
public:
  void init(void);
  void final(void);

public:
  void run(void);

public:
  kiss_fft_scalar *buff(void) { return _fft_in; }
  float *output(void) { return _out_sq; }

private:
  kiss_fft_scalar *_fft_in = nullptr;
  kiss_fft_cpx *_fft_out = nullptr;
  kiss_fftr_cfg _cfg = nullptr;
  float *_out_sq = nullptr;
};

} // namespace p2sv

#endif// __P2SV_FFT_H__
