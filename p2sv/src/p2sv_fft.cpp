#include "p2sv_conf.h"
#include "p2sv_fft.h"

namespace p2sv
{

void FFT::init(void)
{
  _cfg = kiss_fftr_alloc(N_SAMPLE, false, NULL, NULL);
  _fft_in = new kiss_fft_scalar[N_SAMPLE];
  _fft_out = new kiss_fft_cpx[N_SAMPLE];
  _out_sq = new float[N_SAMPLE / 2];
}

void FFT::final(void)
{
  delete _out_sq;
  delete _fft_in;
  delete _fft_out;
  kiss_fft_free(_cfg);
}

void FFT::run(void)
{
  kiss_fftr(_cfg, _fft_in, _fft_out);

  int sample_h = N_SAMPLE / 2;
  for (int n = 0; n < sample_h; ++n)
  {
    kiss_fft_cpx &cpx = *(_fft_out + n);
    _out_sq[n] = cpx.r * cpx.r + cpx.i * cpx.i;
  }
}

} // namespace p2sv
