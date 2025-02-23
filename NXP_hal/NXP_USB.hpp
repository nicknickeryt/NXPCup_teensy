#ifndef NXP_USB_HPP
#define NXP_USB_HPP

#include <zephyr/usb/usb_device.h>

#include "usb.h"

class NXP_USB {
   public:
    NXP_USB() = default;
    void start();
};

#endif  // NXP_USB_HPP