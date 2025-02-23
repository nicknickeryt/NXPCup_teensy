#ifndef __SERVO_H
#define __SERVO_H

#include <stdint.h>

#define SERVO_PWM_MIN_NSEC 700000
#define SERVO_PWM_MAX_NSEC 2300000

#define SERVO_PWM_ZERO (SERVO_PWM_MAX_NSEC + SERVO_PWM_MIN_NSEC) / 2

#define SERVO_PWM_RANGE (SERVO_PWM_MAX_NSEC - SERVO_PWM_MIN_NSEC)

void setupServo();
void servoSetDegrees(float degrees);
void servoPulseSet(int32_t pulseWidth);

#endif /* __SERVO_H */