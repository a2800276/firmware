#include "ssp.h"
#include "clocks.h"
#include "startup.h"
#include "scu.h"
#include "../utils/progfault.h"

bool ssp_writebuf_empty(SSP_STRUCT* hw);
bool ssp_writebuf_full(SSP_STRUCT* hw);
bool ssp_readbuf_empty(SSP_STRUCT* hw);
bool ssp_readbuf_full(SSP_STRUCT* hw);
bool ssp_readbuf_overflow(SSP_STRUCT* hw);
void ssp_write_frame(SSP_STRUCT* hw, uint16_t frame);
uint16_t ssp_read_frame(SSP_STRUCT* hw);


void ssp_init(SSP_STRUCT* hw, SSP_FRAMEFORMAT format, uint8_t spiMode, bool master,
	int8_t framelength, uint16_t clockDivide) {

	if (!master) progfault(NOT_IMPLEMENTED);
	if ((hw != SSP0_HW) && (hw != SSP1_HW)) progfault(ILLEGAL_ARGUMENT);

	//Temporarily disable SSP so that we can configure without surprises
	hw->CR1 &= ~SSP_ENABLE;

	clock_set_source(BASE_Mx_CLK, true, CLKSRC_PLL1);
	if (hw == SSP0_HW) {
		clock_set_source(BASE_SSP0_CLK, true, CLKSRC_PLL1);
		clock_enable(CLK_Mx_SSP0, true);
		clock_enable(CLK_APB0_SSP0, true);
	} else {
		clock_set_source(BASE_SSP1_CLK, true, CLKSRC_PLL1);
		clock_enable(CLK_Mx_SSP1, true);
		clock_enable(CLK_APB2_SSP1, true);
	}

	bool cpol = false;
	bool cpha = false;

	if (format == SSP_FRAMEFORMAT_SPI) {
		cpol = ((spiMode & 0x02) != 0);
		cpha = ((spiMode & 0x01) != 0);
	}
	//We want the prescaler as low as possible (highest base clock)
	uint16_t prescaler = (clockDivide >> 9) * 2 + 2;
	uint8_t clockrate = (clockDivide-1) / prescaler;
	hw->CPSR = prescaler;

	hw->IMSC = 0;

	//Set config
	hw->CR0 = (framelength-1) | format | (cpol ? SSP_CPOL : 0) | (cpha ? SSP_CPHA : 0) | (clockrate << 8);
	hw->CR1 = master ? 0 : SSP_SLAVE;

	//Finally enable hardware
	hw->CR1 |= SSP_ENABLE;

	while (!ssp_readbuf_empty(hw)) ssp_read_frame(hw);
}

bool ssp_writebuf_empty(SSP_STRUCT* hw) {
	return (hw->SR & SSP_STAT_TX_EMPTY) != 0;
}

bool ssp_writebuf_full(SSP_STRUCT* hw) {
	return (hw->SR & SSP_STAT_TX_NOT_FULL) == 0;
}

bool ssp_readbuf_empty(SSP_STRUCT* hw) {
	return (hw->SR & SSP_STAT_RX_NOT_EMPTY) == 0;
}

bool ssp_readbuf_full(SSP_STRUCT* hw) {
	return (hw->SR & SSP_STAT_RX_FULL) != 0;
}

bool ssp_readbuf_overflow(SSP_STRUCT* hw) {
	return (hw->RIS & SSP_INT_RECEIVE_OVERRUN) != 0;
}

void ssp_write_frame(SSP_STRUCT* hw, uint16_t frame) {
	hw->DR = frame;
}

uint16_t ssp_read_frame(SSP_STRUCT* hw) {
	return hw->DR;
}

uint8_t ssp_reverse8(uint8_t val) {
	val = ((val & 0xaa)>>1) | ((val & 0x55)<<1);
	val = ((val & 0xcc)>>2) | ((val & 0x33)<<2);
	val = ((val & 0xf0)>>4) | ((val & 0x0f)<<4);
	return val;
}

bool ssp_write_read_sync8(SSP_STRUCT* hw, uint16_t length, const uint8_t* writeBuf, uint8_t* readBuffer, bool lsbFirst) {

//	while (!ssp_readbuf_empty(hw)) {	//Flush read buffer
//		ssp_read_frame(hw);
//	}
	hw->ICR = 3;						//Clear interrupts
	uint16_t sent = 0;
	uint16_t received = 0;
	while ((sent < length) || (received < length)) {
		if ((!(ssp_writebuf_full(hw))) && (sent < length)) {
			uint8_t outVal = writeBuf ? writeBuf[sent] : 0;
			if (lsbFirst) outVal = ssp_reverse8(outVal);
			sent++;
			ssp_write_frame(hw, outVal);
		}
		if (ssp_readbuf_overflow(hw)) return false;
		if ((!(ssp_readbuf_empty(hw))) && (received < length)) {
			uint16_t inVal = ssp_read_frame(hw);
			if (lsbFirst) inVal = ssp_reverse8(inVal);
			if (readBuffer) readBuffer[received] = inVal;
			received++;
		}
	}
	return true;
}
