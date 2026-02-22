#ifndef AXI_REGS_H
#define AXI_REGS_H

#include "xparameters.h"
#include "xil_io.h"
#include "xil_types.h"

/* Replace with your actual macro from xparameters.h if different */
#define REG_BASE_ADDR  XPAR_CUSTOM_REG_0_BASEADDR

/* ---------------- REGISTER MAP ---------------- */

/* Operands: 0x00 – 0x1F (32 bytes = 8 x 32-bit words) */
#define REG_OPERAND(i)   (0x00 + ((i) * 4))

/* Opcode: 1 byte at 0x20 */
#define REG_OPCODE       0x20

/* Command: 1 byte at 0x21 */
#define REG_COMMAND      0x21

/* StatusCode: 2 bytes at 0x22–0x23 */
#define REG_STATUS       0x22

/* Result registers: 0x24 – 0x43 (32 bytes = 8 x 32-bit words) */
#define REG_RESULT(i)    (0x24 + ((i) * 4))

/* ---------------- READ/WRITE MACROS ---------------- */

#define WRITE8(offset, value)  Xil_Out8(REG_BASE_ADDR + (offset), (u8)(value))
#define WRITE16(offset, value) Xil_Out16(REG_BASE_ADDR + (offset), (u16)(value))
#define WRITE32(offset, value) Xil_Out32(REG_BASE_ADDR + (offset), (u32)(value))

#define READ8(offset)  Xil_In8(REG_BASE_ADDR + (offset))
#define READ16(offset) Xil_In16(REG_BASE_ADDR + (offset))
#define READ32(offset) Xil_In32(REG_BASE_ADDR + (offset))

#endif /* AXI_REGS_H */
