#ifndef _VECTORTABLE_
#define _VECTORTABLE_

/** TODO: This was taken from the LPC1343. The 18xx probably has different entries in the end. */
#include "types.h"

typedef struct {
	void* STACK_ENTRY;              //Initial Stack pointer
	void* BOOT_HANDLER;             //Reset handler
	void* NMI_HANDLER;              //NMI handler
	void* HARDFAULT_HANDLER;        //Hard fault handler
	void* MPUFAULT_HANDLER;         //Memory protection unit fault handler
	void* BUSFAULT_HANDLER;         //Bus fault handler
	void* USAGEFAULT_HANDLER;       //Usage fault handler
	void* RESERVED1;
	void* RESERVED2;
	void* RESERVED3;                //Reserved for CRC checksum
	void* RESERVED4;
	void* SVCALL_HANDLER;           //SVCall handler
	void* DEBUGMON_HANDLER;        	//Debug monitor handler
	void* RESERVED5;
	void* PENDSV_HANDLER;           //PendSV handler
	void* SYSTICK_HANDLER;          //The SysTick handler
	void* PIO0_0_WAKEUP_HANDLER;    //PIO0_0  Wakeup
	void* PIO0_1_WAKEUP_HANDLER;    //PIO0_1  Wakeup
	void* PIO0_2_WAKEUP_HANDLER;    //PIO0_2  Wakeup
	void* PIO0_3_WAKEUP_HANDLER;    //PIO0_3  Wakeup
	void* PIO0_4_WAKEUP_HANDLER;    //PIO0_4  Wakeup
	void* PIO0_5_WAKEUP_HANDLER;    //PIO0_5  Wakeup
	void* PIO0_6_WAKEUP_HANDLER;    //PIO0_6  Wakeup
	void* PIO0_7_WAKEUP_HANDLER;    //PIO0_7  Wakeup
	void* PIO0_8_WAKEUP_HANDLER;    //PIO0_8  Wakeup
	void* PIO0_9_WAKEUP_HANDLER;    //PIO0_9  Wakeup
	void* PIO0_10_WAKEUP_HANDLER;   //PIO0_10  Wakeup
	void* PIO0_11_WAKEUP_HANDLER;   //PIO0_11  Wakeup
	void* PIO1_0_WAKEUP_HANDLER;    //PIO1_0  Wakeup
	void* PIO1_1_WAKEUP_HANDLER;    //PIO1_1  Wakeup
	void* PIO1_2_WAKEUP_HANDLER;    //PIO1_2  Wakeup
	void* PIO1_3_WAKEUP_HANDLER;    //PIO1_3  Wakeup
	void* PIO1_4_WAKEUP_HANDLER;    //PIO1_4  Wakeup
	void* PIO1_5_WAKEUP_HANDLER;    //PIO1_5  Wakeup
	void* PIO1_6_WAKEUP_HANDLER;    //PIO1_6  Wakeup
	void* PIO1_7_WAKEUP_HANDLER;    //PIO1_7  Wakeup
	void* PIO1_8_WAKEUP_HANDLER;    //PIO1_8  Wakeup
	void* PIO1_9_WAKEUP_HANDLER;    //PIO1_9  Wakeup
	void* PIO1_10_WAKEUP_HANDLER;   //PIO1_10  Wakeup
	void* PIO1_11_WAKEUP_HANDLER;   //PIO1_11  Wakeup
	void* PIO2_0_WAKEUP_HANDLER;    //PIO2_0  Wakeup
	void* PIO2_1_WAKEUP_HANDLER;    //PIO2_1  Wakeup
	void* PIO2_2_WAKEUP_HANDLER;    //PIO2_2  Wakeup
	void* PIO2_3_WAKEUP_HANDLER;    //PIO2_3  Wakeup
	void* PIO2_4_WAKEUP_HANDLER;    //PIO2_4  Wakeup
	void* PIO2_5_WAKEUP_HANDLER;    //PIO2_5  Wakeup
	void* PIO2_6_WAKEUP_HANDLER;    //PIO2_6  Wakeup
	void* PIO2_7_WAKEUP_HANDLER;    //PIO2_7  Wakeup
	void* PIO2_8_WAKEUP_HANDLER;    //PIO2_8  Wakeup
	void* PIO2_9_WAKEUP_HANDLER;    //PIO2_9  Wakeup
	void* PIO2_10_WAKEUP_HANDLER;   //PIO2_10  Wakeup
	void* PIO2_11_WAKEUP_HANDLER;   //PIO2_11  Wakeup
	void* PIO3_0_WAKEUP_HANDLER;    //PIO3_0  Wakeup
	void* PIO3_1_WAKEUP_HANDLER;    //PIO3_1  Wakeup
	void* PIO3_2_WAKEUP_HANDLER;    //PIO3_2  Wakeup
	void* PIO3_3_WAKEUP_HANDLER;    //PIO3_3  Wakeup
	void* I2C_HANDLER;              //I2C
	void* TIMER16_0_HANDLER;        //16-bit Timer 0 handler
	void* TIMER16_1_HANDLER;        //16-bit Timer 1 handler
	void* TIMER32_0_HANDLER;        //32-bit Timer 0 handler
	void* TIMER32_1_HANDLER;        //32-bit Timer 1 handler
	void* SSP_HANDLER;              //SSP
	void* UART_HANDLER;             //UART
	void* USB_IRQ_HANDLER;          //USB IRQ
	void* USB_FIQ_HANDLER;          //USB FIQ
	void* ADC_HANDLER;              //ADC
	void* WDT_HANDLER;              //WDT
	void* BOD_HANDLER;              //BOD
	void* FLASH_HANDLER;            //Flash
	void* PIOINT3_HANDLER;          //PIO INT3
	void* PIOINT2_HANDLER;          //PIO INT2
	void* PIOINT1_HANDLER;          //PIO INT1
	void* PIOINT0_HANDLER;          //PIO INT0
} VECTOR_TABLE;

