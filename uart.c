#include "uart.h"
#include "xil_printf.h"

/* Standard Xilinx BSP function to receive a single character over UART */
extern char inbyte(void);

void uart_getline(char *buffer, int max_len) 
{
    int count = 0;
    char c;
    
    while (1) {
        c = inbyte(); // Wait for character from terminal
        
        /* Handle Enter key (Carriage Return or Newline) */
        if (c == '\r' || c == '\n') {
            buffer[count] = '\0'; // Null-terminate the string
            //xil_printf("\r\n");   // Echo newline to terminal
            break;
        } 
        /* Handle Backspace */
        else if (c == '\b' || c == 0x7F) {
            if (count > 0) {
                count--;
                //xil_printf("\b \b"); // Erase character on terminal screen
            }
        } 
        /* Handle regular characters */
        else if (count < max_len - 1) {
            buffer[count++] = c;
            //xil_printf("%c", c);     // Echo character to terminal
        }
    }
}
