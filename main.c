#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "xil_printf.h"
#include "parser.h"
#include "executor.h"
#include "axi_regs.h"
#include "uart.h"

#define CMD_BUFFER_SIZE 128

int main()
{
    init_platform(); // Initialize hardware platform

    xil_printf("\r\n=======================================\r\n");
    xil_printf("   TI-PSG API Wrapper Interactive Test \r\n");
    xil_printf("   (All inputs and outputs are in HEX) \r\n");
    xil_printf("=======================================\r\n");
    xil_printf("Available Commands:\r\n");
    xil_printf(" - WRITE <hex_addr> <hex_val>\r\n");
    xil_printf(" - READ <hex_addr>\r\n");
    xil_printf(" - ARRAY_WRITE <hex_addr> <hex_len> <hex_val1> <hex_val2>...\r\n");
    xil_printf(" - ARRAY_READ <hex_addr> <hex_len>\r\n");
    
    char cmd_buffer[CMD_BUFFER_SIZE];

    while (1) 
    {
        xil_printf("\r\nCMD> ");
        
        // 1. Get string from the user via Putty/TeraTerm
        uart_getline(cmd_buffer, CMD_BUFFER_SIZE);
        
        // Skip empty lines
        if (strlen(cmd_buffer) == 0) continue;

        // 2. Parse the string and map to registers
        parse_and_store(cmd_buffer);

        // 3. Kick off execution logic
        executor_poll();

        // 4. Print results based on the operation
        u16 status = READ16(REG_STATUS);
        u8 opcode  = READ8(REG_OPCODE);

        if (status == TI_AFE_RET_EXEC_PASS) {
            xil_printf("SUCCESS (Status: 0x%04X)\r\n", status);
            
            if (opcode == OPCODE_READ) {
                xil_printf(" -> Value Read: 0x%08lX\r\n", READ32(REG_RESULT(0)));
            } 
            else if (opcode == OPCODE_ARRAY_READ) {
                u32 len = READ32(REG_OPERAND(1));
                xil_printf(" -> Array Data Read:\r\n");
                for (u32 i = 0; i < len; i++) {
                    xil_printf("    Result[%lu] = 0x%08lX\r\n", i, READ32(REG_RESULT(i)));
                }
            }
        } else {
            xil_printf("ERROR: Operation Failed (Status: 0x%04X)\r\n", status);
        }
    }

    cleanup_platform();
    return 0;
}
