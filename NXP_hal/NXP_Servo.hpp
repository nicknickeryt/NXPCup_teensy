#ifndef __SERVO_HPP
#define __SERVO_HPP

#include <stdint.h>

#include "NXP_PWM.hpp"

// TODO add user interface to change these
#define SERVO_PWM_MIN_NSEC 700000
#define SERVO_PWM_MAX_NSEC 2300000

#define SERVO_PWM_ZERO (SERVO_PWM_MAX_NSEC + SERVO_PWM_MIN_NSEC) / 2

#define SERVO_PWM_RANGE (SERVO_PWM_MAX_NSEC - SERVO_PWM_MIN_NSEC)

class NXP_Servo {
   public:
    NXP_Servo(NXP_PWM& pwm);
    void setup();
    void setDegrees(float degrees);

   private:
    NXP_PWM& servoPwm;
    float servoDegrees;

    void setPulseWidth(int32_t pulseWidth);
};

#endif /* __SERVO_HPP */