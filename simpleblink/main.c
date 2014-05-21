#include "everykey/everykey.h"

void main(void) {
	sparrow_init();
	systick_start(SYSTICK_10MS);
}

void systick() {
	static uint32_t counter;
	if (read_pin(BUTTON_PIN)) counter++;
	else counter--;

	write_pin(LED1_PIN, (counter / 50) & 1);
	write_pin(LED2_PIN, ((counter + 10) / 50) & 1);
	write_pin(LED3_PIN, ((counter + 20) / 50) & 1);
}
