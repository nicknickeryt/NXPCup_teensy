#include "servo.h"

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>

#include "pwm.h"

static const struct pwm_dt_spec pwmServo = PWM_DT_SPEC_GET(DT_NODELABEL(servo));

void setupServo() { pwmInit(&pwmServo); }

void servoSetDegrees(float degrees) {
    // if (degrees < -45)
    //     degrees = -45;
    // else if (degrees > 45)
    //     degrees = 45;

    int32_t pulseWidth = (SERVO_PWM_RANGE / 180.0f) * degrees + SERVO_PWM_ZERO;

    servoPulseSet(pulseWidth);
}

void servoPulseSet(int32_t pulseWidth) { pwmSetPulseNs(&pwmServo, pulseWidth); }
