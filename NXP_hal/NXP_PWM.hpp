#ifndef __NXP_PWM_HPP
#define __NXP_PWM_HPP

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

// TODO improve this thing
#define PWM_CHANNEL_SIPWM (&pwmSi)
#define PWM_CHANNEL_SERVO (&pwmServo)

static const struct pwm_dt_spec pwmSi = PWM_DT_SPEC_GET(DT_NODELABEL(sipwm));
static const struct pwm_dt_spec pwmServo = PWM_DT_SPEC_GET(DT_NODELABEL(servo));

// TODO stop/start methods
class NXP_PWM {
   public:
    explicit NXP_PWM(const struct pwm_dt_spec* spec);

    void setup();

    // TODO check if pulseWidth is between device limits
    void setPulseWidthNs(uint32_t pulseWidth);
    void setPulseWidthUs(uint32_t pulseWidth);
    void setPulseWidthMs(uint32_t pulseWidth);

    const char* getDeviceName() const;

   private:
    const struct pwm_dt_spec* pwmSpec;
};

#endif /* __NXP_PWM_HPP */
