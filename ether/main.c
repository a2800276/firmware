#include "everykey/everykey.h"
#include "everykey/peripherals/lan8720a.h"
void main(void) {
	dkl_init();
	write_pin(LED1_PIN, true);
	ethernet_on();
	write_pin(LED1_PIN, false);

	systick_start(SYSTICK_10MS);

	//BCR = 1000
	//BSR = 7809
	//PHYID1 = 0007h. Meets spec
	//PHYID2 = c0f1: Model f Rev 1. OUI 110000b. Meets spec

	//OUI =  0003(1xxxb)
	while (1) {
		uint16_t val = eth_mii_read(LAN8720A_REG_BSR);
		bool up = (val & LAN8720A_BSR_LS) ? true : false;
		write_pin(LED2_PIN, true);
		delay(DELAY_MS_UNITS * (up ? 400 : 100));
		write_pin(LED2_PIN, false);
		delay(DELAY_MS_UNITS * (up ? 100 : 400));
/*
		int i;
		for (i=0; i<16; i++) {
			bool on = (val & (0x8000 >> i)) ? true : false;
			write_pin(LED2_PIN, true);
			delay(DELAY_MS_UNITS * (on ? 400 : 100));
			write_pin(LED2_PIN, false);
			delay(DELAY_MS_UNITS * (on ? 100 : 400));
		}
		delay(DELAY_MS_UNITS * 1000);
*/
	}
}

void systick() {
	static uint32_t counter = 0;
	counter++;
//	write_pin(LED1_PIN,counter & 0x08);
}
