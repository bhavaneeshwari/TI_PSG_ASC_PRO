#ifndef FUNC_REG_32_H
#define FUNC_REG_32_H

#include "xparameters.h"
#include "xil_io.h"
#include "xil_types.h"
// #include"reg_32ip.h"
#define  reg0_addr XPAR_REG_32IP_0_SREG_AXI_BASEADDR
// offset
#define FUNC_ID_STATUS_REG 0X00
#define DEVICE_ID_REG      0X04
#define ADDRESS_REG        0X08
#define DATA_REG           0X0C
#define BURST_LEN_REG      0X10
#define RESULT_REG         0X14
#define RETURN_REG         0X18
#define ERROR              0X1C
#define BUF_BASE_REG      0X20
//#define RBUF_BASE_REG      0X40

#define FUNC_RAW_WRITE      0x01U
#define FUNC_RAW_READ       0x02U
#define FUNC_BURST_WRITE    0x03U
#define FUNC_BURST_READ     0x04U
#define FUNC_RAW_WR_MULTI   0x05U
#define FUNC_RAW_RD_MULTI   0x06U
#define FUNC_BURST_WR_MULTI 0x07U

#define STATUS_MASK         0x000000FFU   // Byte 0
#define FUNC_ID_MASK        0x0000FF00U   // Byte 1
#define FUNC_ID_SHIFT       8

#define STATUS_IDLE         0x00U
#define STATUS_START        0x01U
#define STATUS_DONE         0x02U
#define STATUS_ERROR        0xFFU

#define REG_32IP_myWrite( RegOffset, Data) \
  	Xil_Out32((reg0_addr) + (RegOffset), (u32)(Data))

#define REG_32IP_myRead(RegOffset) \
    Xil_In32((reg0_addr) + (RegOffset))


#define MAKE_FUNC_ID_STATUS_REG(func_id, status) \
    ((((U32)(func_id) << FUNC_ID_SHIFT) & FUNC_ID_MASK) | \
     ((U32)(status) & STATUS_MASK))

#define GET_STATUS(reg0)    ((reg0) & STATUS_MASK)
#define GET_FUNC_ID(reg0)   (((reg0) & FUNC_ID_MASK) >> FUNC_ID_SHIFT)

#define FUNC_ID_STATUS_REG_WRITE(func_id,status)   REG_32IP_myWrite(FUNC_ID_STATUS_REG,MAKE_FUNC_ID_STATUS_REG(func_id, status) )


#define READ_STATUS() \
    GET_STATUS(REG_32IP_myRead(FUNC_ID_STATUS_REG))

#define READ_FUNC_ID() \
    GET_FUNC_ID(REG_32IP_myRead(FUNC_ID_STATUS_REG))

#define WRITE_DEVICE_ID(id)     REG_32IP_myWrite(DEVICE_ID_REG, (u32)(id))
#define READ_DEVICE_ID()        REG_32IP_myRead(DEVICE_ID_REG)

#define WRITE_ADDRESS(addr)     REG_32IP_myWrite(ADDRESS_REG, (u32)(addr))
#define READ_ADDRESS()          REG_32IP_myRead(ADDRESS_REG)

#define WRITE_DATA(data)        REG_32IP_myWrite(DATA_REG, (u32)(data))
#define READ_DATA()             REG_32IP_myRead(DATA_REG)

#define WRITE_BURST_LEN(len)    REG_32IP_myWrite(BURST_LEN_REG, (u32)(len))
#define READ_BURST_LEN()        REG_32IP_myRead(BURST_LEN_REG)

#define WRITE_RESULT(data)      REG_32IP_myWrite(RESULT_REG, (u32)(data))
#define READ_RESULT()           REG_32IP_myRead(RESULT_REG)

#define WRITE_RETURN(data)      REG_32IP_myWrite(RETURN_REG, (u32)(data))
#define READ_RETURN()           REG_32IP_myRead(RETURN_REG)

#define WRITE_ERROR(data)       REG_32IP_myWrite(ERROR_REG, (u32)(data))
#define READ_ERROR()            REG_32IP_myRead(ERROR_REG)
// for array data buf address is given by adding the offset to reg0_addr


#define WRITE_BUF(index, data) \
    REG_32IP_myWrite(BUF_BASE_REG + ((index) * 4U), data)
#define READ_BUF(index, data) \
   REG_32IP_myRead (BUF_BASE_REG + ((index) * 4U))


/* #define WRITE_BUFF(BaseAddress, RegOffset, Data)
for (int write_loop_index = 0 ; write_loop_index < 4; write_loop_index++)
	  REG_32IP_myWriteReg (baseaddr, write_loop_index*4, );
	for (int read_loop_index = 0 ; read_loop_index < 4; read_loop_index++)
	   REG_32IP_mReadReg (baseaddr, read_loop_index*4) != (read_loop_index+1)*READ_WRITE_MUL_FACTOR);
	   */
