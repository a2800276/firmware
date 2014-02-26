#include "everykey/everykey.h"


#define LED_PINGROUP 1
#define LED_PINIDX 17
#define LED_FUNC 0
#define LED_GPIOPORT 0
#define LED_GPIOIDX 12


void main(void) {

	scu_set_pin_mode(LED_PINGROUP, LED_PINIDX, LED_FUNC);
	scu_set_pin_drive_strength(LED_PINGROUP, LED_PINIDX, 3);
	gpio_set_pin_dir(LED_GPIOPORT, LED_GPIOIDX, OUTPUT);
	gpio_write_digital_pin(LED_GPIOPORT, LED_GPIOIDX, false);

	clock_set_xtal_core_freq(15, 1);

	systick_start(SYSTICK_10MS);
	return;

}

void systick() {
	static uint32_t counter = 0;	
	counter++;
	gpio_write_digital_pin(LED_GPIOPORT, LED_GPIOIDX, counter & 0x08);

}

