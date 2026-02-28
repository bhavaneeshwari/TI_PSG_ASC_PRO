#include "uart.h"
#include "xil_printf.h"

extern char inbyte(void);

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
        }
    }
}
