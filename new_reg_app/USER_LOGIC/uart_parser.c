/*
 * uart_parser.c
 *
 *  Created on: Feb 23, 2026
 *      Author: vlsiuser
 */


/*
 * parser.c  (user_logic)
 *
 * Parses a UART command line into register writes then asserts CMD_START.
 *
 * Line format:
 *   "OPCODE:<hex> ARG:<hex> ARG:<hex> ..."
 *
 * Fixed operand layout per funcID (mirrors reg_map.h OP_OFF_* constants):
 *
 *   funcID  | arg order written to operand window
 *   --------+----------------------------------------------
 *   0,4     | [0]:afeInst(u8)  [1]:addr(u16)  [3]:data(u8)
 *   1,5     | [0]:afeInst(u8)  [1]:addr(u16)
 *   2,6     | [0]:afeInst(u8)  [1]:addr(u16)  [3]:size(u16)  [5..]:data[]
 *   3       | [0]:afeInst(u8)  [1]:addr(u16)  [3]:size(u16)
 */

#include <string.h>
#include <stdlib.h>
#include "uart_parser.h"
#include "uart_driver.h"
#include "reg_io.h"
#include "../func_reg_32.h"
#include "../api_types.h"

#define MAX_BURST_BYTES  27U


static int parse_hex(const char *s, uint32_t *out)
{
    uint32_t val = 0;
    if (s == NULL || *s == '\0') return -1;
    while (*s) {
        char c = *s++;
        uint8_t nibble;
        if      (c >= '0' && c <= '9') nibble = (uint8_t)(c - '0');
        else if (c >= 'A' && c <= 'F') nibble = (uint8_t)(c - 'A' + 10);
        else if (c >= 'a' && c <= 'f') nibble = (uint8_t)(c - 'a' + 10);
        else return -1;
        val = (val << 4) | nibble;
    }
    *out = val;
    return 0;
}


static int tokenise(const char *line, char *buf, char *tokens[], int max_tokens)
{
    int count = 0;
    strcpy(buf, line);
    char *p = buf;

    while (*p && count < max_tokens) {

        while (*p == ' ') p++;
        if (*p == '\0') break;
        tokens[count++] = p;

        while (*p && *p != ' ') p++;
        if (*p) *p++ = '\0';
    }
    return count;
}


static const char *strip_arg_prefix(const char *token)
{
    if (strncmp(token, "ARG:", 4) == 0) return token + 4;
    return token;
}


int parser_handle_line(const char *line)
{
    /* Max tokens: "OPCODE:xx" + up to 3 ARGs + 27 burst bytes */
    char  buf[128];
    char *tokens[32];
    int   ntok;

    if (line == NULL || strlen(line) == 0) return -1;
    if (strlen(line) >= sizeof(buf)) {
        uart_sendline("ERR: line too long");
        return -1;
    }

    ntok = tokenise(line, buf, tokens, 32);
    if (ntok < 1) {
        uart_sendline("ERR: empty line");
        return -1;
    }

    if (strncmp(tokens[0], "OPCODE:", 7) != 0) {
        uart_sendline("ERR: missing OPCODE token");
        return -1;
    }
    uint32_t opcode_val;
    if (parse_hex(tokens[0] + 7, &opcode_val) != 0 || opcode_val >= FUNC_ID_COUNT) {
        uart_sendline("ERR: invalid OPCODE");
        return -1;
    }
    funcID id = (funcID)opcode_val;


    int tok_idx = 1;
    uint32_t tmp;

    switch (id) {


    case FUNC_ID_AFE_SPI_RAW_WRITE:
    case FUNC_ID_AFE_SPI_RAW_WRITE_MULTI:
        if (ntok < 4) { uart_sendline("ERR: need 3 ARGs"); return -1; }

        if (parse_hex(strip_arg_prefix(tokens[tok_idx++]), &tmp)) goto bad_arg;
        reg_write_operand_u8(OP_OFF_AFE_INST,    (uint8_t)tmp);

        if (parse_hex(strip_arg_prefix(tokens[tok_idx++]), &tmp)) goto bad_arg;
        reg_write_operand_u16(OP_OFF_ADDR,        (uint16_t)tmp);

        if (parse_hex(strip_arg_prefix(tokens[tok_idx++]), &tmp)) goto bad_arg;
        reg_write_operand_u8(OP_OFF_DATA_SCALAR,  (uint8_t)tmp);
        break;


    case FUNC_ID_AFE_SPI_RAW_READ:
    case FUNC_ID_AFE_SPI_RAW_READ_MULTI:
        if (ntok < 3) { uart_sendline("ERR: need 2 ARGs"); return -1; }

        if (parse_hex(strip_arg_prefix(tokens[tok_idx++]), &tmp)) goto bad_arg;
        reg_write_operand_u8(OP_OFF_AFE_INST, (uint8_t)tmp);

        if (parse_hex(strip_arg_prefix(tokens[tok_idx++]), &tmp)) goto bad_arg;
        reg_write_operand_u16(OP_OFF_ADDR,    (uint16_t)tmp);
        break;


    case FUNC_ID_AFE_SPI_BURST_WRITE:
    case FUNC_ID_AFE_SPI_BURST_WRITE_MULTI:
        if (ntok < 4) { uart_sendline("ERR: need at least 3 ARGs + data"); return -1; }

        if (parse_hex(strip_arg_prefix(tokens[tok_idx++]), &tmp)) goto bad_arg;
        reg_write_operand_u8(OP_OFF_AFE_INST, (uint8_t)tmp);

        if (parse_hex(strip_arg_prefix(tokens[tok_idx++]), &tmp)) goto bad_arg;
        reg_write_operand_u16(OP_OFF_ADDR,    (uint16_t)tmp);

        if (parse_hex(strip_arg_prefix(tokens[tok_idx++]), &tmp)) goto bad_arg;
        uint16_t size = (uint16_t)tmp;
        if (size > MAX_BURST_BYTES) { uart_sendline("ERR: burst data too large"); return -1; }
        reg_write_operand_u16(OP_OFF_ARRAY_SIZE, size);


        if ((ntok - tok_idx) < (int)size) {
            uart_sendline("ERR: not enough data bytes");
            return -1;
        }
        for (uint16_t i = 0; i < size; i++) {
            if (parse_hex(strip_arg_prefix(tokens[tok_idx++]), &tmp)) goto bad_arg;
            reg_write_operand_u8((uint8_t)(OP_OFF_DATA_ARRAY + i), (uint8_t)tmp);
        }
        break;


    case FUNC_ID_AFE_SPI_BURST_READ:
        if (ntok < 4) { uart_sendline("ERR: need 3 ARGs"); return -1; }

        if (parse_hex(strip_arg_prefix(tokens[tok_idx++]), &tmp)) goto bad_arg;
        reg_write_operand_u8(OP_OFF_AFE_INST, (uint8_t)tmp);

        if (parse_hex(strip_arg_prefix(tokens[tok_idx++]), &tmp)) goto bad_arg;
        reg_write_operand_u16(OP_OFF_ADDR,    (uint16_t)tmp);

        if (parse_hex(strip_arg_prefix(tokens[tok_idx++]), &tmp)) goto bad_arg;
        reg_write_operand_u16(OP_OFF_ARRAY_SIZE, (uint16_t)tmp);
        break;

    default:
        uart_sendline("ERR: unknown funcID");
        return -1;
    }


    reg_set_opcode((uint8_t)id);
    reg_set_command(CMD_START);

    uart_sendline("OK: command dispatched");
    return 0;

bad_arg:
    uart_sendline("ERR: bad hex argument");
    return -1;
}
