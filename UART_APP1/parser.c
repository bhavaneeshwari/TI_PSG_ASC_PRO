#include "parser.h"
#include "axi_regs.h"
#include "api.h"
#include "xil_printf.h"
#include <string.h>

u32 parse_hex(const char *s)
{
    u32 val = 0;

    if (!s)
        return 0;

    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        s += 2;

    while (*s) {
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
    char *tok = strtok(cmd, " ");
    u32 addr, data;
    int i, array_count;

    if (!tok)
        return;

    if (!strcmp(tok, "READ")) {
        addr = parse_hex(strtok(NULL, " "));
        
        xil_printf("\r\n[PARSER] Preparing READ command\r\n");
        xil_printf("[PARSER] Writing to registers:\r\n");
        xil_printf("  REG_ADDRESS (0x%02x) = 0x%08x\r\n", REG_ADDRESS, addr);
        
        REG_ADDRESS_WRITE(addr);
        REG_DATA_WRITE(0);  
        
        
        REG_COMMAND_WRITE(MAKE_COMMAND(FUNC_READ, STATUS_START));
        xil_printf("  REG_COMMAND (0x%02x) = 0x%08x (Func=%d, Status=START)\r\n", 
                   REG_COMMAND, MAKE_COMMAND(FUNC_READ, STATUS_START), FUNC_READ);
    }
    

    else if (!strcmp(tok, "WRITE")) {
        addr = parse_hex(strtok(NULL, " "));
        data = parse_hex(strtok(NULL, " "));
        
        xil_printf("\r\n[PARSER] Preparing WRITE command\r\n");
        xil_printf("[PARSER] Writing to registers:\r\n");
        xil_printf("  REG_ADDRESS (0x%02x) = 0x%08x\r\n", REG_ADDRESS, addr);
        xil_printf("  REG_DATA    (0x%02x) = 0x%08x\r\n", REG_DATA, data);
        
        REG_ADDRESS_WRITE(addr);
        REG_DATA_WRITE(data);
        
        
        REG_COMMAND_WRITE(MAKE_COMMAND(FUNC_WRITE, STATUS_START));
        xil_printf("  REG_COMMAND (0x%02x) = 0x%08x (Func=%d, Status=START)\r\n", 
                   REG_COMMAND, MAKE_COMMAND(FUNC_WRITE, STATUS_START), FUNC_WRITE);
    }
    
  
    else if (!strcmp(tok, "ARRAY_WRITE")) {
        addr = parse_hex(strtok(NULL, " "));
        array_count = parse_hex(strtok(NULL, " "));
        
        if (array_count > MAX_ARRAY_SIZE) {
            xil_printf("[PARSER] Warning: Count %d exceeds max %d, truncating\r\n", 
                       array_count, MAX_ARRAY_SIZE);
            array_count = MAX_ARRAY_SIZE;
        }
        
        xil_printf("\r\n[PARSER] Preparing ARRAY_WRITE command\r\n");
        xil_printf("[PARSER] Writing to registers:\r\n");
        xil_printf("  REG_ADDRESS (0x%02x) = 0x%08x\r\n", REG_ADDRESS, addr);
        xil_printf("  REG_DATA    (0x%02x) = 0x%08x (count)\r\n", REG_DATA, array_count);
        
        REG_ADDRESS_WRITE(addr);
        REG_DATA_WRITE(array_count);
        
        
        for (i = 0; i < array_count; i++) {
            data = parse_hex(strtok(NULL, " "));
            REG_ARRAY_WRITE(i, data);
            xil_printf("  REG_ARRAY_%d (0x%02x) = 0x%08x\r\n", 
                       i, REG_ARRAY_0 + (i * 4), data);
        }
        
  
        REG_COMMAND_WRITE(MAKE_COMMAND(FUNC_ARRAY_WRITE, STATUS_START));
        xil_printf("  REG_COMMAND (0x%02x) = 0x%08x (Func=%d, Status=START)\r\n", 
                   REG_COMMAND, MAKE_COMMAND(FUNC_ARRAY_WRITE, STATUS_START), 
                   FUNC_ARRAY_WRITE);
    }
    
   
    else if (!strcmp(tok, "ARRAY_READ")) {
        addr = parse_hex(strtok(NULL, " "));
        array_count = parse_hex(strtok(NULL, " "));
        
        if (array_count > MAX_ARRAY_SIZE) {
            array_count = MAX_ARRAY_SIZE;
        }
        
        xil_printf("\r\n[PARSER] Preparing ARRAY_READ command\r\n");
        xil_printf("[PARSER] Writing to registers:\r\n");
        xil_printf("  REG_ADDRESS (0x%02x) = 0x%08x\r\n", REG_ADDRESS, addr);
        xil_printf("  REG_DATA    (0x%02x) = 0x%08x (count)\r\n", REG_DATA, array_count);
        
        REG_ADDRESS_WRITE(addr);
        REG_DATA_WRITE(array_count);
        
      
        REG_COMMAND_WRITE(MAKE_COMMAND(FUNC_ARRAY_READ, STATUS_START));
        xil_printf("  REG_COMMAND (0x%02x) = 0x%08x (Func=%d, Status=START)\r\n", 
                   REG_COMMAND, MAKE_COMMAND(FUNC_ARRAY_READ, STATUS_START), 
                   FUNC_ARRAY_READ);
    }
    

    else if (!strcmp(tok, "DUMP")) {
        addr = parse_hex(strtok(NULL, " "));
        data = parse_hex(strtok(NULL, " "));
        
        xil_printf("\r\n[PARSER] Preparing DUMP_MEMORY command\r\n");
        xil_printf("[PARSER] Writing to registers:\r\n");
        xil_printf("  REG_ADDRESS (0x%02x) = 0x%08x\r\n", REG_ADDRESS, addr);
        xil_printf("  REG_DATA    (0x%02x) = 0x%08x (count)\r\n", REG_DATA, data);
        
        REG_ADDRESS_WRITE(addr);
        REG_DATA_WRITE(data);
      
        REG_COMMAND_WRITE(MAKE_COMMAND(FUNC_DUMP_MEMORY, STATUS_START));
        xil_printf("  REG_COMMAND (0x%02x) = 0x%08x (Func=%d, Status=START)\r\n", 
                   REG_COMMAND, MAKE_COMMAND(FUNC_DUMP_MEMORY, STATUS_START), 
                   FUNC_DUMP_MEMORY);
    }
    else {
        xil_printf("[PARSER] Unknown command: %s\r\n", tok);
    }
}
