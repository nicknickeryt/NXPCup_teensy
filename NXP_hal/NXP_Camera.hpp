#ifndef __NXP_CAMERA_HPP
#define __NXP_CAMERA_HPP

#include <stdarg.h>
#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>

#include "NXP_ADC.hpp"
#include "NXP_GPIO.hpp"
#include "NXP_PWM.hpp"
#include "NXP_UART.hpp"

#define CAMERA_CLK_GPIO_PORT GPIO_PORT_1
#define CAMERA_CLK_GPIO_PIN GPIO_PIN_25

// TODO get rid of this lame gpio interrupts
#define CAMERA_SI_GPIO_PORT GPIO_PORT_1
#define CAMERA_SI_GPIO_PIN GPIO_PIN_26

#define CAMERA_ADC_SAMPLES 128

#define CAMERA_MSG_LEN CAMERA_ADC_SAMPLES * sizeof(uint32_t)
#define CAMERA_MSG_ARRAY_SIZE 4

#define CAMERA_DELAY_US 36
#define CAMERA_DELAY_US_HALF CAMERA_DELAY_US / 2

#define CAMERA_LOG_ENABLED 1


class NXP_Camera {
   public:
    NXP_Camera(NXP_ADC& adc, NXP_GPIO& clkPin, NXP_GPIO& siPin, NXP_PWM& siPwm, NXP_UART& uart);

    void setup();
    void proc();  // FIXME: We don't want a blocking while loop here

    // TODO is using a pointer here a good idea?
    uint32_t* getCameraBufArr();

    static void cameraThreadWrapper(void* arg1, void* arg2, void* arg3);

   private:
    NXP_ADC& cameraAdc;
    NXP_GPIO cameraClkPin;
    NXP_GPIO cameraSiPin;
    NXP_PWM cameraSiPwm;
    NXP_UART& cameraLogUart;

    uint32_t cameraBufArr[CAMERA_ADC_SAMPLES];

    uint32_t cameraDelayStartUs = 0;

    // TODO this will break with more than 1 NXP_Camera object
    // We should find a better way to handle this, especially if we want to use a secondary camera
    static bool cameraInterruptState;
    static void cameraInterruptHandler(const struct device* dev,
                                struct gpio_callback* cb, uint32_t pins);

    void cameraDelayUs(uint32_t us);
    void cameraDelayUsStart(uint32_t us);

};

#endif /* __NXP_CAMERA_HPP */
