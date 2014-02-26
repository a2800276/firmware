#include "everykey/everykey.h"

#define LED P1_17_AS_GPIO_0_12
#define WIFI_SPI_IRQ_PIN P1_9_AS_GPIO_1_2
#define BUTTON P2_7_AS_GPIO_0_7
#define GS_PROG_PIN P3_5_AS_GPIO_1_15

static bool program_mode = false;

void earlysetup(void) {
	set_digital_output(GS_PROG_PIN);
	write_pin(GS_PROG_PIN, true);
}

void main(void) {

	set_strong_output(LED);
//	set_digital_input(WIFI_SPI_IRQ_PIN, false, false, false);
	set_digital_input(BUTTON, false, false, false);
	write_pin(LED, false);

	clock_init_default();

	gs_init();
//	gs_program_firmware();

	systick_start(SYSTICK_10MS);

}


void systick(void) {
	static uint32_t counter = 0;
	counter++;
	if (program_mode) write_pin(LED, counter & 0x08);
	else write_pin(LED, counter & 0x20);

	if ((counter % 100) == 0) {
//		gs_ping_module();
	}
	if ((counter % 100) == 50) {
//		gs_poll();
	}

	if (!read_pin(BUTTON)) {
		program_mode = !program_mode;
		gs_reset(program_mode);
	}
}

