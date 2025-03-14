#include "NXP_USB.hpp"

void NXP_USB::setup() {
    if (usb_enable(NULL)) return;
}