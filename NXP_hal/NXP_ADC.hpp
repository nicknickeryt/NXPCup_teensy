#ifndef NXP_ADC_HPP
#define NXP_ADC_HPP

#include <zephyr/drivers/adc.h>
#include <zephyr/kernel.h>

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
    !DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
    ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1
#define ADC_CHANNEL_2 2
#define ADC_CHANNEL_3 3
// shouldn't we move that ^ to a separate defines file?

class NXP_ADC {
   public:
    explicit NXP_ADC(int channel);
    void setup();
    uint32_t read();

   private:
    uint32_t adcBuffer = 0;
    struct adc_sequence adcSequence;
    static const struct adc_dt_spec adc_channels[];
    int channel;
};

#endif  // NXP_ADC_HPP
