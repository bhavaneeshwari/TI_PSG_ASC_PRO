/*
 * dispatcher.c  (core_logic)
 *
 * Polls the COMMAND register and dispatches API calls based on OPCODE.
 *
 * Operand deserialization uses the fixed byte-offset layout defined in
 * reg_map.h (OP_OFF_*). Each funcID has a known layout so we read the
 * exact offsets rather than iterating generically.
 *
 * void* args[] is built on the stack with pointers into local typed
 * variables. For burst writes the data slice comes directly from the
 * operand window via a local staging buffer.
 */

#include "demapper.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../user_logic/reg_io.h"
#include "../func_reg_32.h"
#include "../api.h"
#include "wrapper_fns.h"

/* Largest args[] array needed across all funcIDs (burst write: 4 args + 1 out-slot) */
#define MAX_ARGS  5

/*
 * Read the fixed-layout operands for the given funcID, populate typed
 * local storage variables, and point args[] at them.
 *
 * Returns 0 on success, -1 if funcID is unknown or operands are invalid.
 */
static int deserialize_operands(funcID id, void *args[MAX_ARGS],
                                /* out-param storage owned by caller */
                                uint8_t  *u8_store,   /* [4] */
                                uint16_t *u16_store,  /* [2] */
                                uint8_t  *data_buf,   /* [MAX_BURST_BYTES] */
                                buffer_t *out_buf)
{
#define MAX_BURST_BYTES 27U

    switch (id) {

    /* ── afeSpiRawWrite / afeSpiRawWriteMulti ──────────────────── */
    case FUNC_ID_AFE_SPI_RAW_WRITE:
    case FUNC_ID_AFE_SPI_RAW_WRITE_MULTI:
        u8_store[0]  = reg_read_operand_u8 (OP_OFF_AFE_INST);
        u16_store[0] = reg_read_operand_u16(OP_OFF_ADDR);
        u8_store[1]  = reg_read_operand_u8 (OP_OFF_DATA_SCALAR);
        args[0] = &u8_store[0];
        args[1] = &u16_store[0];
        args[2] = &u8_store[1];
        break;

    /* ── afeSpiRawRead / afeSpiRawReadMulti ────────────────────── */
    case FUNC_ID_AFE_SPI_RAW_READ:
    case FUNC_ID_AFE_SPI_RAW_READ_MULTI:
        u8_store[0]  = reg_read_operand_u8 (OP_OFF_AFE_INST);
        u16_store[0] = reg_read_operand_u16(OP_OFF_ADDR);
        /*
         * args[2] is the out-param (uint8_t**).
         * readWrapperFn will fill args[num_args] = &out_buf->buf before calling.
         * We pre-allocate 1 byte for the scalar read result.
         */
        out_buf->len = 1;
        out_buf->buf = NULL;   /* wrapper allocates via &out_buf->buf */
        /* Provide a 1-byte storage area inline */
        u8_store[2]  = 0;
        out_buf->buf = &u8_store[2];
        args[0] = &u8_store[0];
        args[1] = &u16_store[0];
        /* args[2] filled by readWrapperFn */
        break;

    /* ── afeSpiBurstWrite / afeSpiBurstWriteMulti ──────────────── */
    case FUNC_ID_AFE_SPI_BURST_WRITE:
    case FUNC_ID_AFE_SPI_BURST_WRITE_MULTI:
        u8_store[0]  = reg_read_operand_u8 (OP_OFF_AFE_INST);
        u16_store[0] = reg_read_operand_u16(OP_OFF_ADDR);
        u16_store[1] = reg_read_operand_u16(OP_OFF_ARRAY_SIZE);
        if (u16_store[1] > MAX_BURST_BYTES) return -1;
        reg_read_operand_buf(OP_OFF_DATA_ARRAY, data_buf, (uint8_t)u16_store[1]);
        args[0] = &u8_store[0];
        args[1] = &u16_store[0];
        args[2] =  data_buf;       /* uint8_t* — pointer directly into staging buf */
        args[3] = &u16_store[1];
        break;

    /* ── afeSpiBurstRead ────────────────────────────────────────── */
    case FUNC_ID_AFE_SPI_BURST_READ:
        u8_store[0]  = reg_read_operand_u8 (OP_OFF_AFE_INST);
        u16_store[0] = reg_read_operand_u16(OP_OFF_ADDR);
        u16_store[1] = reg_read_operand_u16(OP_OFF_ARRAY_SIZE);
        out_buf->len = u16_store[1];
        out_buf->buf = NULL;       /* API will malloc; readWrapperFn passes &out_buf->buf */
        args[0] = &u8_store[0];
        args[1] = &u16_store[0];
        args[2] = &u16_store[1];
        /* args[3] filled by readWrapperFn */
        break;

    default:
        return -1;
    }

    return 0;
#undef MAX_BURST_BYTES
}

/* ── Main polling function ─────────────────────────────────────────── */
void dispatcher_poll(void)
{
    if (reg_get_command() != CMD_START)
        return;

    /* ── Step 1: Mark in-progress immediately ──────────────────── */
    reg_set_command(CMD_INPROGRESS);

    /* ── Step 2: Read opcode ────────────────────────────────────── */
    uint8_t opcode = reg_read8(REG_OPCODE);
    if (opcode >= FUNC_ID_COUNT) {
        reg_set_status(STATUS_EXEC_FAIL);
        reg_set_command(CMD_IDLE);
        return;
    }
    funcID     id   = (funcID)opcode;
    funcSpec   spec = funcTable[id];

    /* ── Step 3: Deserialize operands ───────────────────────────── */
    void    *args[MAX_ARGS]   = { NULL };
    uint8_t  u8_store[4]      = { 0 };
    uint16_t u16_store[2]     = { 0 };
    uint8_t  data_buf[27]     = { 0 };
    buffer_t out_buf          = { NULL, 0 };

    if (deserialize_operands(id, args, u8_store, u16_store, data_buf, &out_buf) != 0) {
        reg_set_status(STATUS_EXEC_FAIL);
        reg_set_command(CMD_IDLE);
        return;
    }

    /* ── Step 4: Call the wrapper (which calls the API) ─────────── */
    spec.funcWrapper(spec, args, &out_buf);

    /* ── Step 5: Write pass/fail to STATUS_CODE ─────────────────── */
    /*
     * The wrapper has already written the result bytes to RESULT_REG.
     * We derive pass/fail from RESULT_REG: 0 == PASS, non-zero == FAIL.
     * (API functions return TI_AFE_RET_EXEC_PASS=0 / FAIL=1.)
     * For a cleaner design the wrapper could return the RetType — but
     * keeping result and status separate avoids changing the wrapper API.
     */
    uint32_t result = reg_get_result();
    reg_set_status((result == TI_AFE_RET_EXEC_PASS) ? STATUS_EXEC_PASS
                                                     : STATUS_EXEC_FAIL);

    /* Free any malloc'd burst-read buffer */
    if (id == FUNC_ID_AFE_SPI_BURST_READ && out_buf.buf != NULL)
        free(out_buf.buf);

    /* ── Step 6: Return to IDLE ──────────────────────────────────── */
    reg_set_command(CMD_IDLE);
}
