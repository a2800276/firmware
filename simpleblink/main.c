#include "everykey/everykey.h"

void main(void) {
	clock_init_default();
	
	set_digital_output(LED1_PIN);
	set_digital_output(LED2_PIN);
	set_digital_output(LED3_PIN);
	set_digital_input(BUTTON, true, false, false);
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

