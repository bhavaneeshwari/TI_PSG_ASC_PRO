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



void uart_sendchar(char c)
{
    while (XUartLite_Send(&Uart, (u8 *)&c, 1) == 0);
}

void uart_sendstr(const char *str)
{
    while (*str)
        uart_sendchar(*str++);
}

void uart_sendline(const char *str)
{
    uart_sendstr(str);
    uart_sendchar('\r');
    uart_sendchar('\n');
}

void uart_sendhex(u32 val)
{
    char buf[11];
    int i;
    buf[0]  = '0';
    buf[1]  = 'x';
    buf[10] = '\0';

    for (i = 9; i >= 2; i--) {
        u8 nibble = val & 0xF;
        buf[i] = (nibble < 10) ? ('0' + nibble)
                               : ('A' + nibble - 10);
        val >>= 4;
    }
    uart_sendstr(buf);
}

void uart_senddec(u32 val)
{
    char buf[11];
    int i = 10;
    buf[10] = '\0';

    if (val == 0) {
        uart_sendchar('0');
        return;
    }
    while (val > 0 && i > 0) {
        buf[--i] = '0' + (val % 10);
        val /= 10;
    }
    uart_sendstr(&buf[i]);
}

