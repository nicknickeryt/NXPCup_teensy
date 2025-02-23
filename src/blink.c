#include "blink.h"

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>

#include "camera.h"
#include "led.h"

void blinkAmber() {
    while (1) {
        ledAmberSet();
        k_msleep(500);
        ledAmberReset();
        k_msleep(500);
    }
}