#ifndef __NXP_PWM_HPP
#define __NXP_PWM_HPP

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>

// TODO improve this thing
#define PWM_CHANNEL_CAMERA_SI (&pwm_camera_si)
#define PWM_CHANNEL_ESC0 (&pwm_esc0)
#define PWM_CHANNEL_ESC1 (&pwm_esc1)
#define PWM_CHANNEL_SERVO (&pwm_servo)

static const struct pwm_dt_spec pwm_camera_si = PWM_DT_SPEC_GET(DT_NODELABEL(pwm_camera_si));
static const struct pwm_dt_spec pwm_esc0 = PWM_DT_SPEC_GET(DT_NODELABEL(pwm_esc0));
static const struct pwm_dt_spec pwm_esc1 = PWM_DT_SPEC_GET(DT_NODELABEL(pwm_esc1));
static const struct pwm_dt_spec pwm_servo = PWM_DT_SPEC_GET(DT_NODELABEL(pwm_servo));


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
