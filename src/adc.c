#include "adc.h"

#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>

#include "led.h"

void setupAdc() {
    if (!adc_is_ready_dt(&adc_channels[0])) {
        printk("ADC controller device %s not ready\n",
               adc_channels[0].dev->name);
        return;
    }

    int err = adc_channel_setup_dt(&adc_channels[0]);
    if (err < 0) {
        printk("Could not setup channel #%d (%d)", 0, err);
        return;
    }
}

void adcInitSequence(uint32_t channel, struct adc_sequence *seq) {
    int err = adc_sequence_init_dt(&adc_channels[channel], seq);
    if (err < 0) {
        printk("Could not initalize sequnce");
        return;
    }
}

void adcRead(const struct device *dev, const struct adc_sequence *sequence) {
    int err = adc_read(dev, sequence);
    if (err < 0) {
        printk("Could not read (%d)\n", err);
        return;
    }
}