#ifndef _RGU_
#define _RGU_

#include "../core/types.h"

typedef struct {
	HW_UU unused1 [64];
	HW_WO RESET_CTRL[2];
	HW_UU unused2[2];
	HW_RW RESET_STATUS[4];
	HW_UU unused3[12];
	HW_RO RESET_ACTIVE_STATUS[2];
	HW_UU unused4[170];
	HW_RW RESET_EXT_STAT[64];
} RGU_STRUCT;

#define RGU ((RGU_STRUCT*)0x40053000)

typedef enum RGU_RESET_INDEX {
	CORE_RST = 0,
	PERIPH_RST = 1,
	MASTER_RST = 2,
	WWDT_RST = 4,
	CREG_RST = 5,
	BUS_RST = 8,
	SCU_RST = 9,
	M3_RST = 13,
	LCD_RST = 16,
	USB0_RST = 17,
	USB1_RST = 18,
	DMA_RST = 19,
	SDIO_RST = 20,
	EMC_RST = 21,
	ETHERNET_RST = 22,
	FLASHA_RST = 25,
	EEPROM_RST = 27,
	GPIO_RST = 28,
	FLASHB_RST = 29,
	TIMER0_RST = 32,
	TIMER1_RST = 33,
	TIMER2_RST = 34,
	TIMER3_RST = 35,
	RITIMER_RST = 36,
	SCT_RST = 37,
	MOTOCONPWM_RST = 38,
	QEI_RST = 39,
	ADC0_RST = 40,
	ADC1_RST = 41,
	DAC_RST = 42,
	UART0_RST = 44,
	UART1_RST = 45,
	UART2_RST = 46,
	UART3_RST = 47,
	I2C0_RST = 48,
	I2C1_RST = 49,
	SSP0_RST = 50,
	SSP1_RST = 51,
	I2S_RST = 52,
	SPIFI_RST = 53,
	CAN1_RST = 54,
	CAN0_RST = 55
} RGU_RESET_INDEX;

#ifdef __cplusplus
extern "C" {
#endif

/** trigger a reset in a given domain. May return before reset is complete. */
void rgu_trigger_reset(RGU_RESET_INDEX idx);

/** check whether a reset in a given domain is running. Useful for
blocking until a reset is done. */
bool rgu_reset_active(RGU_RESET_INDEX idx);

#ifdef __cplusplus
}
#endif

#endif
