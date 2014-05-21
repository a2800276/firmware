/*********************************************************************
 Nested vectored interrupt controller and System Control Block (SCB)
**********************************************************************/

#ifndef _NVIC_
#define _NVIC_

#include "../core/types.h"

/* -----------------------------------
   --- NVIC --------------------------
   -----------------------------------

The Nested Vector Interrupt Controller controls interrupts. Note that we use a different base address (0xe000e1000 instead of 0xe000e000) because the first 256 bytes are not used. */

typedef struct {
	HW_UU unused0[64];
	HW_RW ISER0;	//Interrupt set enabled
	HW_RW ISER1;
	HW_UU unused1[30];
	HW_RW ICER0;	//Interrupt clear enabled
	HW_RW ICER1;
	HW_UU unused2[30];
	HW_RW ISPR0;	//Interrupt set pending
	HW_RW ISPR1;
	HW_UU unused3[30];
	HW_RW ICPR0;	//Interrupt clear pending
	HW_RW ICPR1;
	HW_UU unused4[30];
	HW_RO IABR0;	//Interrupt active
	HW_RO IABR1;
	HW_UU unused5[62];
	HW_RW IPR0;		//Interrupt priority
	HW_RW IPR1;
	HW_RW IPR2;
	HW_RW IPR3;
	HW_RW IPR4;
	HW_RW IPR5;
	HW_RW IPR6;
	HW_RW IPR7;
	HW_UU unused6[696];
	HW_WO STIR;	//Software trigger interrupt register
} NVIC_STRUCT;

/* due to the number of interrupts, it doesn't make much sense
to write out all masks explicitly - they are split over multiple registers anyway.
Instead, they are just numbered. The respective register and bit mask can be
obtained by comparison and bit shifting. */

typedef enum NVIC_INTERRUPT_INDEX {
	NVIC_DAC = 0,
	NVIC_DMA = 2,
	NVIC_FLASHEEPROM = 4,
	NVIC_ETHERNET = 5,
	NVIC_SDIO = 6,
	NVIC_LCD = 7,
	NVIC_USB0 = 8,
	NVIC_USB1 = 9,
	NVIC_SCT = 10,
	NVIC_RITIMER = 11,
	NVIC_TIMER0 = 12,
	NVIC_TIMER1 = 13,
	NVIC_TIMER2 = 14,
	NVIC_TIMER3 = 15,
	NVIC_MCPWM = 16,
	NVIC_ADC0 = 17,
	NVIC_I2C0 = 18,
	NVIC_I2C1 = 19,
	NVIC_ADC1 = 21,
	NVIC_SSP0 = 22,
	NVIC_SSP1 = 23,
	NVIC_USART0 = 24,
	NVIC_UART1 = 25,
	NVIC_USART2 = 26,
	NVIC_USART3 = 27,
	NVIC_I2S0 = 28,
	NVIC_I2S1 = 29,
	NVIC_PIN_INT0 = 32,
	NVIC_PIN_INT1 = 33,
	NVIC_PIN_INT2 = 34,
	NVIC_PIN_INT3 = 35,
	NVIC_PIN_INT4 = 36,
	NVIC_PIN_INT5 = 37,
	NVIC_PIN_INT6 = 38,
	NVIC_PIN_INT7 = 39,
	NVIC_GINT0 = 40,
	NVIC_GINT1 = 41,
	NVIC_EVENT_ROUTER = 42,
	NVIC_C_CAN1 = 43,
	NVIC_ATIMER = 46,
	NVIC_RTC = 47,
	NVIC_WWDT = 49,
	NVIC_C_CAN0 = 51,
	NVIC_QEI = 52
} NVIC_INTERRUPT_INDEX;

#define NVIC ((NVIC_STRUCT*)0xe000e000)

/* ----------------------------------
   --- System Control Block (SCB) ---
   ----------------------------------

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

#ifdef __cplusplus
extern "C" {
#endif

/** enables a given interrupt. Note this only applies to the NVIC, enabling a specific interrupt might require additional setup in other peripherals.
	@param interrupt index of interrupt to enable
*/
void nvic_enable_interrupt(NVIC_INTERRUPT_INDEX interrupt);

/** disables a given interrupt.
	@param interrupt index of interrupt to disable
*/
void nvic_disable_interrupt(NVIC_INTERRUPT_INDEX interrupt);

/** returns whether a given interrupt is currently enabled
	@param interrupt index of interrupt to query
	@return true if the interrupt is enabled, false otherwise
*/
bool nvic_is_interrupt_enabled(NVIC_INTERRUPT_INDEX interrupt);

/** marks a given interrupt as pending
	@param interrupt index of interrupt to set pending
*/
void nvic_set_interrupt_pending(NVIC_INTERRUPT_INDEX interrupt);

/** unmarks a given interrupt as pending
	@param interrupt index of interrupt to clear pending
*/
void nvic_clear_interrupt_pending(NVIC_INTERRUPT_INDEX interrupt);

/** returns whether a given interrupt is pending
	@param interrupt index of interrupt to query
	@return true if the interrupt is pending, false otherwise
*/
bool nvic_is_interrupt_pending(NVIC_INTERRUPT_INDEX interrupt);

/** returns whether a given interrupt is currently active
	@param interrupt index of interrupt to query
	@return true if the interrupt is active, false otherwise
*/
bool nvic_is_interrupt_active(NVIC_INTERRUPT_INDEX interrupt);

/** sets the priority of a given interrupt. Note that the values aren't taken exactly - only the top bits are actually stored and used. LPC1343 user manual states that there are 32 interrupt priority levels (which would result in steps of 8). However, the IPRx registers only state 3 bits of priority storage (which would result in steps of 32). Need to sort this out.
	@param interrupt index of interrupt to modify
	@param prio the new interrupt priority (0..255, 0 highest)
*/
void nvic_set_interrupt_priority(NVIC_INTERRUPT_INDEX interrupt, uint8_t prio);

/** returns the current priority of a given interrupt. See NVIC_SetInterruptPriority remarks.
	@param interrupt index of interrupt to query
	@return the interrupt priority (0..255, 0 highest)
*/
uint8_t nvic_get_interrupt_priority(NVIC_INTERRUPT_INDEX interrupt);

/** triggers a specified interrupt
	@param interrupt index of interrupt to modify
*/
void nvic_trigger_interrupt(NVIC_INTERRUPT_INDEX interrupt);


/* The following functions are not really NVIC, but SCB. Anyway, since it's related to interrupts,
 we put it here so it's easier to find. */

/** returns the number of bits within interrupt priorities used for interrupt group priority. Interrupt priorities are separated into group priority and subpriority (priority within the group). Interrupts priority is still chosen among the whole priority field, but preemption only happens among priority groups. In other words: Interrupts with the same priority groups do not interrupt each other. Note that not all processors support all priority bits. In this case, the lowest bits are ignored.
	@return number of bits used for interrupt group priority (0..7). */
uint8_t nvic_get_interrupt_group_priority_bits();

/** sets the number of bits within interrupt priorities used for interrupt group priority. See NVIC_GetInterruptGroupPriorityBits.
 @param groupLength number of bits to be used for interrupt group priority (0..7). */
void nvic_set_interrupt_group_priority_bits(uint8_t groupLength);

/** requests a system reset */
void nvic_reset_system();

#ifdef __cplusplus
}
#endif


#endif
