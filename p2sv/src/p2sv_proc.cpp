#include "p2sv_conf.h"
#include "p2sv_proc.h"

#include <cmath>

#ifndef M_PI
#define M_PI 3.1415926535897932385f
#endif

namespace p2sv
{

void Proc::init(void)
{
  _window = new float[N_SAMPLE];
  for (int n = 0; n < N_SAMPLE; ++n)
    _window[n] = 0.5f * (1.0f - cosf(2.0f * M_PI * n / (N_SAMPLE - 1)));
  // _window is 0 at both ends and 1.0 at center
}

void Proc::final(void)
{
  delete _window;
}

} // namespace p2sv
