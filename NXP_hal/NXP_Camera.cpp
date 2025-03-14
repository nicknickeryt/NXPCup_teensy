#include "NXP_Camera.hpp"

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "NXP_UART.hpp"

bool NXP_Camera::cameraInterruptState = 0;

K_MSGQ_DEFINE(camera_msgq, CAMERA_MSG_LEN, CAMERA_MSG_ARRAY_SIZE, 4);

NXP_Camera::NXP_Camera(NXP_ADC& adc, NXP_GPIO& clkPin, NXP_GPIO& siPin,
                       NXP_PWM& siPwm, NXP_UART& uart)
    : cameraAdc(adc),
      cameraClkPin(clkPin),
      cameraSiPin(siPin),
      cameraSiPwm(siPwm),
      cameraLogUart(uart) {}

void NXP_Camera::setup() {
    cameraClkPin.configure(GPIO_OUTPUT_ACTIVE);
    cameraSiPin.configure(GPIO_INPUT | GPIO_INT_EDGE_RISING);

    cameraClkPin.reset();

    cameraSiPin.configureInterrupt(GPIO_INT_EDGE_RISING,
                                   cameraInterruptHandler);

    cameraAdc.setup();

    cameraInterruptState = 0;

    cameraSiPwm.setup();
    cameraSiPwm.setPulseWidthUs(16);
}

void NXP_Camera::proc() {
    while (1) { 
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

#ifdef CAMERA_LOG_ENABLED
            cameraLogUart.write("\nCAML");
            for (unsigned int i = 0; i < CAMERA_ADC_SAMPLES; i++)
                cameraLogUart.write(".%hhu", (uint8_t)(cameraBufArr[i] >> 2));
#endif

            // TODO:
            // int steeringError = algorithmCalculatePosition(cameraBufArr);

            // printk("Steering error: %d\n", steeringError);
            // cameraDebugPrintk();

            // TODO:
            // servoSetDegrees(steeringError);
            k_msgq_put(&camera_msgq, &cameraBufArr, K_NO_WAIT);

            cameraInterruptState = 0;
        }

        k_yield();
    }
}

uint32_t* NXP_Camera::getCameraBufArr() { return cameraBufArr; }

void NXP_Camera::cameraInterruptHandler(const struct device* dev,
                                        struct gpio_callback* cb,
                                        uint32_t pins) {
    cameraInterruptState = 1;
}

void NXP_Camera::cameraDelayUs(uint32_t us) {
    uint32_t start = k_cycle_get_32();
    while (k_cyc_to_us_floor32(k_cycle_get_32() - start) < us) k_yield();
}

void NXP_Camera::cameraDelayUsStart(uint32_t us) {
    while (k_cyc_to_us_floor32(k_cycle_get_32() - cameraDelayStartUs) < us)
        k_yield();
}

void NXP_Camera::cameraThreadWrapper(void* arg1, void* arg2, void* arg3) {
    static_cast<NXP_Camera*>(arg1)->proc();
}