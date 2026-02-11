#include "api.h"
#include "axi_regs.h"
#include "xil_printf.h"

static u32 dummy_mem[256];

void print_register_contents(const char* func_name)
{
    u32 addr = REG_ADDRESS_READ();
    u32 data = REG_DATA_READ();
    
    xil_printf("API Function: %s\r\n", func_name);
  
    xil_printf("REG_ADDRESS (0x%02x) = 0x%08x (%d)\r\n", 
               REG_ADDRESS, addr, addr);
    xil_printf("REG_DATA    (0x%02x) = 0x%08x (%d)\r\n", 
               REG_DATA, data, data);
}

void print_array_registers(int count)
{
    int i;
    xil_printf("\r\nArray Registers:\r\n");
    for (i = 0; i < count && i < MAX_ARRAY_SIZE; i++) {
        u32 val = REG_ARRAY_READ(i);
        xil_printf("  REG_ARRAY_%d (0x%02x) = 0x%08x (%d)\r\n", 
                   i, REG_ARRAY_0 + (i * 4), val, val);
    }
}

void api_read_handler(void)
{
    u32 addr;
    u32 data;

    print_register_contents("READ");
    
    addr = REG_ADDRESS_READ();
    xil_printf("\r\nReading from dummy_mem[0x%02x]\r\n", addr & 0xFF);
    
    data = dummy_mem[addr & 0xFF];
    
    REG_DATA_WRITE(data);
    
    xil_printf("Read value: 0x%08x (%d)\r\n", data, data);
    xil_printf("Result written to REG_DATA (0x%02x)\r\n", REG_DATA);
}

void api_write_handler(void)
{
    u32 addr;
    u32 data;

    print_register_contents("WRITE");
    
    addr = REG_ADDRESS_READ();
    data = REG_DATA_READ();
    
    xil_printf("\r\nWriting 0x%08x to dummy_mem[0x%02x]\r\n", 
               data, addr & 0xFF);
    
    dummy_mem[addr & 0xFF] = data;
    
    xil_printf("Write complete!\r\n");
}

void api_array_write_handler(void)
{
    u32 addr;
    u32 count;
    int i;

    print_register_contents("ARRAY_WRITE");
    
    addr = REG_ADDRESS_READ();
    
    count = REG_DATA_READ();
    if (count > MAX_ARRAY_SIZE) {
        count = MAX_ARRAY_SIZE;
    }
    
    print_array_registers(count);
    
    xil_printf("\r\nWriting %d elements to dummy_mem starting at 0x%02x\r\n", 
               count, addr & 0xFF);
    
    for (i = 0; i < count; i++) {
        u32 array_val = REG_ARRAY_READ(i);
        dummy_mem[(addr + i) & 0xFF] = array_val;
        xil_printf("  dummy_mem[0x%02x] = 0x%08x\r\n", 
                   (addr + i) & 0xFF, array_val);
    }
    
    xil_printf("Array write complete!\r\n");
}

void api_array_read_handler(void)
{
    u32 addr;
    u32 count;
    int i;

    print_register_contents("ARRAY_READ");
    
    addr = REG_ADDRESS_READ();
    
    count = REG_DATA_READ();
    if (count > MAX_ARRAY_SIZE) {
        count = MAX_ARRAY_SIZE;
    }
    
    xil_printf("\r\nReading %d elements from dummy_mem starting at 0x%02x\r\n", 
               count, addr & 0xFF);
    
    for (i = 0; i < count; i++) {
        u32 array_val = dummy_mem[(addr + i) & 0xFF];
        REG_ARRAY_WRITE(i, array_val);
        xil_printf("  dummy_mem[0x%02x] = 0x%08x -> REG_ARRAY_%d\r\n", 
                   (addr + i) & 0xFF, array_val, i);
    }
    
    xil_printf("Array read complete! Data written to array registers.\r\n");
    print_array_registers(count);
}

void api_dump_memory_handler(void)
{
    u32 start_addr;
    u32 count;
    int i;

    print_register_contents("DUMP_MEMORY");
    
    start_addr = REG_ADDRESS_READ();
    count = REG_DATA_READ();
    
    if (count > 32) count = 32; 
    
    xil_printf("\r\nDumping %d locations from dummy_mem[0x%02x]:\r\n", 
               count, start_addr & 0xFF);
    xil_printf("Addr   Value\r\n");
    
    for (i = 0; i < count; i++) {
        u32 addr = (start_addr + i) & 0xFF;
        xil_printf("0x%02x   0x%08x\r\n", addr, dummy_mem[addr]);
    }
    
}

api_func_t api_table[] = {
    0,                        // FUNC_ID = 0 (unused) 
    api_read_handler,         // FUNC_ID = 1 
    api_write_handler,        // FUNC_ID = 2 
    api_array_write_handler,  // FUNC_ID = 3 
    api_array_read_handler,   // FUNC_ID = 4 
    api_dump_memory_handler   // FUNC_ID = 5 
};
