#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "servo.h"

#include "NXP_GPIO.hpp"
#include "NXP_Blinky.hpp"
#include "NXP_USB.hpp"
#include "NXP_ADC.hpp"
#include "NXP_Camera.hpp"

#define ALGORITHM_STACKSIZE 8192
#define CAMERA_STACKSIZE 8192
#define CAMERA_LOG_STACKSIZE 8192

#define CAMERA_PROC_PRIORITY 100
#define ALGORITHM_PRIORITY 7
#define CAMERA_LOG_PRIORITY 3

#define AMBER_LED_GPIO_PORT GPIO_PORT_2
#define AMBER_LED_GPIO_PIN GPIO_PIN_3

NXP_GPIO kittyAmberLed(AMBER_LED_GPIO_PORT, AMBER_LED_GPIO_PIN, GPIO_OUTPUT_ACTIVE);
NXP_Blinky kittyBlinky(kittyAmberLed);
NXP_USB kittyLogUSB;

NXP_ADC kittyCameraAdc(ADC_CHANNEL_0);
NXP_GPIO kittyCameraClkPin(CAMERA_CLK_GPIO_PORT, CAMERA_CLK_GPIO_PIN);
NXP_GPIO kittyCameraSiPin(CAMERA_SI_GPIO_PORT, CAMERA_SI_GPIO_PIN);

// TODO proper camera port please :3
NXP_Camera kittyCamera(kittyCameraAdc, kittyCameraClkPin, kittyCameraSiPin);

int main() {
    // setupGPIO();
    kittyBlinky.start();
    kittyLogUSB.start();
    kittyCameraAdc.setup();

    kittyCamera.setup();

    kittyCamera.start();

    // setupAdc();
    // setupCamera();
    // setupServo();

    printk("Kitty v2 says hello <3\n");
    uint32_t freq = CLOCK_GetCpuClkFreq();
    printk("CPU Freq: %d\n", freq);
}

// K_THREAD_DEFINE(cameraProc_id, CAMERA_STACKSIZE, cameraProc, NULL, NULL,
//                 NULL, CAMERA_PROC_PRIORITY, 0, 0);

// K_THREAD_DEFINE(algorithmDemo_id, ALGORITHM_STACKSIZE, algorithmDemo, NULL,
// NULL,
//                 NULL, ALGORITHM_PRIORITY, 0, 0);

// K_THREAD_DEFINE(cameraDebugPrintkLoop_id, CAMERA_LOG_STACKSIZE,
// cameraDebugPrintkLoop, NULL, NULL,
//                 NULL, CAMERA_LOG_PRIORITY, 0, 0);
