#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include "adc.h"
#include "algorithm.h"
#include "blink.h"
#include "camera.h"
#include "gpio.h"
#include "led.h"
#include "servo.h"
#include "usb.h"

#define ALGORITHM_STACKSIZE 8192
#define CAMERA_STACKSIZE 8192
#define CAMERA_LOG_STACKSIZE 8192

#define BLINK_STACKSIZE 4096

#define CAMERA_PROC_PRIORITY 100
#define ALGORITHM_PRIORITY 7
#define CAMERA_LOG_PRIORITY 3
#define BLINK_AMBER_PRIORIY 1

int main() {
    setupGPIO();
    setupAmberLed();
    setupUSB();
    setupAdc();
    setupCamera();
    setupServo();

    printk("NXP says hello <3\n");
    uint32_t freq = CLOCK_GetCpuClkFreq();
    printk("CPU Freq: %d\n", freq);
}

K_THREAD_DEFINE(cameraProc_id, CAMERA_STACKSIZE, cameraProc, NULL, NULL,
                NULL, CAMERA_PROC_PRIORITY, 0, 0);

// K_THREAD_DEFINE(algorithmDemo_id, ALGORITHM_STACKSIZE, algorithmDemo, NULL,
// NULL,
//                 NULL, ALGORITHM_PRIORITY, 0, 0);

// K_THREAD_DEFINE(cameraDebugPrintkLoop_id, CAMERA_LOG_STACKSIZE,
// cameraDebugPrintkLoop, NULL, NULL,
//                 NULL, CAMERA_LOG_PRIORITY, 0, 0);

K_THREAD_DEFINE(blinkAmber_id, BLINK_STACKSIZE, blinkAmber, NULL, NULL, NULL,
                BLINK_AMBER_PRIORIY, 0, 0);

// K_THREAD_DEFINE(servoDemoProc_id, STACKSIZE, servoDemoProc, NULL, NULL, NULL,
//                 BLINK_AMBER_PRIORIY, 0, 0);
