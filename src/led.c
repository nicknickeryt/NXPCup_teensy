#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#include "led.h"
#include "gpio.h"

void setupAmberLed() {
    setupGPIO();
    gpioConfigurePin(AMBER_LED_GPIO_PORT, AMBER_LED_GPIO_PIN, GPIO_OUTPUT_ACTIVE);
}

void ledAmberSet() {
    gpioSetPin(AMBER_LED_GPIO_PORT, AMBER_LED_GPIO_PIN);
}

void ledAmberReset() {
    gpioResetPin(AMBER_LED_GPIO_PORT, AMBER_LED_GPIO_PIN);
}