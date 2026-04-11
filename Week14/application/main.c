/**
 * @file main.c
 * @author PSG_TI_TEAM
 * @brief Main execution loop and UART command dispatcher.
 */

#include <stdio.h>
#include <string.h>
#include "xil_printf.h"
#include "axi_regs.h"
#include "interface/uart.h"
#include "user_logic/parser.h"
#include "core_logic/executor.h"

#include "core_logic/afe_drivers.h"
#include "core_logic/api_wrapper.h"

#define CMD_BUFFER_SIZE 128

/**
 * @brief Displays the available command syntax to the UART terminal.
 */
void print_help()
{
    xil_printf("\r\n--- Command Input Formats (Hex) ---\r\n");
    xil_printf("1. Raw Write:   spiRawWrite(Inst, Addr, Data)\r\n");
    xil_printf("2. Raw Read:    spiRawRead(Inst, Addr)\r\n");
    xil_printf("3. Burst Write: spiBurstWrite(Inst, Addr, Size, D0, D1...)\r\n");
    xil_printf("4. Burst Read:  spiBurstRead(Inst, Addr, Size)\r\n");
    xil_printf("5. Multi Write: spiRawWriteMulti(Sel, Addr, Data)\r\n");
    xil_printf("6. Multi Read:  spiRawReadMulti(Sel, Addr)\r\n");
    xil_printf("7. Multi Burst: spiBurstWriteMulti(Sel, Addr, Size, D0, D1...)\r\n");
    xil_printf("-----------------------------------\r\n");
}

/**
 * @brief Main execution loop handling command parsing and hardware polling.
 * @return int Returns 0 on system exit.
 */
int main()
{
    uart_init();
    print_help();

    char cmd_buffer[CMD_BUFFER_SIZE];

    while (1)
    {
        xil_printf("\r\nCMD> ");
        uart_getline(cmd_buffer, CMD_BUFFER_SIZE);

        if (strlen(cmd_buffer) == 0) continue;

        memset((void *)HW_OPERAND_BASE, 0, 0x1f);
        WRITE_OPCODE(0x00);
        WRITE_STATUS(0x00);
        WRITE_STATUS(0x0000);
        memset((void *)HW_RESULT_BASE, 0, 0x1f);
        
        parse_and_store(cmd_buffer);
        executor_poll();

        u16 status = READ_STATUS();
        u8 opcode  = READ_OPCODE();

        if (status == TI_AFE_RET_EXEC_PASS) {
            xil_printf("[MAIN] SUCCESS: Opcode 0x%02X\r\n", opcode);

            if (opcode == OPCODE_SPI_RAW_READ) {
                xil_printf("   -> Result: 0x%02X\r\n", HW_RESULT_BASE[0]);
            }
            else if (opcode == OPCODE_SPI_RAW_READ_MULTI) {
                /*
                 * Read afeInstSel back from operand buffer (offset 0).
                 * Result buffer starts at HW_RESULT_BASE[0] = readVal[0].
                 */
                Result_spiRawReadMulti_t *result = (Result_spiRawReadMulti_t *)HW_RESULT_BASE;

                /* afeInstSel is first byte of operand buffer */
                uint8_t instSel = ((volatile uint8_t *)HW_OPERAND_BASE)[0];

                xil_printf("   -> afeInstSel: 0x%02X\r\n", instSel);
                for (int i = 0; i < NUM_SPI; i++) {
                    if (instSel & (1u << i)) {
                        xil_printf("      SPI[%d]: 0x%02X\r\n", i, result->readVal[i]);
                    }
                }
            }
            else if (opcode == OPCODE_SPI_BURST_READ) {
                 uint8_t  *resultBase  = (uint8_t *)HW_RESULT_BASE;
                 uint16_t  dataArraySize;
                 
                 memcpy(&dataArraySize, resultBase, sizeof(uint16_t));

                 xil_printf("   -> Burst Read (%u bytes):\r\n", dataArraySize);
                 uint8_t *data = resultBase + sizeof(uint16_t);
                 for (uint16_t i = 0; i < dataArraySize; i++) {
                     xil_printf("      [%u]: 0x%02X\r\n", i, data[i]);
                 }
            }
        }
        else if (opcode != 0xFF) {
            xil_printf("[MAIN] ERROR: Failed (0x%04X)\r\n", status);
        }
        else {
            xil_printf("[MAIN] ERROR: Unknown Command Syntax or Args.\r\n");
            print_help();
        }
    }

    return 0;
}
