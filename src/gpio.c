#include "gpio.h"

static struct gpio_callback gpio_cb;

void setupGPIO() {
    if (!device_is_ready(GPIO_PORT_6)) {
        printk("GPIO (Port 6) device not ready\n");
        return;
    }
    if (!device_is_ready(GPIO_PORT_2)) {
        printk("GPIO (Port 2) device not ready\n");
        return;
    }
    // TODO more ports...
}

void gpioConfigurePin(const struct device *gpio_ct_dev, uint8_t pin,
                      unsigned int flag) {
    int ret;
    ret = gpio_pin_configure(gpio_ct_dev, pin, flag);

    if (ret != 0) {
        printk("GPIO device error\n");
        return;
    }
}

void gpioSetPin(const struct device *gpio_ct_dev, uint8_t pin) {
    int ret = gpio_pin_set_raw(gpio_ct_dev, pin, 1);

    if (ret != 0) {
        printk("err 0\n");
        return;
    }
}
void gpioResetPin(const struct device *gpio_ct_dev, uint8_t pin) {
    int ret = gpio_pin_set_raw(gpio_ct_dev, pin, 0);

    if (ret != 0) {
        printk("err 0\n");
        return;
    }
}

void gpioConfigureInterrupt(const struct device *port, uint32_t pin,
                            gpio_flags_t flags,
                            gpio_callback_handler_t isr_handler) {
    gpio_init_callback(&gpio_cb, isr_handler, BIT(pin));
    gpio_add_callback(port, &gpio_cb);

    int ret = gpio_pin_interrupt_configure(port, pin, flags);
    if (ret < 0) {
        printk("Failed to configure GPIO interrupt for pin %d! (Error: %d)\n",
               pin, ret);
        return;
    }
}