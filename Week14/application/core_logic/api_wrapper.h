/**
 * @file api_wrapper.h
 * @author PSG_TI_TEAM
 * @brief API wrapper definitions and memory-mapped struct layouts.
 *
 * @details
 * This file defines the core data structures used to bridge the gap between 
 * the software parser and the physical hardware drivers. It uses packed 
 * structs to ensure strict byte alignment when overlaying C-structures onto 
 * the physical AXI shared memory space (HW_OPERAND_BASE and HW_RESULT_BASE).
 */

#ifndef API_WRAPPER_H
#define API_WRAPPER_H

#include "xil_types.h"
#include <stdint.h>

/** @brief The total number of SPI devices supported on the bus */
#define NUM_SPI     8

/** @brief The maximum payload size (in bytes) allowed for a single burst transaction */
#define MAX_BURST_SIZE 64

/** @brief Function pointer signature for all API wrapper functions */
typedef u16 (*api_func_ptr)(volatile u8 *operands);

/**
 * @brief Hardware execution opcodes mapped to specific driver functions.
 */
typedef enum {
    OPCODE_SPI_RAW_WRITE         = 0,
    OPCODE_SPI_RAW_READ          = 1,
    OPCODE_SPI_BURST_WRITE       = 2,
    OPCODE_SPI_BURST_READ        = 3,
    OPCODE_SPI_RAW_WRITE_MULTI   = 4,
    OPCODE_SPI_RAW_READ_MULTI    = 5,
    OPCODE_SPI_BURST_WRITE_MULTI = 6,
    API_TABLE_SIZE
} opcode_t;

/* =========================================================================
 * COMMAND ARGUMENT STRUCTS (Host -> Firmware via HW_OPERAND_BASE)
 * ========================================================================= */

/** @brief Memory layout for a raw single-byte write command. */
typedef struct __attribute__((packed)) {
    uint8_t  afeInst;
    uint16_t addr;
    uint8_t  data;
} Cmd_spiRawWrite_Args_t;

/** @brief Memory layout for a raw single-byte read command. */
typedef struct __attribute__((packed)) {
    uint8_t  afeInst;
    uint16_t addr;
} Cmd_spiRawRead_Args_t;

/** @brief Memory layout for a burst write command. */
typedef struct __attribute__((packed)) {
    uint8_t  afeInst;
    uint16_t addr;
    uint16_t dataArraySize;         /**< Number of data bytes that follow in the operand buffer */
} Cmd_spiBurstWrite_Args_t;

/** @brief Memory layout for a burst read command. */
typedef struct __attribute__((packed)) {
    uint8_t  afeInst;
    uint16_t addr;
    uint16_t dataArraySize;         /**< Number of bytes requested to read */
} Cmd_spiBurstRead_Args_t;

/** @brief Memory layout for a multi-device broadcast write command. */
typedef struct __attribute__((packed)) {
    uint8_t  afeInstSel;            /**< Bitmask: bit N = SPI device N selected */
    uint16_t addr;
    uint8_t  data;
} Cmd_spiRawWriteMulti_Args_t;

/** @brief Memory layout for a multi-device read command. */
typedef struct __attribute__((packed)) {
    uint8_t  afeInstSel;            /**< Bitmask: bit N = SPI device N selected */
    uint16_t addr;
} Cmd_spiRawReadMulti_Args_t;

/** @brief Memory layout for a multi-device burst write command. */
typedef struct __attribute__((packed)) {
    uint8_t  afeInstSel;            /**< Bitmask: bit N = SPI device N selected */
    uint16_t addr;
    uint16_t dataArraySize;         /**< Number of data bytes that follow in the operand buffer */
} Cmd_spiBurstWriteMulti_Args_t;

/* =========================================================================
 * RESULT REGISTER STRUCTS (Firmware -> Host via HW_RESULT_BASE)
 * ========================================================================= */

/** * @brief Return layout for OPCODE_SPI_RAW_READ.
 * @details Offset 0 contains the single byte read from the target. 
 */
typedef struct __attribute__((packed)) {
    uint8_t readVal;
} Result_spiRawRead_t;

/** * @brief Return layout for OPCODE_SPI_RAW_READ_MULTI.
 * @details 

 */
typedef struct __attribute__((packed)) {
    uint8_t readVal[NUM_SPI];
} Result_spiRawReadMulti_t;

/** * @brief Return layout for OPCODE_SPI_BURST_READ.
 * @details 
 * - Offset 0 (dataArraySize): The number of valid data bytes retrieved.
 * - Offset 2 (data): The sequential payload of the retrieved bytes.
 */
typedef struct __attribute__((packed)) {
    uint16_t dataArraySize;
    uint8_t  data[MAX_BURST_SIZE];
} Result_spiBurstRead_t;

/* =========================================================================
 * EXTERN DECLARATIONS
 * ========================================================================= */

/** @brief The master lookup table mapping opcodes to their respective wrapper functions */
extern api_func_ptr api_table[API_TABLE_SIZE];

#endif // API_WRAPPER_H
