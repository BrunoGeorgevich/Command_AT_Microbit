#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stdint.h>

void DriverSetCallback(void (*callback)(uint8_t));
uint8_t DriverOpen();
void DriverWrite(uint8_t data);

#endif // UART_DRIVER_H
