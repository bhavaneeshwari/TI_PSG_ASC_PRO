#include "api.h"
#include "axi_regs.h"

#define MEM_SIZE 256
static u32 target_memory[MEM_SIZE] = {0};

u16 api_write(void)
{
    u32 addr = READ32(REG_OPERAND(0));
    u32 val  = READ32(REG_OPERAND(1));

    if (addr >= MEM_SIZE) return TI_AFE_RET_EXEC_FAIL;

    target_memory[addr] = val;
    return TI_AFE_RET_EXEC_PASS;
}

u16 api_read(void)
{
    u32 addr = READ32(REG_OPERAND(0));

    if (addr >= MEM_SIZE) return TI_AFE_RET_EXEC_FAIL;

    u32 val = target_memory[addr];
    WRITE32(REG_RESULT(0), val);

    return TI_AFE_RET_EXEC_PASS;
}

u16 api_array_write(void)
{
    u32 addr = READ32(REG_OPERAND(0));
    u32 len  = READ32(REG_OPERAND(1));

    if ((addr + len > MEM_SIZE) || (len > 6)) return TI_AFE_RET_EXEC_FAIL;

    for (u32 i = 0; i < len; i++) {
        target_memory[addr + i] = READ32(REG_OPERAND(2 + i));
    }

    return TI_AFE_RET_EXEC_PASS;
}

u16 api_array_read(void)
{
    u32 addr = READ32(REG_OPERAND(0));
    u32 len  = READ32(REG_OPERAND(1));

    if ((addr + len > MEM_SIZE) || (len > 8)) return TI_AFE_RET_EXEC_FAIL;

    for (u32 i = 0; i < len; i++) {
        WRITE32(REG_RESULT(i), target_memory[addr + i]);
    }

    return TI_AFE_RET_EXEC_PASS;
}

api_func_ptr api_table[API_TABLE_SIZE] = {
    api_write,
    api_read,
    api_array_write,
    api_array_read
};
