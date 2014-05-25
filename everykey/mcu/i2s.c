#include "i2s.h"
#include "nvic.h"
#include "scu.h"
#include "clocks.h"
#include "../board.h"
#include "../utils/progfault.h"

static I2S_PLAY_SAMPLE_CALLBACK playCallback;

// Preliminary data source
uint32_t getOutSample() {
	static uint32_t counter = 0;
	counter += 257;
//	write_pin(DEBUG_LED_PIN, counter & 0x40000);
	uint32_t val = (counter & 0xffff);
	return val | (val << 16);
}

// Preliminary data drain
void putInSample(uint32_t sample) {

}

/** find clock dividers for a given clock and audio format. Throws progfault if the needed
parameters cannot be met (i.e. bit rate too low or too high for PCLK)

@param pclkHz PCLK in MHz (normally PCLK1, running at nominal processor speed, but may differ)
@param numChannels 1 for mono, 2 for stereo
@param clocksPerSample bits per channel (must be >= bit depth of samples)
@param sampleRateHz audio sample rate in Hz
@param outDivM MCLK divider on successful return. Must not be NULL.
@param outMulM MCLK multiplier on successful return. Must not be NULL.
@param outDivB TX/RX bit clock divider on successful return (1..64). Must not be NULL.
*/

void findDividers(uint16_t pclkMHz, uint8_t numChannels, uint8_t clocksPerSample, uint32_t sampleRateHz, uint8_t* outDivM, uint8_t* outMulM, uint8_t* outDivB) {

	//To avoid overflows and keep usable fidelity, frequency unit is 10Hz.
	uint32_t pclk = pclkMHz * 50000;	//MHz to 10Hz unit, take into account that MCLK will additionally divide by two
	uint32_t bclk = (numChannels * clocksPerSample * sampleRateHz) / 10; //10Hz unit

	//1. find largest multiple of bclk (*1..*64) below pclk as preliminary divB
	uint8_t divB;
	for (divB = 64; divB > 0; divB--) {
		if (divB * bclk < pclk) break;
	}
	if (divB < 1) progfault(ILLEGAL_ARGUMENT);


	//2. find fraction that gives closest approximation
	uint32_t wantedMclk = bclk * divB;
	uint32_t bestError = 0xffffffff;
	uint8_t bestMulM;
	uint8_t bestDivM;
	uint16_t tryMulM;
	uint16_t tryDivM;
	for (tryDivM = 1; tryDivM < 256; tryDivM++) {
		for (tryMulM = 1; tryMulM <= tryDivM; tryMulM++) {
			uint32_t isMclk = (pclk * tryMulM) / tryDivM;
			uint32_t err = (isMclk > wantedMclk) ? (isMclk - wantedMclk) : (wantedMclk - isMclk);
			if (err < bestError) {
				bestMulM = tryMulM;
				bestDivM = tryDivM;
				bestError = err;
			}
		}
	}

	//3. Simple try to reduce divB if it can be moved over to divM
	// (we want MCLK as low as possible so that it can be used as clock source for codec)
	while ((divB % 2 == 0) && (bestMulM % 2 == 0)) {
		divB /= 2;
		bestMulM /= 2;
	}
	while ((divB % 3 == 0) && (bestMulM % 3 == 0)) {
		divB /= 3;
		bestMulM /= 3;
	}
	while ((divB % 2 == 0) && (bestDivM < 128)) {
		divB /= 2;
		bestDivM *= 2;
	}
	while ((divB % 3 == 0) && (bestDivM < 85)) {
		divB /= 3;
		bestDivM *= 3;
	}
	*outDivB = divB;
	*outDivM = bestDivM;
	*outMulM = bestMulM;
}


