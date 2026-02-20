# func_reg_32.h

 -> include files \
 -xparameters.h -> for the base address of the reg\
 -xil_io.h -> for the Xil_In32 and Xil_Out32 \
 -xil_types.h -> for the u32\
  ->macros defined for the 32 reg offset \
  ->function to write and read the register\
  ->using xil_in32 and xil_out32 function 

   
# uart_driver.h

   -> include files \
         -xuartlite.h 
           -> typedef struct XUartlite  -> a variable is allocated **XUartLite Uart;**
            and pointer of this variable for callinf functions \
           ->  initializing , pointer for varaible   **XUartLite_Initialize(&Uart, XPAR_UARTLITE_0_DEVICE_ID);**
           ->   **func (XUartLite_Recv(&Uart, (u8 *)&c, 1) == 0);**
           -> 
           -> 
           -> 
           -> 

           
        
           
