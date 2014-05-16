#include "everykey/everykey.h"
#include "everykey/progfault.h"

void main(void) {
	sparrow_init();
	systick_start(SYSTICK_10MS);
}

void systick() {
	static uint32_t counter;
	counter++;
	write_pin(LED1_PIN, (counter / 50) & 1);
	if (counter == 100) audio_on();
	if (counter == 200) {
		audio_off();
		counter = 0;
	}

}



