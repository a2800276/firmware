#include "everykey/everykey.h"


#define LED1_PINGROUP 7
#define LED1_PINIDX 4
#define LED1_FUNC 0
#define LED1_GPIOPORT 3
#define LED1_GPIOIDX 12

#define LED2_PINGROUP 7
#define LED2_PINIDX 5
#define LED2_FUNC 0
#define LED2_GPIOPORT 3
#define LED2_GPIOIDX 13

#define LED3_PINGROUP 7
#define LED3_PINIDX 6
#define LED3_FUNC 0
#define LED3_GPIOPORT 3
#define LED3_GPIOIDX 14



void main(void) {
//	scu_set_pin_drive_strength(LED_PINGROUP, LED_PINIDX, 3);

	scu_set_pin_mode(LED1_PINGROUP, LED1_PINIDX, LED1_FUNC);
	gpio_set_pin_dir(LED1_GPIOPORT, LED1_GPIOIDX, OUTPUT);
	gpio_write_digital_pin(LED1_GPIOPORT, LED1_GPIOIDX, false);

	scu_set_pin_mode(LED2_PINGROUP, LED2_PINIDX, LED2_FUNC);
	gpio_set_pin_dir(LED2_GPIOPORT, LED2_GPIOIDX, OUTPUT);
	gpio_write_digital_pin(LED2_GPIOPORT, LED2_GPIOIDX, false);

	scu_set_pin_mode(LED3_PINGROUP, LED3_PINIDX, LED3_FUNC);
	gpio_set_pin_dir(LED3_GPIOPORT, LED3_GPIOIDX, OUTPUT);
	gpio_write_digital_pin(LED3_GPIOPORT, LED3_GPIOIDX, false);

	clock_set_xtal_core_freq(15, 1);

	systick_start(SYSTICK_10MS);
	return;

}

void systick() {
	static uint32_t counter = 0;	
	counter++;
	gpio_write_digital_pin(LED1_GPIOPORT, LED1_GPIOIDX, counter & 0x20);
	gpio_write_digital_pin(LED2_GPIOPORT, LED2_GPIOIDX, (counter+8) & 0x20);
	gpio_write_digital_pin(LED3_GPIOPORT, LED3_GPIOIDX, (counter+16) & 0x20);

}

