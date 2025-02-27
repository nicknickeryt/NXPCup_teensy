#include "NXP_Servo.hpp"

NXP_Servo::NXP_Servo(NXP_PWM& pwm) : servoPwm(pwm) {}

void NXP_Servo::setup() { servoPwm.setup(); }

void NXP_Servo::setDegrees(float degrees) {
    // if (degrees < -45)
    //     degrees = -45;
    // else if (degrees > 45)
    //     degrees = 45;

    int32_t pulseWidth = (SERVO_PWM_RANGE / 180.0f) * degrees + SERVO_PWM_ZERO;

    this->setPulseWidth(pulseWidth);
}

void NXP_Servo::setPulseWidth(int32_t pulseWidth) { servoPwm.setPulseWidthNs(pulseWidth); }