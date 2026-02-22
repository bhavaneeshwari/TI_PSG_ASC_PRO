#include <string.h>
#include "parser.h"
#include "axi_regs.h"
#include "api.h"

/* Custom hex parser to handle hex strings (e.g., "0x1A" or "1A") */
static u32 parse_hex(const char *s)
{
    u32 val = 0;

    if (!s) return 0;

    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        s += 2;

    while (*s)
    {
        val <<= 4;

        if (*s >= '0' && *s <= '9')
            val |= (*s - '0');
        else if (*s >= 'A' && *s <= 'F')
            val |= (*s - 'A' + 10);
        else if (*s >= 'a' && *s <= 'f')
            val |= (*s - 'a' + 10);
        else
            break;

        s++;
    }

    return val;
}

void parse_and_store(char *cmd)
{
    /* Block new commands if currently busy */
    if (READ8(REG_COMMAND) != 0) return;

    char *tok = strtok(cmd, " ");
    if (!tok) return;

    if (!strcmp(tok, "WRITE"))
    {
        WRITE8(REG_OPCODE, OPCODE_WRITE);
        char *addr = strtok(NULL, " ");
        char *val = strtok(NULL, " ");
        if (addr) WRITE32(REG_OPERAND(0), parse_hex(addr));
        if (val)  WRITE32(REG_OPERAND(1), parse_hex(val));
    }
    else if (!strcmp(tok, "READ"))
    {
        WRITE8(REG_OPCODE, OPCODE_READ);
        char *addr = strtok(NULL, " ");
        if (addr) WRITE32(REG_OPERAND(0), parse_hex(addr));
    }
    else if (!strcmp(tok, "ARRAY_WRITE"))
    {
        WRITE8(REG_OPCODE, OPCODE_ARRAY_WRITE);
        char *addr = strtok(NULL, " ");
        char *len = strtok(NULL, " ");
        if (addr && len) 
        {
            WRITE32(REG_OPERAND(0), parse_hex(addr));
            u32 length = parse_hex(len);
            
            /* Limit length to 6 to fit in Operands[2..7] */
            if (length > 6) length = 6; 
            WRITE32(REG_OPERAND(1), length);
            
            for (u32 i = 0; i < length; i++) {
                char *data = strtok(NULL, " ");
                if (data) WRITE32(REG_OPERAND(2 + i), parse_hex(data));
                else break;
            }
        }
    }
    else if (!strcmp(tok, "ARRAY_READ"))
    {
        WRITE8(REG_OPCODE, OPCODE_ARRAY_READ);
        char *addr = strtok(NULL, " ");
        char *len = strtok(NULL, " ");
        if (addr) WRITE32(REG_OPERAND(0), parse_hex(addr));
        if (len)  WRITE32(REG_OPERAND(1), parse_hex(len));
    }

    /* Start execution */
    WRITE8(REG_COMMAND, 1);
}
