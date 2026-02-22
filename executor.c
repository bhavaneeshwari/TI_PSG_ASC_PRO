#include "executor.h"
#include "axi_regs.h"
#include "api.h"

void executor_poll(void)
{
    if (READ8(REG_COMMAND) == 1)   // START
    {
        WRITE8(REG_COMMAND, 2);   // BUSY

        u8 opcode = READ8(REG_OPCODE);
        u16 status = TI_AFE_RET_EXEC_FAIL; // Default to fail

        if (opcode < API_TABLE_SIZE && api_table[opcode])
        {
            status = api_table[opcode]();
        }

        WRITE16(REG_STATUS, status);
        WRITE8(REG_COMMAND, 0);   // DONE → back to idle
    }
}
