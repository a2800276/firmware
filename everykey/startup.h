#ifndef _STARTUP_H_
#define _STARTUP_H_

/** first code entry point from reset. Does some low level init, then calls main. */
void bootstrap(void);

/** optional hook for early setup, directly at the start of bootstrapping.
May be implemented by user code, defaults to nothing. */
void earlysetup(void);

/** main code - called after bootstrap is done. Must be impemented by user code. */
void main(void);

/** interrupt handlers. These can be implemented by the application or
by library code. If not implemented, they default to an endless loop.
The default implementation can be found in defaulthandlers.c */

void nmi_handler(void);
void hardfault_handler(void);
void mpufault_handler(void);
void busfault_handler(void);
void usagefault_handler(void);
void svcall_handler(void);
void debugmonitor_handler(void);
void pendsv_handler(void);
void systick(void);
void dac_handler(void);
void mapp_handler(void);
void dma_handler(void);
void flash_eeprom_handler(void);
void ethernet_handler(void);
void sdio_handler(void);
void lcd_handler(void);
void usb0_handler(void);
void usb1_handler(void);
void sct_handler(void);
void ritimer_handler(void);
void timer0_handler(void);
void timer1_handler(void);
void timer2_handler(void);
void timer3_handler(void);
void mcpwm_handler(void);
void adc0_handler(void);
void i2c0_handler(void);
void i2c1_handler(void);
void spi_handler(void);
void adc1_handler(void);
void ssp0_handler(void);
void ssp1_handler(void);
void usart0_handler(void);
void uart1_handler(void);
void usart2_handler(void);
void usart3_handler(void);
void i2s0_handler(void);
void i2s1_handler(void);
void spifi_handler(void);
void sgpio_handler(void);
void gpio0_handler(void);
void gpio1_handler(void);
void gpio2_handler(void);
void gpio3_handler(void);
void gpio4_handler(void);
void gpio5_handler(void);
void gpio6_handler(void);
void gpio7_handler(void);
void gpiog0_handler(void);
void gpiog1_handler(void);
void event_router_handler(void);
void can1_handler(void);
void adchs_handler(void);
void atimer_handler(void);
void rtc_handler(void);
void wwdt_handler(void);
void m0sub_handler(void);
void can0_handler(void);
void qei_handler(void);
void invalid_interrupt_handler(void);	//Used for all unassigned interrupt slots


#endif