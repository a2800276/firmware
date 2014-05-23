
#include "everykey/everykey.h"
//#include "sdmmc.h"

uint8_t status = 1;

void progfault(int reason) {
	write_pin(LED3_PIN, true);
	delay(200 * DELAY_MS_UNITS);
	write_pin(LED3_PIN, false);
	delay(200 * DELAY_MS_UNITS);
}

/* Buffer size (in bytes) for R/W operations */
#define BYTES_PER_SECTOR 512

static uint8_t readBuffer[BYTES_PER_SECTOR];

int main(void) {


	sparrow_init();
	systick_start(SYSTICK_10MS);

	write_pin(LED1_PIN, true);

	sd_init();


//	nvic_disable_interrupt(NVIC_SDIO);

	write_pin(LED1_PIN, false);

	while (!sd_card_detected()) {}

	write_pin(LED1_PIN, true);

	sd_set_power(true);

	write_pin(LED2_PIN, false);
	bool success = sd_init_card();
	write_pin(LED2_PIN, true);

/*
	uint32_t result = Chip_SDMMC_Acquire(&sdcardinfo);
	write_pin(LED2_PIN, false);
	if (!result) progfault(PERIPHERAL_FAILURE);


	uint32_t numSectors = Chip_SDMMC_GetDeviceBlocks();

	uint32_t read = Chip_SDMMC_ReadBlocks((void*)readBuffer, 0, 1);
	if(read == 0) progfault(PERIPHERAL_FAILURE);
	write_pin(LED3_PIN, true);
*/
}

void systick() {
	static uint32_t counter = 0;
	counter++;
	uint8_t bit = counter / 50;
	uint8_t phase = counter % 50;
	switch (bit) {
		case 8:
		case 9:
			write_pin(LED3_PIN, false);
			break;
		case 10:
			counter = 0;
			break;
		default:
			write_pin(LED3_PIN, (status & (0x80 >> bit)) ? (phase < 30) : (phase < 10));
			break;
	}

}
