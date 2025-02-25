#include "NXP_ADC.hpp"

const struct adc_dt_spec NXP_ADC::adc_channels[] = {
    DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, DT_SPEC_AND_COMMA)};

NXP_ADC::NXP_ADC(int channel) : channel(channel) {
    adcSequence = {
        .buffer = &adcBuffer,
        .buffer_size = sizeof(adcBuffer),
    };
}

void NXP_ADC::setup() {
    if (!adc_is_ready_dt(&adc_channels[channel])) {
        printk("ADC controller device %s not ready\n",
               adc_channels[channel].dev->name);
        return;
    }

    int err = adc_channel_setup_dt(&adc_channels[channel]);
    if (err < 0) {
        printk("Could not setup channel #%d (%d)\n", channel, err);
        return;
    }

    err = adc_sequence_init_dt(&adc_channels[channel], &adcSequence);
    if (err < 0) {
        printk("Could not initialize sequence\n");
        return;
    }
}

uint32_t NXP_ADC::read() {
    int err = adc_read(adc_channels[channel].dev, &adcSequence);
    if (err < 0) {
        printk("ADC read failed (%d)\n", err);
        return 0;
    }
    return adcBuffer;
}
