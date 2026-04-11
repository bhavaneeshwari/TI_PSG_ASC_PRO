/**
 * @file uart.c
 * @author PSG_TI_TEAM
 * @brief Hardware abstraction layer for UART communication.
 *
 * @details
 * Implements basic serial port initialization and a blocking string 
 * read function. Includes VT100 terminal handling for interactive 
 * command-line use.
 */

#include "uart.h"
#include "xil_printf.h"

extern char inbyte(void);

XUartLite Uart;

/**
 * @brief Initializes the physical AXI UART Lite IP block.
 */
void uart_init(void)
{
    XUartLite_Initialize(&Uart, XPAR_UARTLITE_0_DEVICE_ID);
}

/**
 * @brief Reads a full line of text from the UART terminal into a buffer.
 *
 * @details
 * This function blocks execution until a carriage return ('\r') or newline 
 * ('\n') is received. It includes built-in terminal handling for backspace 
 * ('\b') and delete (0x7F) characters, allowing users to correct typing 
 * mistakes live in the console without corrupting the parser buffer.
 *
 * @param buffer Pointer to the character array where the string will be stored.
 * @param max_len The maximum number of characters the buffer can hold (to prevent buffer overflow).
 */
void uart_getline(char *buffer, int max_len) 
{
    int count = 0;
    char c;
    
    while (1) {
        c = inbyte(); // Block and wait for a single character
        
        /* Handle Enter/Return (End of string) */
        if (c == '\r' || c == '\n') {
            buffer[count] = '\0'; // Null-terminate the string safely
            xil_printf("\r\n");  
            break;
        } 
        /* Handle Backspace or Delete key presses */
        else if (c == '\b' || c == 0x7F) {
            if (count > 0) {
                count--;
                xil_printf("\b \b"); // Visually erase the character from the user's terminal
            }
        } 
        /* Handle standard character input safely within buffer limits */
        else if (count < max_len - 1) {
            buffer[count++] = c;
        }
    }
}
