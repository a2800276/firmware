/* Default implementations for optional functions */

// -------------------------------
// --- Default implementations ---
// -------------------------------

/* infinite loop. We're only there if something went terribly wrong
(unrecoverable exception or other situations that don't allow error handling).
Wait for debugger to pick up. */

void deadend(void) {
	while (1) {}
}

/* a function that does nothing. Useful for defaulting optional handlers */
void nothing(void) {
}

/** default handler for programming faults */
void standard_progfault(int error) {
	while (1) {}
}

/* The DEFAULTS_TO macro sets a weak alias of a function,
i.e. a default implementation if there's no other "real" implementation */

#define DEFAULTS_TO(func) __attribute__((weak, alias (#func)))

// ----------------------------------------------
// --- functions with default implementations ---
// ----------------------------------------------

/** Optional hook for very early setup. Statics and globals are not initialized yet! */
void earlysetup(void) DEFAULTS_TO(nothing);

/** interrupt handlers */
void nmi_handler(void) DEFAULTS_TO(deadend);
void hardfault_handler(void) DEFAULTS_TO(deadend);
void mpufault_handler(void) DEFAULTS_TO(deadend);
void busfault_handler(void) DEFAULTS_TO(deadend);
void usagefault_handler(void) DEFAULTS_TO(deadend);
void svcall_handler(void) DEFAULTS_TO(deadend);
void debugmonitor_handler(void) DEFAULTS_TO(deadend);
void pendsv_handler(void) DEFAULTS_TO(deadend);
void systick(void) DEFAULTS_TO(deadend);
void dac_handler(void) DEFAULTS_TO(deadend);
void mapp_handler(void) DEFAULTS_TO(deadend);
void dma_handler(void) DEFAULTS_TO(deadend);
void flash_eeprom_handler(void) DEFAULTS_TO(deadend);
void ethernet_handler(void) DEFAULTS_TO(deadend);
void sdio_handler(void) DEFAULTS_TO(deadend);
void lcd_handler(void) DEFAULTS_TO(deadend);
void usb0_handler(void) DEFAULTS_TO(deadend);
void usb1_handler(void) DEFAULTS_TO(deadend);
void sct_handler(void) DEFAULTS_TO(deadend);
void ritimer_handler(void) DEFAULTS_TO(deadend);
void timer0_handler(void) DEFAULTS_TO(deadend);
void timer1_handler(void) DEFAULTS_TO(deadend);
void timer2_handler(void) DEFAULTS_TO(deadend);
void timer3_handler(void) DEFAULTS_TO(deadend);
void mcpwm_handler(void) DEFAULTS_TO(deadend);
void adc0_handler(void) DEFAULTS_TO(deadend);
void i2c0_handler(void) DEFAULTS_TO(deadend);
void i2c1_handler(void) DEFAULTS_TO(deadend);
void spi_handler(void) DEFAULTS_TO(deadend);
void adc1_handler(void) DEFAULTS_TO(deadend);
void ssp0_handler(void) DEFAULTS_TO(deadend);
void ssp1_handler(void) DEFAULTS_TO(deadend);
void usart0_handler(void) DEFAULTS_TO(deadend);
void uart1_handler(void) DEFAULTS_TO(deadend);
void usart2_handler(void) DEFAULTS_TO(deadend);
void usart3_handler(void) DEFAULTS_TO(deadend);
void i2s0_handler(void) DEFAULTS_TO(deadend);
void i2s1_handler(void) DEFAULTS_TO(deadend);
void spifi_handler(void) DEFAULTS_TO(deadend);
void sgpio_handler(void) DEFAULTS_TO(deadend);
void gpio0_handler(void) DEFAULTS_TO(deadend);
void gpio1_handler(void) DEFAULTS_TO(deadend);
void gpio2_handler(void) DEFAULTS_TO(deadend);
void gpio3_handler(void) DEFAULTS_TO(deadend);
void gpio4_handler(void) DEFAULTS_TO(deadend);
void gpio5_handler(void) DEFAULTS_TO(deadend);
void gpio6_handler(void) DEFAULTS_TO(deadend);
void gpio7_handler(void) DEFAULTS_TO(deadend);
void gpiog0_handler(void) DEFAULTS_TO(deadend);
void gpiog1_handler(void) DEFAULTS_TO(deadend);
void event_router_handler(void) DEFAULTS_TO(deadend);
void can1_handler(void) DEFAULTS_TO(deadend);
void adchs_handler(void) DEFAULTS_TO(deadend);
void atimer_handler(void) DEFAULTS_TO(deadend);
void rtc_handler(void) DEFAULTS_TO(deadend);
void wwdt_handler(void) DEFAULTS_TO(deadend);
void m0sub_handler(void) DEFAULTS_TO(deadend);
void can0_handler(void) DEFAULTS_TO(deadend);
void qei_handler(void) DEFAULTS_TO(deadend);
void invalid_interrupt_handler(void) DEFAULTS_TO(deadend);

/** standard programming error handler */
void progfault(int error) DEFAULTS_TO(standard_progfault);
