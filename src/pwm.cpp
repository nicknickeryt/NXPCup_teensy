#include "pwm.h"


void pwmInit(const struct pwm_dt_spec *spec) {
    int err = device_is_ready(spec->dev);
    if (err) {
        printk("Error in pwmInit#device_is_ready(), err: %d\n", err);
        return;
    }
}

void pwmSetPulseNs(const struct pwm_dt_spec *spec, uint32_t pulseWidth) {
    int err = pwm_set_pulse_dt(spec, pulseWidth);
    if (err) {
        printk("Error in pwmSetPulseNs#pwm_set_pulse_dt(), err: %d\n", err);
        return;
    }
}

const char *pwmGetDeviceName(const struct pwm_dt_spec *spec) {
    return spec->dev->name;
}