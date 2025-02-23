#include "NXP_Blinky.hpp"

// This looks really wrong in this place
K_THREAD_STACK_DEFINE(blinky_stack, BLINKY_STACKSIZE);

NXP_Blinky::NXP_Blinky(NXP_GPIO& gpio) : blinky_gpio(gpio) {
    gpio.configure(GPIO_OUTPUT_ACTIVE);
}

void NXP_Blinky::start() {
    k_thread_create(&blinky_thread, blinky_stack, BLINKY_STACKSIZE,
                    &NXP_Blinky::thread_entry, this, NULL, NULL,
                    BLINKY_STACKSIZE, 0, K_NO_WAIT);
}

void NXP_Blinky::thread_entry(void* arg1, void*, void*) {
    NXP_Blinky* self = static_cast<NXP_Blinky*>(arg1);
    self->blink();
}

void NXP_Blinky::blink() {
    while (1) {
        blinky_gpio.set();
        k_msleep(500);
        blinky_gpio.reset();
        k_msleep(500);
    }
}