#include "NXP_PWM.hpp"

#include <zephyr/sys/printk.h>

NXP_PWM::NXP_PWM(const struct pwm_dt_spec* spec) : pwmSpec(spec) {}

void NXP_PWM::setup() {
    if (!device_is_ready(pwmSpec->dev)) {
        printk("Error: PWM device %s is not ready!\n", pwmSpec->dev->name);
    }
}

void NXP_PWM::setPulseWidthNs(uint32_t pulseWidth) {
    int err = pwm_set_pulse_dt(pwmSpec, pulseWidth);
    if (err < 0) {
        printk("Error: Failed to set PWM pulse width (%d) on %s\n", err,
               pwmSpec->dev->name);
               return;
    }
    this->pulseWidthNs = pulseWidth;
}

void NXP_PWM::setPulseWidthUs(uint32_t pulseWidthMs) {
    uint32_t pulseWidthNs = pulseWidthMs * 1000;
    setPulseWidthNs(pulseWidthNs);
}

void NXP_PWM::setPulseWidthMs(uint32_t pulseWidthMs) {
    uint32_t pulseWidthNs = pulseWidthMs * 1000000;
    setPulseWidthNs(pulseWidthNs);
}

void NXP_PWM::setDutyCycle(int dutyCycle) {
    uint32_t periodNs = pwmSpec->period;
    uint32_t pulseWidthNs = (dutyCycle * periodNs) / 100;
    setPulseWidthNs(pulseWidthNs);
}

int NXP_PWM::getDutyCycle() {
    int periodNs = pwmSpec->period;
    int dutyCycle = (pulseWidthNs * 100) / periodNs;

    return dutyCycle;
}

const char* NXP_PWM::getDeviceName() const { return pwmSpec->dev->name; }
