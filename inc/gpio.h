#ifndef __GPIO_H
#define __GPIO_H

#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define GPIO_PORT_6 DEVICE_DT_GET(DT_NODELABEL(gpio6))
#define GPIO_PORT_2 DEVICE_DT_GET(DT_NODELABEL(gpio2))

void setupGPIO();

void gpioConfigurePin(const struct device *gpio_ct_dev, uint8_t pin,
                      unsigned int flag);
void gpioSetPin(const struct device *gpio_ct_dev, uint8_t pin);
void gpioResetPin(const struct device *gpio_ct_dev, uint8_t pin);
void gpioConfigureInterrupt(const struct device *port, uint32_t pin,
                            gpio_flags_t flags,
                            gpio_callback_handler_t isr_handler);

#endif /* __GPIO_H */