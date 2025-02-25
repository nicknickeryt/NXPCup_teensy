#ifndef __NXP_CAMERA_HPP
#define __NXP_CAMERA_HPP

#include <stdarg.h>
#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>

#include "NXP_ADC.hpp"
#include "NXP_GPIO.hpp"

#define CAMERA_CLK_GPIO_PORT GPIO_PORT_6
#define CAMERA_CLK_GPIO_PIN GPIO_PIN_3

// TODO get rid of this lame gpio interrupts
#define CAMERA_SI_GPIO_PORT GPIO_PORT_2
#define CAMERA_SI_GPIO_PIN GPIO_PIN_0

#define CAMERA_ADC_SAMPLES 128

// FIXME: This is a terrible way to do this
// (why is it causing an 'unused' warning??)
static uint32_t cameraBufArr[CAMERA_ADC_SAMPLES];

#define CAMERA_MSG_LEN sizeof(cameraBufArr)
#define CAMERA_MSG_ARRAY_SIZE 4

// TODO rename these
#define CAMERA_DELAY_US 36
#define CAMERA_DELAY_US_HALF CAMERA_DELAY_US / 2

// FIXME this is not the way I'd like it to be
extern struct k_msgq camera_dat_msgq;

// FIXME this won't work with more than 1 NXP_Camera object...
class NXP_Camera {
   public:
    NXP_Camera(NXP_ADC& adc, NXP_GPIO& clkPin, NXP_GPIO& siPin);

    void setup();
    void startProc();  // FIXME: We don't want a blocking while loop here

   private:
    NXP_ADC cameraAdc;
    NXP_GPIO cameraClkPin;
    NXP_GPIO cameraSiPin;

    static bool cameraInterruptState;
    uint32_t cameraDelayStartUs = 0;

    static void cameraInterruptHandler(const struct device* dev,
                                       struct gpio_callback* cb, uint32_t pins);

    void cameraPutBufferMsgq();
    void cameraDelayUs(uint32_t us);
    void cameraDelayUsStart(uint32_t us);
};

#endif /* __NXP_CAMERA_HPP */
