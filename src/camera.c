#include "camera.h"

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>

#include "adc.h"
#include "algorithm.h"
#include "camera.h"
#include "gpio.h"
#include "pwm.h"
#include "servo.h"

bool cameraInterruptState = 0;

uint32_t cameraDelayStartUs = 0;

uint32_t buf = 0;
uint32_t cameraBufArr[CAMERA_ADC_SAMPLES];
struct adc_sequence camera_adc_sequence = {.buffer = &buf,
                                           .buffer_size = sizeof(buf)};

K_MSGQ_DEFINE(camera_dat_msgq, CAMERA_MSG_LEN, CAMERA_MSG_ARRAY_SIZE, 4);

void initAdcCameraSequence() {
    adcInitSequence(CAMERA_ADC_CHANNEL, &camera_adc_sequence);
}

void cameraPutBufferMsgq() {
    k_msgq_put(&camera_dat_msgq, &cameraBufArr, K_NO_WAIT);
}

void cameraDelayUs(int32_t us) {
    uint32_t start = k_cycle_get_32();
    while (k_cyc_to_us_floor32(k_cycle_get_32() - start) < us);
}

void cameraDelayUsStart(int32_t us) {
    while (k_cyc_to_us_floor32(k_cycle_get_32() - cameraDelayStartUs) < us);
}

// TODO move main logic to another thread etc
void cameraProc() {
    while (1) {
        while (cameraInterruptState) {
            cameraDelayStartUs = k_cycle_get_32();

            for (unsigned int i = 0; i < CAMERA_ADC_SAMPLES; i++) {
                cameraSetCLKPin();

                adcRead(CAMERA_ADC_DEVICE, &camera_adc_sequence);

                cameraDelayUsStart(i * DELAY_US + DELAY_US_HALF);
                cameraResetCLKPin();

                cameraBufArr[i] = buf;
                cameraDelayUsStart((i + 1) * DELAY_US);
            }

            cameraSetCLKPin();
            cameraDelayUsStart((128) * DELAY_US + DELAY_US_HALF);
            cameraResetCLKPin();

            cameraPutBufferMsgq();

            cameraInterruptState = 0;
            int steeringError = algorithmCalculatePosition(cameraBufArr);

            // printk("Steering error: %d\n", steeringError);
            // cameraDebugPrintk();
            servoSetDegrees(steeringError);
        }
    }
}

void cameraSetCLKPin() {
    gpioSetPin(CAMERA_CLK_GPIO_PORT, CAMERA_CLK_GPIO_PIN);
}
void cameraResetCLKPin() {
    gpioResetPin(CAMERA_CLK_GPIO_PORT, CAMERA_CLK_GPIO_PIN);
}

void gpio_isr(const struct device *dev, struct gpio_callback *cb,
              uint32_t pins) {
    cameraInterruptState = 1;
}

void setupCamera() {
    setupGPIO();

    gpioConfigurePin(CAMERA_CLK_GPIO_PORT, CAMERA_CLK_GPIO_PIN,
                     GPIO_OUTPUT_ACTIVE);
    gpioConfigurePin(GPIO_PORT_2, GPIO_SI_INPUT_PIN,
                     GPIO_INPUT | GPIO_INT_EDGE_RISING);

    gpioSetPin(CAMERA_CLK_GPIO_PORT, CAMERA_CLK_GPIO_PIN);

    gpioConfigureInterrupt(GPIO_PORT_2, GPIO_SI_INPUT_PIN, GPIO_INT_EDGE_RISING,
                           gpio_isr);

    pwmInit(PWM_SIPWM);
    pwmSetPulseNs(PWM_SIPWM, 16000);

    initAdcCameraSequence();
}

void cameraDebugPrintk() {
    printk("[CAMERA-LOG] ADC readings:\n");
    uint32_t cameraBufferArr[CAMERA_ADC_SAMPLES];

    k_msgq_get(&camera_dat_msgq, &cameraBufferArr, K_NO_WAIT);

    char logBuffer[CAMERA_ADC_SAMPLES * 12];

    size_t offset = 0;
    for (unsigned int i = 0; i < CAMERA_ADC_SAMPLES; i++)
        offset += snprintf(logBuffer + offset, sizeof(logBuffer) - offset,
                           "%" PRId32 "\n", cameraBufferArr[i]);

    printk("%s\n", logBuffer);
}

void cameraDebugPrintkLoop() {
    while (1) {
        cameraDebugPrintk();
        k_msleep(500);
    }
}
