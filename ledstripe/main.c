#include "everykey/everykey.h"

void main(void) {
	sparrow_init();
	set_digital_output(LED1_PIN);
	set_digital_output(LED2_PIN);
	write_pin(LED1_PIN, false);
	write_pin(LED1_PIN, false);
	ledstripe_on();
	systick_start(SYSTICK_10MS*3);
}

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} RGB;

#define NUM_LEDS 30
static RGB rgb[NUM_LEDS];

void systick() {
	static uint32_t counter;
	counter++;

	int i;
	for (i=NUM_LEDS-1; i>0; i--) {
		rgb[i].r = rgb[i-1].r;
		rgb[i].g = rgb[i-1].g;
		rgb[i].b = rgb[i-1].b;
	}
	rgb[0].r = (7*counter) & 0xff;
	rgb[0].g = (11*counter) & 0xff;
	rgb[0].b = (13*counter) & 0xff;

	ledstripe_sendRGB((uint8_t*)rgb,30);

	write_pin(LED1_PIN, counter & 0x10);
}
