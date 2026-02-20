
#include "xparameters.h"
#include "xil_printf.h"
#include "reg_32ip.h"
#include <stdio.h>
#define addr XPAR_REG_32IP_0_SREG_AXI_BASEADDR
int main()
{  XStatus out;

	out =  REG_32IP_Reg_SelfTest((void *)addr);

    print("Hello World\n\r");
    xil_printf("out %d",out );

    return out ;
}