/* ---------------------------------
   --- System Control Block (SCB ---
   ---------------------------------
 
 Low level interrupt, exception and handler configuration, overall system control.
 This information is not taken from the LPC datasheet, but from ARM's info center.
 It is identical for most (all?) Cortex M3s. */

typedef struct SCB_STRUCT {
	HW_RO CPUID;	//CPUID base register
	HW_RW ICSR;		//Interrupt Control and State Register
	HW_RW VTOR;		//Vector Table Offset register
	HW_RW AIRCR;	//Application Interrupt and Reset Control Register
	HW_RW SCR;		//System Control Register
	HW_RW CCR;		//Configuration and Control Register
	HW_RW SHPR1;	//System Handler Priority Register 1
	HW_RW SHPR2;	//System Handler Priority Register 2
	HW_RW SHPR3;	//System Handler Priority Register 3
	HW_RW SHCRS;	//System Handler Control and State Register
	HW_RW CFSR;		//Configurable Fault Status Register
	HW_RW MMSR;		//MemManage Fault Status Register
} SCB_STRUCT;

typedef enum AIRCR_BITS {
	AIRCR_VECTKEY = (0xFA05 << 16),
	AIRCR_ENDIAN_MASK = 1 << 15,
	AIRCR_LITTLE_ENDIAN = 0,
	AIRCR_BIG_ENDIAN = 1,
	AIRCR_PRIGROUP_MASK = 0x7 << 8,
	AIRCR_PRIGROUP_BASE = 1 << 8,
	AIRCR_SYSRESETREQ = 1 << 2
} AIRCR_BITS;

#define SCB ((SCB_STRUCT*)0xe00ed00)

/** SCB_ACTLR (Auxiliary Control Register)
 is off the other block, so it's addressed separately */
#define SCB_ACTLR ((HW_RW*)0xe00e008)





#endif
