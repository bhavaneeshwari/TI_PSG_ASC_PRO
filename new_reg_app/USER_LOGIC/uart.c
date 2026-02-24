#include "uart.h"
#include "xil_printf.h"

/* Standard Xilinx BSP function to receive a single character over UART */
extern char inbyte(void);

XUartLite Uart;

void uart_init(void)
{
    XUartLite_Initialize(&Uart, XPAR_UARTLITE_0_DEVICE_ID);
}
void uart_getline(char *buffer, int max_len) 
{
    int count = 0;
    char c;
    
    while (1) {
        c = inbyte(); 
        
        if (c == '\r' || c == '\n') {
            buffer[count] = '\0'; 
            xil_printf("\r\n");   
            break;
        } 
        else if (c == '\b' || c == 0x7F) {
            if (count > 0) {
                count--;
                xil_printf("\b \b"); 
            }
        } 
        else if (count < max_len - 1) {
            buffer[count++] = c;
            xil_printf("%c", c);     
        }
    }
}
