
#include "everykey/everykey.h"

uint8_t status = 1;

void progfault(int reason) {
	write_pin(LED3_PIN, true);
	delay(200 * DELAY_MS_UNITS);
	write_pin(LED3_PIN, false);
	delay(200 * DELAY_MS_UNITS);
}

SD_CARD_INFO card_info;


#define BUFFER_SIZE 2048
#define HALF_BUFFER_SIZE 1024
static uint32_t buffer[BUFFER_SIZE];

static volatile uint32_t playbackIdx = 0;
static volatile uint32_t readBlockIdx = 10;
static volatile uint8_t lastFilledHalfBuffer = 1;

void fillHalfBuffer() {
	lastFilledHalfBuffer = 1-lastFilledHalfBuffer;
	uint32_t* base = (&(buffer[HALF_BUFFER_SIZE*lastFilledHalfBuffer]));
	bool read = sd_read_blocks(base, readBlockIdx, 8, &card_info, false);
	readBlockIdx+=8;
	write_pin(LED2_PIN,!read_pin(LED2_PIN));
}

uint32_t getNextSample() {
	playbackIdx = (playbackIdx+1) % BUFFER_SIZE;
	return buffer[playbackIdx];
	// static uint32_t counter = 0;
	// counter++;
	// return (counter & 0x20) ? 0xa000a000 : 0x60006000;
}

int main(void) {


	sparrow_init();
	systick_start(SYSTICK_10MS);

	sd_init();

	while (!sd_card_detected()) {}

	sd_set_power(true);

	bool success = sd_init_card(&card_info);
	status = success ? 3 : 2;
	fillHalfBuffer();
	fillHalfBuffer();

	audio_on();
	audio_play(2, 16, 44100, getNextSample);

	while (1) {
		uint8_t playingHalfBuffer = playbackIdx / HALF_BUFFER_SIZE;
		if (lastFilledHalfBuffer == playingHalfBuffer) {
			fillHalfBuffer();
		}
	}

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
