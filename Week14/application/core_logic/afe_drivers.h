/**
 * @file afe_drivers.h
 * @author PSG_TI_TEAM
 * @brief Hardware abstraction layer prototypes for TI AFE SPI communication.
 *
 * @details
 * Defines the standard API for interacting with the AFE chips. Supports 
 * single-instance targeting and multi-instance bitmask broadcasting.
 */

#ifndef AFE_DRIVERS_H
#define AFE_DRIVERS_H

#include <stdint.h>

/**
 * @brief Standard execution return codes for the hardware drivers.
 */
typedef enum RET_TYPE
{
    TI_AFE_RET_EXEC_PASS = 0, /**< Hardware execution completed successfully */
    TI_AFE_RET_EXEC_FAIL = 1  /**< Hardware execution failed or timed out */
} RetType_e;

/** @brief The total number of physical SPI devices connected to the bus */
#define NUM_SPI 8

// --- OFFICIAL DRIVER PROTOTYPES ---

/**
 * @brief Writes a single byte to a specific register on a single AFE instance.
 * @param afeInst The ID of the target SPI device.
 * @param addr The 16-bit hardware register address.
 * @param data The 8-bit payload to write.
 * @return RetType_e Execution status.
 */
uint32_t afeSpiRawWrite(uint8_t afeInst, uint16_t addr, uint8_t data);

/**
 * @brief Reads a single byte from a specific register on a single AFE instance.
 * @param afeInst The ID of the target SPI device.
 * @param addr The 16-bit hardware register address.
 * @param readVal Pointer to memory where the result byte will be stored.
 * @return RetType_e Execution status.
 */
uint32_t afeSpiRawRead(uint8_t afeInst, uint16_t addr, uint8_t *readVal);

/**
 * @brief Writes a sequential block of memory to a single AFE instance.
 * @param afeInst The ID of the target SPI device.
 * @param addr The 16-bit starting hardware register address.
 * @param data Pointer to the array of bytes to be written.
 * @param dataArraySize The number of bytes to write.
 * @return RetType_e Execution status.
 */
uint32_t afeSpiBurstWrite(uint8_t afeInst, uint16_t addr, uint8_t *data, uint16_t dataArraySize);

/**
 * @brief Reads a sequential block of memory from a single AFE instance.
 * @param afeInst The ID of the target SPI device.
 * @param addr The 16-bit starting hardware register address.
 * @param dataArraySize The number of bytes requested.
 * @param data Pointer to the array where the retrieved bytes will be stored.
 * @return RetType_e Execution status.
 */
uint32_t afeSpiBurstRead(uint8_t afeInst, uint16_t addr, uint16_t dataArraySize, uint8_t *data);

/**
 * @brief Broadcasts a single byte to multiple AFE instances simultaneously.
 * @param afeInstSel An 8-bit mask where each active bit selects a target SPI device.
 * @param addr The 16-bit hardware register address.
 * @param data The 8-bit payload to write to all selected devices.
 * @return RetType_e Execution status.
 */
uint32_t afeSpiRawWriteMulti(uint8_t afeInstSel, uint16_t addr, uint8_t data);

/**
 * @brief Reads a single byte from multiple AFE instances based on a selection bitmask.
 * * @note Architecture uses Direct Index Mapping. The hardware returns an array of size `NUM_SPI`.
 * If bit `i` is set in `afeInstSel`, `readVal[i]` contains the valid data.
 * If bit `i` is 0, `readVal[i]` is zeroed out to prevent stale data ghosting.
 *
 * @param afeInstSel An 8-bit mask selecting the target SPI devices.
 * @param addr The 16-bit hardware register address.
 * @param readVal Pointer to an array of size `NUM_SPI` to hold the mapped results.
 * @return RetType_e Execution status.
 */
uint32_t afeSpiRawReadMulti(uint8_t afeInstSel, uint16_t addr, uint8_t *readVal);

/**
 * @brief Broadcasts a sequential memory block to multiple AFE instances.
 * @param afeInstSel An 8-bit mask selecting the target SPI devices.
 * @param addr The 16-bit starting hardware register address.
 * @param data Pointer to the array of bytes to be written.
 * @param dataArraySize The number of bytes to write.
 * @return RetType_e Execution status.
 */
uint32_t afeSpiBurstWriteMulti(uint8_t afeInstSel, uint16_t addr, uint8_t *data, uint16_t dataArraySize);

#endif
