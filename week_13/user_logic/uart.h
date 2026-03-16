#ifndef UART_H
#define UART_H

#include "xuartlite.h"
extern XUartLite Uart;
void uart_getline(char *buffer, int max_len);

#endif