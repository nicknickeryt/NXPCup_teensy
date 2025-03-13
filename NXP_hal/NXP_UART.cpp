#include "NXP_UART.hpp"

#include <string.h>

NXP_UART::NXP_UART(const struct device* uart_dev) : uart_dev(uart_dev) {}

#include <stdio.h> 

void NXP_UART::write(const char* format, ...) {
    char buffer[64];
    va_list args;
    va_start(args, format);
    int msg_len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (msg_len > 0) {
        for (int i = 0; i < msg_len; i++) uart_poll_out(uart_dev, buffer[i]);
    }
}