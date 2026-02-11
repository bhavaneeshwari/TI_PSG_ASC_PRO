#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include "xuartlite.h"

extern XUartLite Uart;

void uart_init(void);
char uart_getchar_blocking(void);
int uart_getline(char *buf, int maxlen);

#endif
