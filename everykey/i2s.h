#ifndef _I2S_
#define _I2S_

#include "types.h"

typedef struct {
	HW_RW DAO;
	HW_RW DAI;
	HW_WO TXFIFO;
	HW_RO RXFIFO;
	HW_RO STATE;
	HW_RW DMA1;
	HW_RW DMA2;
	HW_RW IRQ;
	HW_RW TXRATE;
	HW_RW RXRATE;
	HW_RW TXBITRATE;
	HW_RW RXBITRATE;
	HW_RW TXMODE;
	HW_RW RXMODE;
	HW_UU padding[1010];
} I2S_STRUCT;

#define I2S ((I2S_STRUCT*)(0x400A2000))

void i2s_init(uint8_t i2s);

/** configures a pin to use with I2C1. Should be called prior to use for SDA and SCL pin
@param group pin group, e.g. 2 of P2_3
@param idx pin idx, e.g. 3 of P2_3
@param mode I2C mode of pin (see UM10430, SCU), e.g. 1 for P2_3 */
void i2s_configure_pin(uint8_t group, uint8_t idx, uint8_t mode);

void i2s_shutdown(uint8_t i2s);

void i2s_start_play(uint8_t i2s, uint8_t numChannels, uint8_t bitsPerSample, uint32_t sampleRate);


#endif