
#include "everykey/everykey.h"
#include "sdmmc.h"

void progfault(int reason) {
	write_pin(LED3_PIN, true);
	delay(200 * DELAY_MS_UNITS);
	write_pin(LED3_PIN, false);
	delay(200 * DELAY_MS_UNITS);
}

/* Buffer size (in bytes) for R/W operations */
#define BYTES_PER_SECTOR 512

static uint8_t readBuffer[BYTES_PER_SECTOR];

/* SDIO wait flag */
static volatile bool sdio_event_fired = false;

/* SDMMC card info structure */
mci_card_struct sdcardinfo;

/* Very stupid delay function */
static void sdmmc_waitms(uint32_t time) {
	//very unreliable busy wait
	delay(DELAY_MS_UNITS * time);
}

/** sets up the sdio interrupt */
static void arm_sdio_interrupt(void *bits) {
	uint32_t bit_mask = *((uint32_t *)bits);
	/* Wait for IRQ - for an RTOS, you would pend on an event here with a IRQ based wakeup. */
	nvic_clear_interrupt_pending(NVIC_SDIO);
	sdio_event_fired = false;
	sd_set_interrupt_mask(bit_mask);
	nvic_enable_interrupt(NVIC_SDIO);
}

/** waits for sdio interrupt trigger */
static uint32_t wait_for_sdio_interrupt(void) {
	while (!sdio_event_fired) {}  //wait for interrupt

	uint32_t status = sd_get_raw_interrupt_status();
	sd_clear_interrupt_status(status);
	sd_set_interrupt_mask(0);

	return status;
}

/** actual sdio handler */
void sdio_handler(void) {
	nvic_disable_interrupt(NVIC_SDIO);
	sdio_event_fired = true;
}

int main(void) {

	/** enables SD base clock, configures pins */
	sparrow_init();

//	nvic_disable_interrupt(NVIC_SDIO);

	memset(&sdcardinfo, 0, sizeof(sdcardinfo));
	sdcardinfo.card_info.evsetup_cb = arm_sdio_interrupt;
	sdcardinfo.card_info.waitfunc_cb = wait_for_sdio_interrupt;
	sdcardinfo.card_info.msdelay_func = sdmmc_waitms;

	/* The SDIO driver needs to know the SDIO clock rate */
	sd_init();

//	nvic_disable_interrupt(NVIC_SDIO);

	while (!sd_card_detected()) {}

	write_pin(LED1_PIN, true);

	sd_set_power(true);

	write_pin(LED2_PIN, true);
	uint32_t result = Chip_SDMMC_Acquire(&sdcardinfo);
	write_pin(LED2_PIN, false);
	if (!result) progfault(PERIPHERAL_FAILURE);


	uint32_t numSectors = Chip_SDMMC_GetDeviceBlocks();

	uint32_t read = Chip_SDMMC_ReadBlocks((void*)readBuffer, 0, 1);
	if(read == 0) progfault(PERIPHERAL_FAILURE);
	write_pin(LED3_PIN, true);

}
