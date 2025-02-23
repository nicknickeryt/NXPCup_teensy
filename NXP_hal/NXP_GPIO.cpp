#include "NXP_GPIO.hpp"

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

NXP_GPIO::NXP_GPIO(const struct device* port, uint8_t pin)
    : port(port), pin(pin) {}

NXP_GPIO::NXP_GPIO(const struct device* port, uint8_t pin, gpio_flags_t flags)
    : port(port), pin(pin) {
    configure(flags);
}

NXP_GPIO::NXP_GPIO(const struct device* port, uint8_t pin, gpio_flags_t flags,
                   gpio_callback_handler_t isr_handler)
    : port(port), pin(pin) {
    configure(flags);
    configureInterrupt(flags, isr_handler);
}

int NXP_GPIO::configure(gpio_flags_t flags) {
    if (!device_is_ready(port)) {
        printk("GPIO device not ready\n");
        return -1;
    }
    return gpio_pin_configure(port, pin, flags);
}

int NXP_GPIO::set() { return gpio_pin_set_raw(port, pin, 1); }

int NXP_GPIO::reset() { return gpio_pin_set_raw(port, pin, 0); }

int NXP_GPIO::toggle() { return gpio_pin_toggle(port, pin); }

int NXP_GPIO::configureInterrupt(gpio_flags_t flags,
                                 gpio_callback_handler_t isr_handler) {
    gpio_init_callback(&gpio_cb, isr_handler, BIT(pin));
    gpio_add_callback(port, &gpio_cb);

    int ret = gpio_pin_interrupt_configure(port, pin, flags);
    if (ret < 0) {
        printk("Failed to configure GPIO interrupt for pin %d! (Error: %d)\n",
               pin, ret);
    }
    return ret;
}
