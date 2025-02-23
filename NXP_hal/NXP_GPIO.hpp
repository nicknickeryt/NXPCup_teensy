#ifndef NXP_GPIO_HPP
#define NXP_GPIO_HPP

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#include <cstdint>

#define GPIO_PORT_1 DEVICE_DT_GET(DT_NODELABEL(gpio1))
#define GPIO_PORT_2 DEVICE_DT_GET(DT_NODELABEL(gpio2))
#define GPIO_PORT_3 DEVICE_DT_GET(DT_NODELABEL(gpio3))
#define GPIO_PORT_4 DEVICE_DT_GET(DT_NODELABEL(gpio4))
#define GPIO_PORT_5 DEVICE_DT_GET(DT_NODELABEL(gpio5))
#define GPIO_PORT_6 DEVICE_DT_GET(DT_NODELABEL(gpio6))

#define GPIO_PIN_0 0
#define GPIO_PIN_1 1
#define GPIO_PIN_2 2
#define GPIO_PIN_3 3
#define GPIO_PIN_4 4
#define GPIO_PIN_5 5
#define GPIO_PIN_6 6
#define GPIO_PIN_7 7
#define GPIO_PIN_8 8
#define GPIO_PIN_9 9
#define GPIO_PIN_10 10
#define GPIO_PIN_11 11
#define GPIO_PIN_12 12
#define GPIO_PIN_13 13
#define GPIO_PIN_14 14
#define GPIO_PIN_15 15

class NXP_GPIO {
   public:
    NXP_GPIO(const struct device* port, uint8_t pin);
    NXP_GPIO(const struct device* port, uint8_t pin, gpio_flags_t flags);
    NXP_GPIO(const struct device* port, uint8_t pin, gpio_flags_t flags,
             gpio_callback_handler_t isr_handler);

    int configure(gpio_flags_t flags);
    int set();
    int reset();
    int toggle();
    int configureInterrupt(gpio_flags_t flags,
                           gpio_callback_handler_t isr_handler);

   private:
    const struct device* port;
    uint8_t pin;
    struct gpio_callback gpio_cb;
};

#endif  // NXP_GPIO_HPP
