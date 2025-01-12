#include "p2sv_conf.h"
#include "p2sv_fft.h"

namespace p2sv
{

void FFT::init(void)
{
  _cfg = kiss_fftr_alloc(N_SAMPLE, false, NULL, NULL);
  _fft_in = new kiss_fft_scalar[N_SAMPLE];
  _fft_out = new kiss_fft_cpx[N_SAMPLE];
}

void FFT::final(void)
{
  delete _fft_in;
  delete _fft_out;
  kiss_fft_free(_cfg);
}

void FFT::run(void)
{
  kiss_fftr(_cfg, _fft_in, _fft_out);
}

kiss_fft_scalar FFT::output(int idx)
{
  kiss_fft_cpx &cpx = *(_fft_out + idx);
  kiss_fft_scalar pwr = cpx.r * cpx.r + cpx.i * cpx.i;
  return pwr;
}

} // namespace p2sv
