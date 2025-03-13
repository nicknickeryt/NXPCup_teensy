#ifndef NXP_UART_HPP
#define NXP_UART_HPP

#include <zephyr/drivers/uart.h>

#define UART_BT DEVICE_DT_GET(DT_CHOSEN(zephyr_bt_uart))

class NXP_UART {
   private:
    const struct device* const uart_dev;

   public:
    NXP_UART(const struct device* uart_dev);
    void write(const char* format, ...);
};

#endif /* NXP_UART_HPP */