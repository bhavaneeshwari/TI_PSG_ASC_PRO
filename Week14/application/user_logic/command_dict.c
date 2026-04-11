/**
 * @file command_dict.c
 * @author PSG_TI_TEAM
 * @brief Command dictionary data layer for the UART string parser.
 *
 * @details
 * This file contains the static lookup table used by the `parse_and_store` 
 * function to validate and unpack incoming ASCII commands. It acts as the 
 * master configuration linking human-readable strings to physical AXI opcodes 
 * and their required memory layouts.
 */

#include "command_dict.h"
#include "../core_logic/api_wrapper.h"

/**
 * @brief The master command lookup table.
 * * @details 
 * Each entry maps a UART string (e.g., "spiBurstWrite") to:
 * 1. The hardware execution opcode.
 * 2. The minimum number of required arguments.
 * 3. An array of `arg_type_t` enums dictating how the parser should 
 * cast and pack the hexadecimal string arguments into AXI memory.
 */
const command_meta_t cmd_dict[] = {
    {"spiRawWrite", OPCODE_SPI_RAW_WRITE, 3, {ARG_U8, ARG_U16, ARG_U8, ARG_NONE, ARG_NONE}},
    {"spiRawRead", OPCODE_SPI_RAW_READ, 2, {ARG_U8, ARG_U16, ARG_NONE, ARG_NONE, ARG_NONE}},
    {"spiBurstWrite", OPCODE_SPI_BURST_WRITE, 4, {ARG_U8, ARG_U16, ARG_U16, ARG_ARRAY_U8, ARG_NONE}},
    {"spiBurstRead", OPCODE_SPI_BURST_READ, 3, {ARG_U8, ARG_U16, ARG_U16, ARG_NONE, ARG_NONE}},
    {"spiRawWriteMulti", OPCODE_SPI_RAW_WRITE_MULTI, 3, {ARG_U8, ARG_U16, ARG_U8, ARG_NONE, ARG_NONE}},
    {"spiRawReadMulti", OPCODE_SPI_RAW_READ_MULTI, 2, {ARG_U8, ARG_U16, ARG_NONE, ARG_NONE, ARG_NONE}},
    {"spiBurstWriteMulti", OPCODE_SPI_BURST_WRITE_MULTI, 4, {ARG_U8, ARG_U16, ARG_U16, ARG_ARRAY_U8, ARG_NONE}},
};

/** @brief The total number of valid commands, dynamically calculated at compile time */
const int DICT_SIZE = sizeof(cmd_dict) / sizeof(cmd_dict[0]);
