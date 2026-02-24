/*
 * reg_io.h
 *
 *  Created on: Feb 23, 2026
 *      Author: vlsiuser
 */

#ifndef REG_IO_H_
#define REG_IO_H_
#include <stdint.h>
#include "xil_io.h"
#include "../func_reg_32.h"





static inline void     reg_write8 (uint32_t off, uint8_t  v) { Xil_Out8 (REG_BASE_ADDR + off, v); }
static inline void     reg_write16(uint32_t off, uint16_t v) { Xil_Out16(REG_BASE_ADDR + off, v); }
static inline void     reg_write32(uint32_t off, uint32_t v) { Xil_Out32(REG_BASE_ADDR + off, v); }

static inline uint8_t  reg_read8 (uint32_t off) { return Xil_In8 (REG_BASE_ADDR + off); }
static inline uint16_t reg_read16(uint32_t off) { return Xil_In16(REG_BASE_ADDR + off); }
static inline uint32_t reg_read32(uint32_t off) { return Xil_In32(REG_BASE_ADDR + off); }



static inline void     reg_set_opcode    (uint8_t  id)  { reg_write8 (REG_OPCODE,      id);  }
static inline void     reg_set_command   (uint8_t  cmd) { reg_write8 (REG_COMMAND,      cmd); }
static inline uint8_t  reg_get_command   (void)         { return reg_read8(REG_COMMAND);      }
static inline void     reg_set_status    (uint8_t  s)   { reg_write8 (REG_STATUS_CODE,  s);   }
static inline void     reg_set_result    (uint32_t r)   { reg_write32(REG_RESULT,        r);   }
static inline uint32_t reg_get_result    (void)         { return reg_read32(REG_RESULT);       }


static inline void reg_write_operand_u8(uint8_t off, uint8_t val)
{
    reg_write8(REG_OPERANDS + off, val);
}

static inline void reg_write_operand_u16(uint8_t off, uint16_t val)
{
    reg_write16(REG_OPERANDS + off, val);
}

static inline void reg_write_operand_buf(uint8_t off, const uint8_t *data, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
        reg_write8(REG_OPERANDS + off + i, data[i]);
}

static inline uint8_t  reg_read_operand_u8 (uint8_t off) { return reg_read8 (REG_OPERANDS + off); }
static inline uint16_t reg_read_operand_u16(uint8_t off) { return reg_read16(REG_OPERANDS + off); }

static inline void reg_read_operand_buf(uint8_t off, uint8_t *dst, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
        dst[i] = reg_read8(REG_OPERANDS + off + i);
}


#endif
