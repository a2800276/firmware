#include "everykey/everykey.h"

void delay(int count) {
	volatile int i; 
	for (i=0; i<count; i++) {} 
}

#define LED_PINGROUP 1
#define LED_PINIDX 17
#define LED_FUNC 0
#define LED_GPIOPORT 0
#define LED_GPIOIDX 12


void main(void) {

	scu_set_pin_mode(LED_PINGROUP, LED_PINIDX, LED_FUNC);
	scu_set_pin_drive_strength(LED_PINGROUP, LED_PINIDX, 3);
	gpio_set_pin_dir(LED_GPIOPORT, LED_GPIOIDX, OUTPUT);

	start_systick_10ms();
	return;

}

void systick() {
	static uint32_t counter = 0;	
	counter++;
	gpio_write_digital_pin(LED_GPIOPORT, LED_GPIOIDX, counter & 0x20);

}

