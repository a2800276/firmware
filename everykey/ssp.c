#include "ssp.h"
#include "clocks.h"
#include "progfault.h"
#include "startup.h"

bool ssp_writebuf_empty(SSP_STRUCT* hw);
bool ssp_writebuf_full(SSP_STRUCT* hw);
bool ssp_readbuf_empty(SSP_STRUCT* hw);
bool ssp_readbuf_full(SSP_STRUCT* hw);
bool ssp_readbuf_overflow(SSP_STRUCT* hw);
void ssp_write_frame(SSP_STRUCT* hw, uint16_t frame);
uint16_t ssp_read_frame(SSP_STRUCT* hw);


void ssp_init(SSP_STRUCT* hw, SSP_FRAMEFORMAT format, uint8_t spiMode, bool master,
	int8_t framelength, uint16_t clockDivide,
	uint16_t sselPin, uint16_t sckPin, uint16_t misoPin, uint16_t mosiPin) {

	if (!master) progfault(NOT_IMPLEMENTED);
	if ((hw != SSP0_HW) && (hw != SSP1_HW)) progfault(ILLEGAL_ARGUMENT);

	//Temporarily disable SSP so that we can configure without surprises
	hw->CR1 &= ~SSP_ENABLE;

	clock_enable(BASE_Mx_CLK, true);
	if (hw == SSP0_HW) {
		clock_set_source(BASE_SSP0_CLK, true, CLKSRC_PLL1);
		clock_enable(CLK_Mx_SSP0, true);
		switch (sselPin) {
		 	case 0x0100: scu_set_pin_mode(1,0,5); break;
		 	case 0x0306: scu_set_pin_mode(3,6,2); break;
		 	case 0x0308: scu_set_pin_mode(3,8,5); break;
		 	case 0x0900: scu_set_pin_mode(9,0,7); break;
		 	case 0x0f01: scu_set_pin_mode(15,1,2); break;
		 	case 0xffff: break;
			default: progfault(ILLEGAL_ARGUMENT);
		}
		switch (sckPin) {
			case 0x0300: scu_set_pin_mode(3,0,4); break;
			case 0x0303: scu_set_pin_mode(3,3,2); break;
			case 0x0f00: scu_set_pin_mode(15,0,0); break;
		 	case 0xffff: break;
			default: progfault(ILLEGAL_ARGUMENT);
		}
		switch (misoPin) {
			case 0x0101: scu_set_pin_mode(1,1,5); break;
			case 0x0306: scu_set_pin_mode(3,6,5); break;
			case 0x0307: scu_set_pin_mode(3,7,2); break;
			case 0x0901: scu_set_pin_mode(9,1,7); break;
			case 0x0f02: scu_set_pin_mode(15,2,2); break;
		 	case 0xffff: break;
			default: progfault(ILLEGAL_ARGUMENT);
		}
		switch (mosiPin) {
			case 0x0102: scu_set_pin_mode(1,2,5); break;
			case 0x0307: scu_set_pin_mode(3,7,5); break;
			case 0x0308: scu_set_pin_mode(3,8,2); break;
			case 0x0902: scu_set_pin_mode(9,2,7); break;
			case 0x0f03: scu_set_pin_mode(15,3,2); break;
		 	case 0xffff: break;
			default: progfault(ILLEGAL_ARGUMENT);
		}
	} else {
		clock_set_source(BASE_SSP1_CLK, true, CLKSRC_PLL1);
		clock_enable(CLK_Mx_SSP1, true);
		switch (sselPin) {
		 	case 0x0105: scu_set_pin_mode(1,5,5); break;
		 	case 0x0114: scu_set_pin_mode(1,20,1); break;
		 	case 0x0f05: scu_set_pin_mode(15,5,2); break;
		 	case 0xffff: break;
			default: progfault(ILLEGAL_ARGUMENT);
		}
		switch (sckPin) {
			case 0x0113: scu_set_pin_mode(1,19,1); break;
			case 0x0f04: scu_set_pin_mode(15,4,0); break;
		 	case 0xffff: break;
			//TODO: CLK0 function 6 ****************
			case 0x1000: progfault(NOT_IMPLEMENTED);
			default: progfault(ILLEGAL_ARGUMENT);
		}
		switch (misoPin) {
			case 0x0000: scu_set_pin_mode(0,0,1); break;
			case 0x0103: scu_set_pin_mode(1,3,5); break;
			case 0x0f06: scu_set_pin_mode(15,6,2); break;
		 	case 0xffff: break;
			default: progfault(ILLEGAL_ARGUMENT);
		}
		switch (mosiPin) {
			case 0x0001: scu_set_pin_mode(0,1,1); break;
			case 0x0104: scu_set_pin_mode(1,4,5); break;
			case 0x0f07: scu_set_pin_mode(15,7,2); break;
		 	case 0xffff: break;
			default: progfault(ILLEGAL_ARGUMENT);
		}
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
