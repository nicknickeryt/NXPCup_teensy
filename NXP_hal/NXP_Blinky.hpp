#ifndef NXP_BLINKY_HPP
#define NXP_BLINKY_HPP

#include "NXP_GPIO.hpp"

#define BLINKY_PRIORIY 1

// TODO: learn what is this thing and why does it break at runtime
#define BLINKY_STACKSIZE 256

#define BLINKY_GPIO_PORT GPIO_PORT_2
#define BLINKY_GPIO_PIN GPIO_PIN_3

class NXP_Blinky {
   private:
    NXP_GPIO& blinky_gpio;
    struct k_thread blinky_thread;

   public:
    static void blinkyThreadWrapper(void* arg1, void* arg2, void* arg3);

    NXP_Blinky(NXP_GPIO& gpio);

    void proc();
};

#endif  // NXP_BLINKY_HPP