#include "NXP_Blinky.hpp"

// This looks really wrong in this place
K_THREAD_STACK_DEFINE(blinky_stack, BLINKY_STACKSIZE);

NXP_Blinky::NXP_Blinky(NXP_GPIO& gpio) : blinky_gpio(gpio) {
    gpio.configure(GPIO_OUTPUT_ACTIVE);
}

void NXP_Blinky::proc() {
    while (1) {
        blinky_gpio.set();
        k_msleep(500);
        blinky_gpio.reset();
        k_msleep(500);
    }
}

void NXP_Blinky::blinkyThreadWrapper(void* arg1, void* arg2, void* arg3) {
    static_cast<NXP_Blinky*>(arg1)->proc();
}
