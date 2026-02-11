#ifndef AXI_REGS_H
#define AXI_REGS_H

#include "xparameters.h"
#include "xil_io.h"
#include "xil_types.h"


#define CMD_BASE_ADDR   XPAR_MY_REG_IP_0_S00_AXI_BASEADDR


#define REG_COMMAND      0x00   // Command/Status register - MicroBlaze polls this 
#define REG_ADDRESS      0x04   // Address argument 
#define REG_DATA         0x08   // Data argument (single value) 
#define REG_ARRAY_0      0x0C   // Array element 0 
#define REG_ARRAY_1      0x10   // Array element 1 
#define REG_ARRAY_2      0x14   // Array element 2 
#define REG_ARRAY_3      0x18   // Array element 3 
#define REG_ARRAY_4      0x1C   // Array element 4 
#define REG_ARRAY_5      0x20   // Array element 5 
#define REG_ARRAY_6      0x24   // Array element 6 
#define REG_ARRAY_7      0x28   // Array element 7 

#define CMD_FUNC_ID_MASK   0x0000FF00  // Bits 8-15: Function ID 
#define CMD_FUNC_ID_SHIFT  8
#define CMD_STATUS_MASK    0x000000FF  // Bits 0-7: Status 
#define CMD_STATUS_SHIFT   0

#define STATUS_IDLE        0
#define STATUS_START       1
#define STATUS_EXEC        2
#define STATUS_DONE        3

#define REG_WRITE(offset, val) 
    Xil_Out32(CMD_BASE_ADDR + (offset), (u32)(val))

#define REG_READ(offset) 
    Xil_In32(CMD_BASE_ADDR + (offset))

#define REG_COMMAND_WRITE(v)   REG_WRITE(REG_COMMAND, v)
#define REG_COMMAND_READ()     REG_READ(REG_COMMAND)

#define GET_STATUS(cmd_val)    ((cmd_val) & CMD_STATUS_MASK)
#define GET_FUNC_ID(cmd_val)   (((cmd_val) & CMD_FUNC_ID_MASK) >> CMD_FUNC_ID_SHIFT)

#define MAKE_COMMAND(func_id, status) 
    ((((func_id) << CMD_FUNC_ID_SHIFT) & CMD_FUNC_ID_MASK) | 
     (((status) << CMD_STATUS_SHIFT) & CMD_STATUS_MASK))
#define REG_ADDRESS_WRITE(v)   REG_WRITE(REG_ADDRESS, v)
#define REG_ADDRESS_READ()     REG_READ(REG_ADDRESS)

#define REG_DATA_WRITE(v)      REG_WRITE(REG_DATA, v)
#define REG_DATA_READ()        REG_READ(REG_DATA)

#define REG_ARRAY_WRITE(index, v)  REG_WRITE(REG_ARRAY_0 + ((index) * 4), v)
#define REG_ARRAY_READ(index)      REG_READ(REG_ARRAY_0 + ((index) * 4))

#define MAX_ARRAY_SIZE  8

#endif
