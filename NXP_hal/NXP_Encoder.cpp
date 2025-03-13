#include "NXP_Encoder.hpp"

// Define static members outside the class
NXP_GPIO NXP_Encoder::encoderPinA =
    *(new NXP_GPIO());  // or use a real reference here
NXP_GPIO NXP_Encoder::encoderPinB =
    *(new NXP_GPIO());  // or use a real reference here
int64_t NXP_Encoder::encoderCount = 0;
int NXP_Encoder::direction = 0;
int NXP_Encoder::pulseCount = 0;
uint64_t NXP_Encoder::lastUpdateTime = 0;
uint32_t NXP_Encoder::encoderRPM = 0;

NXP_Encoder::NXP_Encoder(NXP_GPIO& pinA, NXP_GPIO& pinB) {
    encoderPinA = pinA;
    encoderPinB = pinB;
};

void dummyEncoderInterruptHandler(const device* port, gpio_callback* cb,
                                  gpio_port_pins_t pins) {
    printk("dummyEncoderInterruptHandler\r\n");
}

void NXP_Encoder::setup() {
    // Configure encoder pins A and B as input

    encoderPinA.configure(GPIO_INPUT | GPIO_INT_EDGE_BOTH);
    encoderPinB.configure(GPIO_INPUT);

    // Configure interrupt for encoderPinA with the appropriate flags and
    // handler
    int ret = encoderPinA.configureInterrupt(GPIO_INT_EDGE_BOTH,
                                             encoderInterruptHandler);
    if (ret < 0) {
        printk("Failed to configure interrupt for encoderPinA (Error: %d)\n",
               ret);
    }

    lastUpdateTime = k_uptime_get();
}

int64_t NXP_Encoder::getCount() const { return encoderCount; }
void NXP_Encoder::resetCount() { encoderCount = 0; }

int NXP_Encoder::getDirection() const { return direction; }

uint32_t NXP_Encoder::getRPM() {
    return encoderRPM;
}
void NXP_Encoder::encoderInterruptHandler(const device* port, gpio_callback* cb,
                                          gpio_port_pins_t pins) {

    int a = encoderPinA.get();
    int b = encoderPinB.get();

    if (a) {  // Rising edge on A
        if (b) {
            encoderCount--;  // CCW
            direction = -1;
        } else {
            encoderCount++;  // CW
            direction = 1;
        }
    } else {  // Falling edge on A
        if (b) {
            encoderCount++;  // CW
            direction = 1;
        } else {
            encoderCount--;  // CCW
            direction = -1;
        }
    }

    pulseCount++;

    uint64_t now = k_uptime_get();
    uint64_t elapsed = now - lastUpdateTime;

    if (elapsed > 1000) {
        // 40 impulsów na pełny obrót
        uint32_t rpm = (pulseCount * 60) / 40;  // obroty na minutę (RPM)
        pulseCount = 0;  // Zresetuj licznik impulsów
        lastUpdateTime = now;  // Zaktualizuj czas

        encoderRPM = rpm;
    }

}
