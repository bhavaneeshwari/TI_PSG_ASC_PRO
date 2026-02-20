#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include "xuartlite.h"

extern XUartLite Uart;

void uart_init(void);
char uart_getchar_blocking(void);
int uart_getline(char *buf, int maxlen);


// uart_driver.h additions

void uart_sendchar(char c);
void uart_sendstr(const char *str);
void uart_sendline(const char *str);
void uart_sendhex(u32 val);
void uart_senddec(u32 val);

#endif
