#ifndef NXP_BLINKY_HPP
#define NXP_BLINKY_HPP

#include "NXP_GPIO.hpp"

#define BLINKY_PRIORIY 1

// TODO: learn what is this thing and why does it break at runtime
#define BLINKY_STACKSIZE 256

class NXP_Blinky {
   private:
    NXP_GPIO& blinky_gpio;
    struct k_thread blinky_thread;

    void blink();

    // TODO oh this is terrible, but we can't pass a member function to k_thread_create 
    static void thread_entry(void* arg1, void*, void*);

   public:
    NXP_Blinky(NXP_GPIO& gpio);

    void start();
};

#endif  // NXP_BLINKY_HPP