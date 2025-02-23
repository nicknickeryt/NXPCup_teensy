#include "usb.h"

#include <zephyr/usb/usb_device.h>

void setupUSB() {
    if (usb_enable(NULL)) return;
}