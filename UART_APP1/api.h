#ifndef API_H
#define API_H

#include "xil_types.h"

typedef void (*api_func_t)(void);  // function pointer

#define API_TABLE_SIZE 6 

extern api_func_t api_table[API_TABLE_SIZE];


#define FUNC_READ         1
#define FUNC_WRITE        2
#define FUNC_ARRAY_WRITE  3
#define FUNC_ARRAY_READ   4
#define FUNC_DUMP_MEMORY  5

void api_read_handler(void);
void api_write_handler(void);
void api_array_write_handler(void);
void api_array_read_handler(void);
void api_dump_memory_handler(void);


void print_register_contents(const char* func_name);
void print_array_registers(int count);

#endif
