
## reg_ip_selftest   
the sdk terminal op 
<img width="590" height="283" alt="image" src="https://github.com/user-attachments/assets/29132cd9-0883-4a77-a760-d703a5909b5c" />

# using xil_in32 and xil_out32 for writing into reg 

#define REG_32IP_mReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
    
#define REG_32IP_mWriteReg(BaseAddress, RegOffset, Data) \
  	Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
### function for selftest
 XStatus REG_32IP_Reg_SelfTest(void * baseaddr_p); 

  REG_32IP_mWriteReg (baseaddr, write_loop_index*4, (write_loop_index+1)*READ_WRITE_MUL_FACTOR);
###  define READ_WRITE_MUL_FACTOR 10 
   
   ### writing data 10 to the baseaddr
   ### 20 to the baseadddr+4
here we can see the memory monitor window 
            <img width="831" height="560" alt="image" src="https://github.com/user-attachments/assets/2d194d4a-7572-43b8-b8ea-400878b80ca6" />


