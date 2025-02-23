#ifndef __ADC_H
#define __ADC_H

#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
    !DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
    ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

static const struct adc_dt_spec adc_channels[] = {
    DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, DT_SPEC_AND_COMMA)};

void setupAdc();

void adcInitSequence(uint32_t channel, struct adc_sequence *seq);

void adcRead(const struct device *dev, const struct adc_sequence *sequence);

#endif /* __ADC_H */