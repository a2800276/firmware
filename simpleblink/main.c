#include "everykey/everykey.h"

#define LED P1_17_AS_GPIO_0_12

void main(void) {
	clock_init_default();
	set_strong_output(LED);
	systick_start(SYSTICK_10MS);
}

void systick() {
	static uint32_t counter;	
	counter++;
	write_pin(LED, (counter / 50) & 1);
}

