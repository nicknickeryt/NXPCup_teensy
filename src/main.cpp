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

#define AMBER_LED_GPIO_PORT GPIO_PORT_2
#define AMBER_LED_GPIO_PIN GPIO_PIN_3

NXP_GPIO kittyAmberLed(AMBER_LED_GPIO_PORT, AMBER_LED_GPIO_PIN,
                       GPIO_OUTPUT_ACTIVE);

NXP_GPIO kittyEncoderPinA(GPIO_PORT_1, GPIO_PIN_17);
NXP_GPIO kittyEncoderPinB(GPIO_PORT_1, GPIO_PIN_16);

NXP_Blinky kittyBlinky(kittyAmberLed);
NXP_USB kittyLogUSB;

NXP_ADC kittyCameraAdc(ADC_CHANNEL_0);
NXP_GPIO kittyCameraClkPin(CAMERA_CLK_GPIO_PORT, CAMERA_CLK_GPIO_PIN);
NXP_GPIO kittyCameraSiPin(CAMERA_SI_GPIO_PORT, CAMERA_SI_GPIO_PIN);

NXP_PWM kittyCameraSiPwm(PWM_CHANNEL_CAMERA_SI);

NXP_UART kittyUart(UART_BT);

// TODO proper camera port please :3
NXP_Camera kittyCamera(kittyCameraAdc, kittyCameraClkPin, kittyCameraSiPin,
                       kittyCameraSiPwm, kittyUart);

NXP_PWM kittyPwmESC0(PWM_CHANNEL_ESC0);
NXP_PWM kittyPwmESC1(PWM_CHANNEL_ESC1);
NXP_PWM kittyPwmServo(PWM_CHANNEL_SERVO);

NXP_Algorithm kittyAlgorithm(kittyCamera);

NXP_Encoder kittyEncoder(kittyEncoderPinA, kittyEncoderPinB);

int main() {
    kittyBlinky.start();
    kittyLogUSB.start();
    kittyCameraAdc.setup();

    kittyCameraSiPwm.setup();
    kittyCamera.setup();

    kittyPwmESC0.setup();
    kittyPwmESC1.setup();

    kittyPwmServo.setup();
    kittyEncoder.setup();

    // Dummy --------------------------
    kittyPwmESC0.setPulseWidthMs(1);
    kittyPwmESC1.setPulseWidthMs(2);
    kittyPwmServo.setPulseWidthUs(1500);
    // Dummy --------------------------

    printk("Kitty v2 says hello <3\n");
    uint32_t freq = CLOCK_GetCpuClkFreq();
    printk("CPU Freq: %d\n", freq);
    kittyUart.write("Kitty v2 says polling!\n");

}

void periodicLog() {
    while (1) {
        printk("\nENCODER");

        printk("Encoder pulse: %" PRId64 "\r\n", kittyEncoder.getCount());
        printk("Encoder RPM: %u\r\n", kittyEncoder.getRPM());
        printk("Direction: %d\r\n", kittyEncoder.getDirection());
        k_msleep(100);
    }
}

K_THREAD_DEFINE(cameraProc_id, CAMERA_STACKSIZE, periodicLog, NULL, NULL, NULL,
                CAMERA_PROC_PRIORITY, 0, 0);

K_THREAD_DEFINE(camera_thread, 1024, NXP_Camera::cameraThreadWrapper, &kittyCamera,
                NULL, NULL, 5, 0, 0);

// K_THREAD_DEFINE(algorithmDemo_id, ALGORITHM_STACKSIZE, algorithmDemo, NULL,
// NULL,
//                 NULL, ALGORITHM_PRIORITY, 0, 0);
