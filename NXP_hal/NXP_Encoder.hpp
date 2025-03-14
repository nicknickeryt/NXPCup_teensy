#ifndef NXP_ENCODER_HPP
#define NXP_ENCODER_HPP

#include <zephyr/kernel.h>

#include "NXP_GPIO.hpp"

#define ENCODER_GPIO_PORT GPIO_PORT_1
#define ENCODER_A_GPIO_PIN GPIO_PIN_17
#define ENCODER_B_GPIO_PIN GPIO_PIN_16

class NXP_Encoder {
   public:
    NXP_Encoder(NXP_GPIO& encoderPinA, NXP_GPIO& encoderPinB);

    void setup();

    int64_t getCount() const;
    void resetCount();

    int getDirection() const;

    uint32_t getRPM();

   private:
    static NXP_GPIO encoderPinA;
    static NXP_GPIO encoderPinB;
    static int64_t encoderCount;
    static int direction;
    static int pulseCount;
    static uint64_t lastUpdateTime;
    static uint32_t encoderRPM;

    static void encoderInterruptHandler(const device *port, gpio_callback *cb, gpio_port_pins_t pins);
};

#endif  // NXP_ENCODER_HPP
