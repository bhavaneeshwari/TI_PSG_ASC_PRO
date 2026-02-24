#ifndef FUNC_REG_32_H
#define FUNC_REG_32_H

#include "xparameters.h"
#include "xil_io.h"
#include "xil_types.h"
// #include"reg_32ip.h"
#define  REG_BASE_ADDR XPAR_REG_32IP_0_SREG_AXI_BASEADDR
// offset
#define REG_OPERANDS        0x00U   /* 32-byte operand window */
#define REG_OPCODE          0x20U
#define REG_COMMAND         0x21U
#define REG_STATUS_CODE     0x22U
#define REG_RESULT          0x24U


#define CMD_IDLE            0x00U
#define CMD_START           0x01U
#define CMD_INPROGRESS      0x02U

#define STATUS_EXEC_PASS    0x00U
#define STATUS_EXEC_FAIL    0x01U

#define OP_OFF_AFE_INST         0U   /* uint8_t  */
#define OP_OFF_ADDR             1U   /* uint16_t */
#define OP_OFF_DATA_SCALAR      3U   /* uint8_t  (raw write only) */
#define OP_OFF_ARRAY_SIZE       3U   /* uint16_t (burst: number of bytes) */
#define OP_OFF_DATA_ARRAY       5U



#define REG_32IP_myWrite( RegOffset, Data) \
  	Xil_Out32((reg0_addr) + (RegOffset), (u32)(Data))

#define REG_32IP_myRead(RegOffset) \
    Xil_In32((reg0_addr) + (RegOffset))

#define REG_16IP_myWrite( RegOffset, Data) \
  	Xil_Out16((reg0_addr) + (RegOffset), (u16)(Data))

#define REG_16IP_myRead(RegOffset) \
    Xil_In16((reg0_addr) + (RegOffset))


#define REG_8IP_myWrite( RegOffset, Data) \
  	Xil_Out8((reg0_addr) + (RegOffset), (u8)(Data))

#define REG_8IP_myRead(RegOffset) \
    Xil_In8((reg0_addr) + (RegOffset))

#endif
