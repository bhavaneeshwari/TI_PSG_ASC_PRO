#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "axi_regs.h"
#include "api_wrapper.h" 
#include "command_dict.h"
#include "afe_drivers.h" 

void parse_and_store(char *input) {
    if (READ_CMD() != 0) return;

    for (int i = 0; i < 128; i++) {
        HW_OPERAND_BASE[i] = 0x00;
    }

    char *cmd_name = strtok(input, "(");
    if (!cmd_name) return;

    const command_meta_t *active_cmd = NULL;
    for (int i = 0; i < DICT_SIZE; i++) {
        if (strcmp(cmd_name, cmd_dict[i].cmd_string) == 0) {
            active_cmd = &cmd_dict[i];
            break;
        }
    }

    if (active_cmd == NULL) {
        WRITE_OPCODE(0xFF);                  
        WRITE_STATUS(TI_AFE_RET_EXEC_FAIL); 
        WRITE_CMD(0);                        
        return;                              
    }

    char *arg_string = strtok(NULL, ")");
    if (!arg_string && active_cmd->min_args > 0) {
        WRITE_OPCODE(0xFF);                  
        WRITE_STATUS(TI_AFE_RET_EXEC_FAIL); 
        WRITE_CMD(0);                        
        return;
    }

    char *token = strtok(arg_string, ",");
    uint32_t hw_offset = 0;
    int arg_idx = 0;
    int parsed_args = 0;

    while (token != NULL && hw_offset < 31) {
        while (*token == ' ') token++;

        arg_type_t current_type;

        if (arg_idx < MAX_ARGS && active_cmd->arg_format[arg_idx] != ARG_NONE) {
            current_type = active_cmd->arg_format[arg_idx];
        } else if (arg_idx > 0 && active_cmd->arg_format[arg_idx - 1] == ARG_ARRAY_U8) {
            current_type = ARG_ARRAY_U8;
        } else {
            WRITE_OPCODE(0xFF);                  
            WRITE_STATUS(TI_AFE_RET_EXEC_FAIL); 
            WRITE_CMD(0);                        
            return; 
        }

        uint32_t val = (uint32_t)strtoul(token, NULL, 16);

        if (current_type == ARG_U8 || current_type == ARG_ARRAY_U8) {
            uint8_t temp8 = (uint8_t)(val & 0xFF);
            memcpy((void *)&HW_OPERAND_BASE[hw_offset], &temp8, 1);
            hw_offset += 1;
        }
        else if (current_type == ARG_U16) {
            uint16_t temp16 = (uint16_t)(val & 0xFFFF);
            memcpy((void *)&HW_OPERAND_BASE[hw_offset], &temp16, 2);
            hw_offset += 2;
        }
        else if (current_type == ARG_U32) {
            memcpy((void *)&HW_OPERAND_BASE[hw_offset], &val, 4);
            hw_offset += 4;
        }

        parsed_args++;
        token = strtok(NULL, ",");
        if (current_type != ARG_ARRAY_U8) arg_idx++;
    }

    if (parsed_args >= active_cmd->min_args) {
        WRITE_OPCODE(active_cmd->opcode);
        WRITE_CMD(1);
    } else {
        WRITE_OPCODE(0xFF);                  
        WRITE_STATUS(TI_AFE_RET_EXEC_FAIL); 
        WRITE_CMD(0); 
    }
}