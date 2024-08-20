# README
The purpose of this repo is to make it used by the commercial Nuclei series processor core.
/************************************Start*************************************
- SPKAE10X start FLOW :
- 1. Hardware Initialization
- 2. Hardware Configuration CPU Program Counter(PC)
- 3. Software Turns Off CPU Counts And CPU Global Interrupts
- 4. Software Configuration static  Pointers And Global Pointers
- 5. Software Copies SECTION .data To RAM Space
- 6. Software Initializes SECTION .bss To RAM Space
- 7. Software Initializes EC SPACE Values 0 To RAM Space
- 8. SPKAE10X Init Flow(ODM Custom)
- 9. SPKAE10X Main Flow(ODM Custom)
- 10. SPKAE10X Exit (while(1))
Start*/
/************************************Init*************************************
- SPKAE10X Init FLOW :
- 1. DoubleBoot (Custom Configuration Double Boot Addr，Function Is Get_DoubleBoot_ADDR() In File CUSTOM_INIT.c)
- 2. Default Config(Custom Configuration All ,Function Is Default_Config() In File CUSTOM_INIT.c)
- 3. Modlue Init (Custom Configuration All ,Function Is Module_init() In File KERNEL_SOC_FUNC.c)
- 4. Interrupt Configuration (Custom Configuration All , Function Is Irqc_init() In File KERNEL_IRQ.c)
- 5. Device Initialization (Custom Configuration All , Function Is Device_init() In File CUSTOM_INIT.c)
- 5. Specific memory space Initialization (Custom Configuration All , Function Is Specific_Mem_init() In File KERNEL_MEMORY.c)
- 7. return start and goto main() function
************************************Init*************************************/
