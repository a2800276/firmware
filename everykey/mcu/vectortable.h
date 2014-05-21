#ifndef _VECTORTABLE_
#define _VECTORTABLE_

#include "../core/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*vtfunc)();

#ifdef __cplusplus
}
#endif

typedef struct {
	//Common for all Cortex M3/M4 MCUs
	void* STACK_ENTRY;              //Initial Stack pointer
	vtfunc BOOT_HANDLER;             //Reset handler
	vtfunc NMI_HANDLER;              //NMI handler
	vtfunc HARDFAULT_HANDLER;        //Hard fault handler
	vtfunc MPUFAULT_HANDLER;         //Memory protection unit fault handler
	vtfunc BUSFAULT_HANDLER;         //Bus fault handler
	vtfunc USAGEFAULT_HANDLER;       //Usage fault handler
	vtfunc RESERVED1;
	vtfunc RESERVED2;
	vtfunc RESERVED3;                //Reserved for CRC checksum
	vtfunc RESERVED4;
	vtfunc SVCALL_HANDLER;           //SVCall handler
	vtfunc DEBUGMON_HANDLER;        	//Debug monitor handler
	vtfunc RESERVED5;
	vtfunc PENDSV_HANDLER;           //PendSV handler
	vtfunc SYSTICK_HANDLER;          //The SysTick handler

	//MCU-specific from here. Taken from UM10430 and UM10503.
	vtfunc DAC_HANDLER;              //DAC CONVERTER
	vtfunc MAPP_HANDLER;             //COMMUNICATION TO CORTEX M0 (LPC43XX ONLY)
	vtfunc DMA_HANDLER;              //DMA
	vtfunc RESERVED6;                //UNUSED / RESERVED
	vtfunc FLASH_EEPROM_HANDLER;     //FLASH AND EEPROM
	vtfunc ETHERNET_HANDLER;         //ETHERNET
	vtfunc SDIO_HANDLER;             //SDIO
	vtfunc LCD_HANDLER;              //LCD
	vtfunc USB0_HANDLER;             //USB 0
	vtfunc USB1_HANDLER;             //USB 1
	vtfunc SCT_HANDLER;              //SCT (STATE CONFIGURABLE TIMER) COMBINED INTERRUPT
	vtfunc RITIMER_HANDLER;          //RITIMER
	vtfunc TIMER0_HANDLER;           //TIMER 0
	vtfunc TIMER1_HANDLER;           //TIMER 1
	vtfunc TIMER2_HANDLER;           //TIMER 2
	vtfunc TIMER3_HANDLER;           //TIMER 3
	vtfunc MCPWM_HANDLER;            //MOTOR CONTROL PWM
	vtfunc ADC0_HANDLER;             //ADC 0
	vtfunc I2C0_HANDLER;             //I2C 0
	vtfunc I2C1_HANDLER;             //I2C 1
	vtfunc SPI_HANDLER;              //SPI (LPC43XX ONLY, DON'T CONFUSE WITH SSP0/SSP1)
	vtfunc ADC1_HANDLER;             //ADC 1
	vtfunc SSP0_HANDLER;             //SSP 0
	vtfunc SSP1_HANDLER;             //SSP 1
	vtfunc USART0_HANDLER;           //USART 0
	vtfunc UART1_HANDLER;            //UART 1 (AND MODEM)
	vtfunc USART2_HANDLER;           //USART 2
	vtfunc USART3_HANDLER;           //USART 3 (AND IRDA)
	vtfunc I2S0_HANDLER;             //I2S 0
	vtfunc I2S1_HANDLER;             //I2S 1
	vtfunc SPIFI_HANDLER;            //SPIFI (LPC43XX ONLY)
	vtfunc SGPIO_HANDLER;            //SGPIO (LPC43XX ONLY)
	vtfunc GPIO0_HANDLER;            //GPIO 0 (PIN_INT0)
	vtfunc GPIO1_HANDLER;            //GPIO 1 (PIN_INT1)
	vtfunc GPIO2_HANDLER;            //GPIO 2 (PIN_INT2)
	vtfunc GPIO3_HANDLER;            //GPIO 3 (PIN_INT3)
	vtfunc GPIO4_HANDLER;            //GPIO 4 (PIN_INT4)
	vtfunc GPIO5_HANDLER;            //GPIO 5 (PIN_INT5)
	vtfunc GPIO6_HANDLER;            //GPIO 6 (PIN_INT6)
	vtfunc GPIO7_HANDLER;            //GPIO 7 (PIN_INT7)
	vtfunc GPIOG0_HANDLER;           //GPIO GLOBAL INTERRUPT 0
	vtfunc GPIOG1_HANDLER;           //GPIO GLOBAL INTERRUPT 1
	vtfunc EVENT_ROUTER_HANDLER;     //EVENT ROUTER COMBINED INTERRUPT
	vtfunc CAN1_HANDLER;             //CAN 1
	vtfunc RESERVED7;                //UNUSED / RESERVED
	vtfunc ADCHS_HANDLER;            //ADCHS (LPC43XX ONLY)
	vtfunc ATIMER_HANDLER;           //ALARM TIMER
	vtfunc RTC_HANDLER;              //RTX
	vtfunc RESERVED8;                //UNUSED / RESERVED
	vtfunc WWDT_HANDLER;             //WWDT
	vtfunc M0SUB_HANDLER;            //TXEV FROM CORTEX M0 (LPC43XX ONLY)
	vtfunc CAN0_HANDLER;             //CAN 0
	vtfunc QEI_HANDLER;              //QUADRATURE ENCODER

} VECTOR_TABLE;






#endif
