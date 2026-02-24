/*
 * api.c
 *
 *  Created on: Feb 23, 2026
 *      Author: vlsiuser
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "api.h"
#include "core_logic/wrapper_fns.h"





funcSpec funcTable[] = {
	{afeSpiRawWrite, FUNC_ID_AFE_SPI_RAW_WRITE, genWrapperFn, 3, 1, {DT_UINT8, DT_UINT16, DT_UINT8}, {DT_UINT32}},
	{afeSpiRawRead, FUNC_ID_AFE_SPI_RAW_READ, readWrapperFn, 2, 1, {DT_UINT8, DT_UINT16}, {DT_UINT32}},
	{afeSpiBurstWrite, FUNC_ID_AFE_SPI_BURST_WRITE, genWrapperFn, 4, 1, {DT_UINT8, DT_UINT16, DT_UINT8_P, DT_UINT16}, {DT_UINT32}},
	{afeSpiBurstRead, FUNC_ID_AFE_SPI_BURST_READ, readWrapperFn, 3, 1, {DT_UINT8, DT_UINT16, DT_UINT16}, {DT_UINT32}},
	{afeSpiRawWriteMulti, FUNC_ID_AFE_SPI_RAW_WRITE_MULTI, genWrapperFn, 3, 1, {DT_UINT8, DT_UINT16, DT_UINT8}, {DT_UINT32}},
	{afeSpiRawReadMulti, 	FUNC_ID_AFE_SPI_RAW_READ_MULTI, readWrapperFn,   2, 1, {DT_UINT8, DT_UINT16}, {DT_UINT32}},
	{afeSpiBurstWriteMulti,  FUNC_ID_AFE_SPI_BURST_WRITE_MULTI, genWrapperFn, 4, 1, {DT_UINT8, DT_UINT16, DT_UINT8_P, DT_UINT16}, {DT_UINT32}},
};

uint32_t afeSpiRawWrite(void** args){
    uint8_t afeInst = *(uint8_t*)args[0];
    uint16_t addr = *(uint16_t*)args[1];
    uint8_t data = *(uint8_t*)args[2];
    printf("afeSpiRawWrite called:\n");
	printf("  afeInst (uint8)   = %02X\n", afeInst);
	printf("  addr (uint16)   = %04X\n", addr);
	printf("  data (uint8)  = %02X\n", data);
	return TI_AFE_RET_EXEC_PASS;
}

uint32_t afeSpiRawRead(void** args){
    uint8_t afeInst = *(uint8_t*)args[0];
    uint16_t addr = *(uint16_t*)args[1];
    uint8_t *readVal;
    readVal = *(uint8_t**)args[2];
    *readVal = 192;
    printf("afeSpiRawRead called:\n");
	printf("  afeInst (uint8)   = %02X\n", afeInst);
	printf("  addr (uint16)   = %04X\n", addr);
    printf("  readVal points to the addr= %p\n", readVal);
    printf("  value in readVal = %02X\n", *readVal);
	return TI_AFE_RET_EXEC_PASS;
}

uint32_t afeSpiBurstWrite(void** args)
{
    uint8_t afeInst = *(uint8_t*)args[0];
    uint16_t addr = *(uint16_t*)args[1];
    uint8_t *data = (uint8_t*)args[2];
    uint16_t dataArraySize = *(uint16_t*)args[3];
    printf("afeSpiBurstWrite called:\n");
	printf("  afeInst (uint8)   = %02X\n", afeInst);
	printf("  addr (uint16)   = %04X\n", addr);
	printf("  dataArraySize (uint16)  = %u\n", dataArraySize);
    printf("  data (uint8[]): \n");
    for (int i = 0; i < dataArraySize; i++) {
   	 printf("%02X ", data[i]);
    }
    printf("\n");
	return TI_AFE_RET_EXEC_PASS;
}

uint32_t afeSpiBurstRead(void** args)
{
    uint8_t afeInst = *(uint8_t*)args[0];
    uint16_t addr = *(uint16_t*)args[1];
    uint16_t dataArraySize = *(uint16_t*)args[2];
    uint8_t **data;
    data = (uint8_t**)args[3];
    *data = malloc(dataArraySize);
    for (int i=0; i<dataArraySize; i++){
   	 (*data)[i] = rand()%256;
    }
	printf("afeSpiBurstRead called:\n");
	printf("  afeInst (uint8)   = %02X\n", afeInst);
	printf("  addr (uint16)   = %04X\n", addr);
	printf("  dataArraySize (uint16)  = %u\n", dataArraySize);
    printf("  Burst Read Data\n");
    for (int i = 0; i<dataArraySize; i++){
   	 printf("%02X ", (*data)[i]);
    }
    printf("\n");
    return TI_AFE_RET_EXEC_PASS;
}

uint32_t afeSpiRawWriteMulti(void** args)
{
    uint8_t afeInstSel = *(uint8_t*)args[0];
    uint16_t addr = *(uint16_t*)args[1];
    uint8_t data = *(uint8_t*)args[2];
    printf("afeSpiRawWriteMulti called:\n");
	printf("  afeInstSel (uint8)   = %02X\n", afeInstSel);
	printf("  addr (uint16)   = %04X\n", addr);
	printf("  data (uint8)  = %02X\n", data);
	return TI_AFE_RET_EXEC_PASS;
}

uint32_t afeSpiRawReadMulti(void** args)
{
    uint8_t afeInstSel = *(uint8_t*)args[0];
    uint16_t addr = *(uint16_t*)args[1];
    uint8_t *readVal;
    readVal = *(uint8_t**)args[2];
    printf("afeSpiRawReadMulti called:\n");
	printf("  afeInstSel (uint8)   = %02X\n", afeInstSel);
	printf("  addr (uint16)   = %04X\n", addr);
    printf("  readVal points to the addr= %p\n", (void*)readVal);

	return TI_AFE_RET_EXEC_PASS;
}

uint32_t afeSpiBurstWriteMulti(void** args)
{
    uint8_t afeInstSel = *(uint8_t*)args[0];
    uint16_t addr = *(uint16_t*)args[1];
    uint8_t *data = (uint8_t*)args[2];
    uint16_t dataArraySize = *(uint16_t*)args[3];
	printf("afeSpiBurstWriteMulti called:\n");
	printf("  afeInstSel (uint8)   = %02X\n", afeInstSel);
	printf("  addr (uint16)   = %04X\n", addr);
	printf("  dataArraySize (uint16)  = %u\n", dataArraySize);
    printf("  data (uint8[]): \n");
    for (int i = 0; i < dataArraySize; i++) {
   	 printf("%02X ", data[i]);
    }
    printf("\n");
	return TI_AFE_RET_EXEC_PASS;
}
