#include "everykey/everykey.h"
#include "animation.h"

void DEBUG(bool on) {	// THIS SHOULD GO LATER ****************
	write_pin(LED2_PIN, on);
}

void main(void) {
	dkl_init();

	write_pin(LED1_PIN, false);
	wifi_on();
	write_pin(LED1_PIN, true);
	
	systick_start(SYSTICK_10MS);

}

void systick() {
	bool wifi_in = cc3000_tick();
	write_pin(LED2_PIN, wifi_in);

}
