#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "NXP_ADC.hpp"
#include "NXP_Algorithm.hpp"
#include "NXP_Blinky.hpp"
#include "NXP_Camera.hpp"
#include "NXP_Encoder.hpp"
#include "NXP_GPIO.hpp"
#include "NXP_PWM.hpp"
#include "NXP_Servo.hpp"
#include "NXP_UART.hpp"
#include "NXP_USB.hpp"

#define ALGORITHM_STACKSIZE 8192
#define CAMERA_STACKSIZE 8192
#define CAMERA_LOG_STACKSIZE 8192

#define CAMERA_PROC_PRIORITY 100
#define ALGORITHM_PRIORITY 7
#define CAMERA_LOG_PRIORITY 3

NXP_GPIO kittyAmberLed(BLINKY_GPIO_PORT, BLINKY_GPIO_PIN,
                       GPIO_OUTPUT_ACTIVE);

NXP_GPIO kittyEncoderPinA(ENCODER_GPIO_PORT, ENCODER_A_GPIO_PIN);
NXP_GPIO kittyEncoderPinB(ENCODER_GPIO_PORT, ENCODER_B_GPIO_PIN);

NXP_Blinky kittyBlinky(kittyAmberLed);
NXP_USB kittyLogUSB;

NXP_ADC kittyCameraAdc(ADC_CHANNEL_0);
NXP_GPIO kittyCameraClkPin(CAMERA_CLK_GPIO_PORT, CAMERA_CLK_GPIO_PIN);
NXP_GPIO kittyCameraSiPin(CAMERA_SI_GPIO_PORT, CAMERA_SI_GPIO_PIN);

NXP_PWM kittyCameraSiPwm(PWM_CHANNEL_CAMERA_SI);

NXP_UART kittyUart(UART_BT);

NXP_Camera kittyCamera(kittyCameraAdc, kittyCameraClkPin, kittyCameraSiPin,
                       kittyCameraSiPwm, kittyUart);

NXP_PWM kittyPwmESC0(PWM_CHANNEL_ESC0);
NXP_PWM kittyPwmESC1(PWM_CHANNEL_ESC1);
NXP_PWM kittyPwmServo(PWM_CHANNEL_SERVO);

NXP_Servo kittyServo(kittyPwmServo);

NXP_Algorithm kittyAlgorithm(kittyCamera, kittyServo);

NXP_Encoder kittyEncoder(kittyEncoderPinA, kittyEncoderPinB);

int main() {
    kittyLogUSB.setup();
    kittyCameraAdc.setup();

    kittyCameraSiPwm.setup();
    kittyCamera.setup();

    kittyPwmESC0.setup();
    kittyPwmESC1.setup();

    kittyPwmServo.setup();

    kittyEncoder.setup();

    printk("Kitty v2 says hello <3\n");
    uint32_t freq = CLOCK_GetCpuClkFreq();
    printk("CPU Freq: %d\n", freq);
    kittyUart.write("Kitty v2 says UART!\n");
}

void demo() {
    int i = 0;
    bool direction = 1;

    while (1) {
        printk("\nENCODER");

        printk("Encoder pulse: %" PRId64 "\r\n", kittyEncoder.getCount());
        printk("Encoder RPM: %u\r\n", kittyEncoder.getRPM());
        printk("Direction: %d\r\n", kittyEncoder.getDirection());

        printk("demoDuty: %d\r\n", kittyPwmESC0.getDutyCycle());
        printk("Servo: %d\r\n", (int) kittyServo.getDegrees());
        kittyPwmESC0.setDutyCycle(i);

        if(direction) {
            i++;
        } else {
            i--;
        }

        if(i == 100) {
            direction = false;
        } else if(i == 0) {
            direction = true;
        }

        k_msleep(100);
    }
}


K_THREAD_DEFINE(blinkyThread_id, 1024, NXP_Blinky::blinkyThreadWrapper, &kittyBlinky,
                NULL, NULL, 1, 0, 0);

// K_THREAD_DEFINE(demo_id, 1024, demo, NULL, NULL, NULL,
//                 5, 0, 0);

K_THREAD_DEFINE(cameraThread_id, 2048, NXP_Camera::cameraThreadWrapper, &kittyCamera,
                NULL, NULL, 100, 0, 0);

// K_THREAD_DEFINE(algorithmThread_id, 2048, NXP_Algorithm::algorithmThreadWrapper, &kittyAlgorithm,
//                 NULL, NULL, 3, 0, 0);

// K_THREAD_DEFINE(algorithmDemo_id, ALGORITHM_STACKSIZE, algorithmDemo, NULL,
// NULL,
//                 NULL, ALGORITHM_PRIORITY, 0, 0);
