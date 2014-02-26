#ifndef _VECTORTABLE_
#define _VECTORTABLE_

#include "types.h"

typedef struct {
	//Common for all Cortex M3/M4 MCUs
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

	//MCU-specific from here. Taken from UM10430 and UM10503.
	void* DAC_HANDLER;              //DAC CONVERTER
	void* MAPP_HANDLER;             //COMMUNICATION TO CORTEX M0 (LPC43XX ONLY)
	void* DMA_HANDLER;              //DMA
	void* RESERVED6;                //UNUSED / RESERVED
	void* FLASH_EEPROM_HANDLER;     //FLASH AND EEPROM
	void* ETHERNET_HANDLER;         //ETHERNET
	void* SDIO_HANDLER;             //SDIO
	void* LCD_HANDLER;              //LCD
	void* USB0_HANDLER;             //USB 0
	void* USB1_HANDLER;             //USB 1
	void* SCT_HANDLER;              //SCT (STATE CONFIGURABLE TIMER) COMBINED INTERRUPT
	void* RITIMER_HANDLER;          //RITIMER
	void* TIMER0_HANDLER;           //TIMER 0
	void* TIMER1_HANDLER;           //TIMER 1
	void* TIMER2_HANDLER;           //TIMER 2
	void* TIMER3_HANDLER;           //TIMER 3
	void* MCPWM_HANDLER;            //MOTOR CONTROL PWM
	void* ADC0_HANDLER;             //ADC 0
	void* I2C0_HANDLER;             //I2C 0
	void* I2C1_HANDLER;             //I2C 1
	void* SPI_HANDLER;              //SPI (LPC43XX ONLY, DON'T CONFUSE WITH SSP0/SSP1)
	void* ADC1_HANDLER;             //ADC 1
	void* SSP0_HANDLER;             //SSP 0
	void* SSP1_HANDLER;             //SSP 1
	void* USART0_HANDLER;           //USART 0
	void* UART1_HANDLER;            //UART 1 (AND MODEM)
	void* USART2_HANDLER;           //USART 2
	void* USART3_HANDLER;           //USART 3 (AND IRDA)
	void* I2S0_HANDLER;             //I2S 0
	void* I2S1_HANDLER;             //I2S 1
	void* SPIFI_HANDLER;            //SPIFI (LPC43XX ONLY)
	void* SGPIO_HANDLER;            //SGPIO (LPC43XX ONLY)
	void* GPIO0_HANDLER;            //GPIO 0 (PIN_INT0)
	void* GPIO1_HANDLER;            //GPIO 1 (PIN_INT1)
	void* GPIO2_HANDLER;            //GPIO 2 (PIN_INT2)
	void* GPIO3_HANDLER;            //GPIO 3 (PIN_INT3)
	void* GPIO4_HANDLER;            //GPIO 4 (PIN_INT4)
	void* GPIO5_HANDLER;            //GPIO 5 (PIN_INT5)
	void* GPIO6_HANDLER;            //GPIO 6 (PIN_INT6)
	void* GPIO7_HANDLER;            //GPIO 7 (PIN_INT7)
	void* GPIOG0_HANDLER;           //GPIO GLOBAL INTERRUPT 0
	void* GPIOG1_HANDLER;           //GPIO GLOBAL INTERRUPT 1
	void* EVENT_ROUTER_HANDLER;     //EVENT ROUTER COMBINED INTERRUPT
	void* CAN1_HANDLER;             //CAN 1
	void* RESERVED7;                //UNUSED / RESERVED
	void* ADCHS_HANDLER;            //ADCHS (LPC43XX ONLY)
	void* ATIMER_HANDLER;           //ALARM TIMER
	void* RTC_HANDLER;              //RTX
	void* RESERVED8;                //UNUSED / RESERVED
	void* WWDT_HANDLER;             //WWDT
	void* M0SUB_HANDLER;            //TXEV FROM CORTEX M0 (LPC43XX ONLY)
	void* CAN0_HANDLER;             //CAN 0
	void* QEI_HANDLER;              //QUADRATURE ENCODER

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
