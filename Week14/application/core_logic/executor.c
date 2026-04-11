/**
 * @file executor.c
 * @author PSG_TI_TEAM
 * @brief Hardware polling and API execution dispatcher.
 */

#include "executor.h"
#include "../axi_regs.h"
#include "afe_drivers.h" 
#include "api_wrapper.h"

/**
 * @brief Polls the AXI command register and executes the requested API.
 *
 * @details
 * This function acts as the bridge between the software parser and the physical 
 * hardware drivers. It implements a 3-step hardware handshake:
 * - State 1 (Input): Parser signals a command is ready to be executed.
 * - State 2 (Active): Execution is in progress (locks the bus).
 * - State 0 (Idle): Execution is complete and results are ready in memory.
 * * It dynamically routes execution through the generated `api_table` using the opcode.
 */
void executor_poll(void) {
    if (READ_CMD() == 1) {
        WRITE_CMD(2); // Lock the bus (Execution in progress)
        
        u8 opcode = READ_OPCODE();
        u16 status = TI_AFE_RET_EXEC_FAIL; 

        // Safely look up and execute the mapped function from the Python-generated table
        if (opcode < API_TABLE_SIZE && api_table[opcode]) {
            status = api_table[opcode](HW_OPERAND_BASE); 
        }

        WRITE_STATUS(status);
        WRITE_CMD(0); // Release the bus (Execution complete)
    }
}
