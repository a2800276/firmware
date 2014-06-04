#include "everykey/everykey.h"

void main(void) {
	sparrow_init();

	while (1) {
		delay(5000000);
		write_pin(LED1_PIN, true);
		audio_on();
		audio_play(2, 16, 48000);

		delay(20000000);
		write_pin(LED1_PIN, false);
		audio_off();
	}
//	systick_start(SYSTICK_10MS);
}

void progfault(int err) {
	write_pin(LED2_PIN, true);
	while(1);
}
/*
void systick() {
	static uint32_t counter;
	counter++;
	write_pin(LED1_PIN, (counter / 25) & 1);

	if ((counter % 50) == 0) {
		audio_on();
	}
}
*/
