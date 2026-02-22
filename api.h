#ifndef API_H
#define API_H

#include "xil_types.h"

/* Status Codes as defined in the TI Spec */
#define TI_AFE_RET_EXEC_PASS 0x0000
#define TI_AFE_RET_EXEC_FAIL 0x0001

typedef u16 (*api_func_ptr)(void);

typedef enum {
    OPCODE_WRITE = 0,
    OPCODE_READ,
    OPCODE_ARRAY_WRITE,
    OPCODE_ARRAY_READ,
    API_TABLE_SIZE
} opcode_t;

extern api_func_ptr api_table[API_TABLE_SIZE];

#endif /* API_H */