void i2s_init(uint8_t i2s) {
	if (i2s > 1) progfault(ILLEGAL_ARGUMENT);
	clock_enable(CLK_APB1_I2S, true);	//Turn on peripheral clock (180MHz)
	I2S[i2s].DAO =  0x87c9; //16 bits stereo master muted stopped, 16 word select half clock
	I2S[i2s].DAI =  0x07c9; //16 bits stereo master stopped, 16 word select half clock
	nvic_enable_interrupt(i2s ? NVIC_I2S1 : NVIC_I2S0);
}

/** configures a pin to use with I2C1. Should be called prior to use for SDA and SCL pin
@param group pin group, e.g. 2 of P2_3
@param idx pin idx, e.g. 3 of P2_3
@param mode I2C mode of pin (see UM10430, SCU), e.g. 1 for P2_3 */
void i2s_configure_pin(uint8_t group, uint8_t idx, uint8_t mode) {
	scu_set_pin_mode(group, idx, mode);
	scu_set_pin_pullup(group, idx, false);
	scu_set_pin_pulldown(group, idx, false);
	scu_enable_pin_in_buffer(group, idx, true);
	scu_enable_pin_glitch_filter(group, idx, true);
}

void i2s_shutdown(uint8_t i2s) {
	if (i2s > 1) progfault(ILLEGAL_ARGUMENT);
	nvic_disable_interrupt(i2s ? NVIC_I2S1 : NVIC_I2S0);
	I2S[i2s].DAO =  0x87c9; //16 bits stereo master muted stopped, 16 word select half clock
	I2S[i2s].DAI =  0x07c9; //16 bits stereo master stopped, 16 word select half clock
	clock_enable(CLK_APB1_I2S, false);

}

void i2s_start_play(uint8_t i2s, uint8_t numChannels, uint8_t bitsPerSample, uint32_t sampleRate, I2S_PLAY_SAMPLE_CALLBACK cb) {
	playCallback = cb;

	if (i2s > 1) progfault(ILLEGAL_ARGUMENT);
	if ((numChannels < 1) || (numChannels > 2)) progfault(ILLEGAL_ARGUMENT);
	if ((bitsPerSample != 8) && (bitsPerSample != 16) && (bitsPerSample != 32)) progfault(ILLEGAL_ARGUMENT);

	uint8_t clocksPerChannel = bitsPerSample; //32;	//must be bitsPerSample..32
	uint8_t mulM, divM, divB;
	findDividers(MAIN_CLOCK_MHZ, numChannels, clocksPerChannel, sampleRate, &divM, &mulM, &divB);

	uint32_t monoMask = (numChannels == 1) ? 4 : 0;
	uint32_t halfWordPeriodMask = (clocksPerChannel-1) << 6;
	uint32_t wordWidthMask;
	if (bitsPerSample == 8) wordWidthMask = 0;
	else if (bitsPerSample == 16) wordWidthMask = 1;
	else wordWidthMask = 3;
	I2S[i2s].IRQ = (I2S[i2s].IRQ & 0x000f03) | 0x040002;	//enable tx interrupt at fill level 4 (half full/empty)
	I2S[i2s].DAO = wordWidthMask | monoMask | halfWordPeriodMask;
	I2S[i2s].TXRATE = divM | (mulM << 8);
	I2S[i2s].TXBITRATE = divB - 1;
}

uint8_t i2s_tx_fifo_level(uint8_t i2s) {
	return (I2S[i2s].STATE >> 16) & 0x0f;
}

uint8_t i2s_rx_fifo_level(uint8_t i2s) {
	return (I2S[i2s].STATE >> 8) & 0x0f;
}

void i2s_handler(uint8_t i2s) {
	while (i2s_tx_fifo_level(i2s) < 8) {
		I2S[i2s].TXFIFO = (*playCallback)();
	}
	while (i2s_rx_fifo_level(i2s) > 0) {
		putInSample(I2S[i2s].RXFIFO);
	}
}

void i2s0_handler(void) {
	i2s_handler(0);
}

void i2s1_handler(void) {
	i2s_handler(1);
}
