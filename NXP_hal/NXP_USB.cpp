#include "NXP_USB.hpp"

void NXP_USB::start() {
    if (usb_enable(NULL)) return;
}