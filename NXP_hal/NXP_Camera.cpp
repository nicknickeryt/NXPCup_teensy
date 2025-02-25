#include "NXP_Camera.hpp"

#include <zephyr/sys/printk.h>

#include "pwm.h"

K_MSGQ_DEFINE(camera_dat_msgq, CAMERA_MSG_LEN, CAMERA_MSG_ARRAY_SIZE, 4);

bool NXP_Camera::cameraInterruptState = 0;

NXP_Camera::NXP_Camera(NXP_ADC& adc, NXP_GPIO& clkPin, NXP_GPIO& siPin)
    : cameraAdc(adc), cameraClkPin(clkPin), cameraSiPin(siPin) {}

void NXP_Camera::setup() {
    cameraClkPin.configure(GPIO_OUTPUT_ACTIVE);
    cameraSiPin.configure(GPIO_INPUT | GPIO_INT_EDGE_RISING);

    cameraClkPin.reset();

    cameraSiPin.configureInterrupt(GPIO_INT_EDGE_RISING,
                                   cameraInterruptHandler);

    cameraAdc.setup();

    cameraInterruptState = 0;

    // TODO convert to c++ thingy
    pwmInit(PWM_SIPWM);
    pwmSetPulseNs(PWM_SIPWM, 16000);
}

void NXP_Camera::startProc() {
    while (1) {  // FIXME: This should not be a blocking loop
        while (cameraInterruptState) {
            cameraDelayStartUs = k_cycle_get_32();

            for (unsigned int i = 0; i < CAMERA_ADC_SAMPLES; i++) {
                cameraClkPin.set();

                cameraBufArr[i] = cameraAdc.read();

                cameraDelayUsStart(i * CAMERA_DELAY_US + CAMERA_DELAY_US_HALF);
                cameraClkPin.reset();

                cameraDelayUsStart((i + 1) * CAMERA_DELAY_US);
            }

            cameraClkPin.set();
            cameraDelayUsStart(128 * CAMERA_DELAY_US + CAMERA_DELAY_US_HALF);
            cameraClkPin.reset();

            // TODO: not this way!
            cameraPutBufferMsgq();

            cameraInterruptState = 0;

            // TODO:
            // int steeringError = algorithmCalculatePosition(cameraBufArr);

            // printk("Steering error: %d\n", steeringError);
            // cameraDebugPrintk();

            // TODO:
            // servoSetDegrees(steeringError);
        }

        // TODO check it!
        // k_yield();
    }
}

void NXP_Camera::cameraInterruptHandler(const struct device* dev,
                                        struct gpio_callback* cb,
                                        uint32_t pins) {
    cameraInterruptState = 1;
}

void NXP_Camera::cameraPutBufferMsgq() {
    k_msgq_put(&camera_dat_msgq, &cameraBufArr, K_NO_WAIT);
}

void NXP_Camera::cameraDelayUs(uint32_t us) {
    uint32_t start = k_cycle_get_32();
    while (k_cyc_to_us_floor32(k_cycle_get_32() - start) < us);
}

void NXP_Camera::cameraDelayUsStart(uint32_t us) {
    while (k_cyc_to_us_floor32(k_cycle_get_32() - cameraDelayStartUs) < us);
}
