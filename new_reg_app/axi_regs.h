#ifndef AXI_REGS_H
#define AXI_REGS_H

#include "xparameters.h"
#include "xil_io.h"
#include "xil_types.h"


#define REG_BASE_ADDR  XPAR_REG_32IP_0_SREG_AXI_BASEADDR
#define REG_OPERAND(i)   (0x00 + ((i) * 4))
#define REG_OPCODE       0x20
#define REG_COMMAND      0x21
#define REG_STATUS       0x22
#define REG_RESULT(i)    (0x24 + ((i) * 4))


#define WRITE8(offset, value)  Xil_Out8(REG_BASE_ADDR + (offset), (u8)(value))
#define WRITE16(offset, value) Xil_Out16(REG_BASE_ADDR + (offset), (u16)(value))
#define WRITE32(offset, value) Xil_Out32(REG_BASE_ADDR + (offset), (u32)(value))

#define READ8(offset)  Xil_In8(REG_BASE_ADDR + (offset))
#define READ16(offset) Xil_In16(REG_BASE_ADDR + (offset))
#define READ32(offset) Xil_In32(REG_BASE_ADDR + (offset))

#endif 
