#ifndef __PWM_H
#define __PWM_H

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

#define PWM_SIPWM (&sipwm)  // Makro wskazujące na strukturę

static const struct pwm_dt_spec sipwm = PWM_DT_SPEC_GET(DT_NODELABEL(sipwm));

void pwmInit(const struct pwm_dt_spec *spec);

void pwmSetPulseNs(const struct pwm_dt_spec *spec, uint32_t pulseWidth);

const char * pwmGetDeviceName(const struct pwm_dt_spec *spec);

#endif /* __PWM_H */
