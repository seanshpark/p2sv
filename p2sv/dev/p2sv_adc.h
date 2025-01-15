#ifndef __P2SV_ADC_H__
#define __P2SV_ADC_H__

#include "p2sv_sys.h"

namespace p2sv
{

class ADC
{
  using adc_data_t = uint16_t;

public:
  void init(void);

public:
  // capture: chn; 0=left, 1=right
  void capture(int chn, uint samples);
  auto *buffer(int chn) { return chn == 0 ? adc_cap_l : adc_cap_r; }
  float value(int chn, uint32_t idx)
  {
    auto *chn_buff = buffer(chn);
    // ADC in 12bit -> -2^11 ~ 2^11
    // we expect audio signal to be 3.3Vpp
    // audio signal is biased at 3.3/2 volt to get range from 0 ~ 3.3V
    return ((float)(*(chn_buff + idx)) - 2048.0f) / 2048.0f;
  }

private:
  int _dma_chan = 0;
  dma_channel_config _cfg;

  adc_data_t *adc_cap_l = nullptr;
  adc_data_t *adc_cap_r = nullptr;
};

} // namespace p2sv

#endif // __P2SV_ADC_H__
