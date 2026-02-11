#include "uart_driver.h"
#include "xparameters.h"

XUartLite Uart;

void uart_init(void)
{
    XUartLite_Initialize(&Uart, XPAR_UARTLITE_0_DEVICE_ID);
}

char uart_getchar_blocking(void)
{
    char c;
    while (XUartLite_Recv(&Uart, (u8 *)&c, 1) == 0);
    return c;
}

int uart_getline(char *buf, int maxlen)
{
    int i = 0;
    char c;

    while (1) {
        c = uart_getchar_blocking();

        if (c == '\r' || c == '\n') {
            buf[i] = '\0';
            return i;
        }

        if (i < maxlen - 1) {
            buf[i++] = c;
        }
    }
}
