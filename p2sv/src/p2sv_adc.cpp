#include "p2sv_conf.h"
#include "p2sv_adc.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

namespace p2sv
{

void ADC::init(void)
{
  adc_gpio_init(ADC_CH_0);
  adc_gpio_init(ADC_CH_1);

  adc_init();
  adc_set_clkdiv(ADC_CLOCK_DIV);

  // capture with 12bit
  adc_fifo_setup(
      true,    // Write each completed conversion to the sample FIFO
      true,    // Enable DMA data request (DREQ)
      1,       // DREQ (and IRQ) asserted when at least 1 sample present
      false,   // We won't see the ERR bit because of 8 bit reads; disable.
      false    // Shift each sample to 8 bits when pushing to FIFO
  );

  _dma_chan = dma_claim_unused_channel(true);
  assert(_dma_chan != -1);
  _cfg = dma_channel_get_default_config(_dma_chan);
  channel_config_set_transfer_data_size(&_cfg, DMA_SIZE_16);
  channel_config_set_read_increment(&_cfg, false);
  channel_config_set_write_increment(&_cfg, true);
  channel_config_set_irq_quiet(&_cfg, true);
  channel_config_set_dreq(&_cfg, DREQ_ADC);

  // buffer
  adc_cap_l = (adc_data_t *)malloc(sizeof(adc_data_t) * N_SAMPLE);
  memset(adc_cap_l, 0, sizeof(adc_data_t) * N_SAMPLE);
  adc_cap_r = (adc_data_t *)malloc(sizeof(adc_data_t) * N_SAMPLE);
  memset(adc_cap_r, 0, sizeof(adc_data_t) * N_SAMPLE);
}

void ADC::capture(int chn, uint samples)
{
  auto *dest = buffer(chn);
  adc_select_input(chn);
  dma_channel_configure(_dma_chan, &_cfg,
    dest,           // dst
    &adc_hw->fifo,  // src
    samples,        // transfer count
    true            // start immediately
  );

  adc_run(true);
  dma_channel_wait_for_finish_blocking(_dma_chan);
  adc_run(false);
  adc_fifo_drain();
}

} // namespace p2sv
