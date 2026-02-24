/*
 * api_types.h
 *
 *  Created on: Feb 23, 2026
 *      Author: vlsiuser
 */

#ifndef API_TYPES_H
#define API_TYPES_H



#include <stdint.h>

#define MAX_ARG_N   31
#define MAX_RET_N   15
typedef enum {
    FUNC_ID_AFE_SPI_RAW_WRITE         = 0,
    FUNC_ID_AFE_SPI_RAW_READ          = 1,
    FUNC_ID_AFE_SPI_BURST_WRITE       = 2,
    FUNC_ID_AFE_SPI_BURST_READ        = 3,
    FUNC_ID_AFE_SPI_RAW_WRITE_MULTI   = 4,
    FUNC_ID_AFE_SPI_RAW_READ_MULTI    = 5,
    FUNC_ID_AFE_SPI_BURST_WRITE_MULTI = 6,
    FUNC_ID_COUNT                     = 7
} funcID;

typedef enum {
    DT_UINT8,
    DT_UINT16,
    DT_UINT32,
    DT_INT32,
    DT_UINT8_P,
    DT_UINT16_P,
    DT_UINT32_P,
    DT_INT32_P,
    DT_CHAR,
    DT_CHAR_P
} DataType;

typedef enum {
    TI_AFE_RET_EXEC_PASS = 0,
    TI_AFE_RET_EXEC_FAIL = 1
} RetType_e;


typedef struct {
    uint8_t  *buf;
    uint32_t  len;
} buffer_t;

typedef struct funcSpec funcSpec;

typedef uint32_t (*PFnApiCall)(void **args);
typedef void     (*PWrapFnCall)(funcSpec spec, void **args, buffer_t *buf);

struct funcSpec {
    PFnApiCall  funcName;
    funcID      ID;
    PWrapFnCall funcWrapper;
    uint8_t     num_args;
    uint8_t     num_rets;
    DataType    arg_types[MAX_ARG_N];
    DataType    ret_types[MAX_RET_N];
};

#endif
