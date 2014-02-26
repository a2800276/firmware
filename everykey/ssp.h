#ifndef _SSP_
#define _SSP_

#include "types.h"

typedef struct {
	HW_RW CR0;
	HW_RW CR1;
	HW_RW DR;
	HW_RO SR;
	HW_RW CPSR;
	HW_RW IMSC;
	HW_RO RIS;
	HW_RO MIS;
	HW_RW ICR;
	HW_RW DMACR;
} __attribute__((packed)) SSP_STRUCT;

#define SSP0_HW ((SSP_STRUCT*)(0x40083000))
#define SSP1_HW ((SSP_STRUCT*)(0x400C5000))

typedef enum {
	SSP_FRAMEFORMAT_SPI       = 0x00,
	SSP_FRAMEFORMAT_TI        = 0x10,
	SSP_FRAMEFORMAT_MICROWIRE = 0x20,
} SSP_FRAMEFORMAT;

typedef enum {
	SSP_DATASIZE_MASK         = 0x0f,
	SSP_FRAMEFORMAT_MASK      = 0x30,
	SSP_CPOL                  = 0x40,
	SSP_CPHA                  = 0x80,
	SSP_CLOCKRATE_MASK        = 0xff00
} SSP_CR0_VALUES;

typedef enum {
	SSP_LOOPBACK              = 0x01,
	SSP_ENABLE                = 0x02,
	SSP_SLAVE                 = 0x04,
	SSP_SLAVEOUTDISABLE       = 0x08
} SSP_CR1_VALUES;

typedef enum {
	SSP_INT_RECEIVE_OVERRUN   = 1,
	SSP_INT_RECEIVE_TIMEOUT   = 2,
	SSP_INT_RX_HALF_FULL      = 4,
	SSP_INT_TX_HALF_FULL      = 8
} SSP_INT_VALUES;

typedef enum {
	SSP_STAT_TX_EMPTY         = 1,
	SSP_STAT_TX_NOT_FULL      = 2,
	SSP_STAT_RX_NOT_EMPTY     = 4,	
	SSP_STAT_RX_FULL          = 8,
	SSP_STAT_BUSY             = 16
} SSP_STAT_VALUES;

/** initializes a ssp module.
@param hw ssp peripheral to init
@param format frame format: SPI, TI or Microwire
@param spiMode transfer mode if format is SPI (0..3). Ignored otherwise.
@param master set true to configure master role, false for slave
@param framelength length of transfer units in bits (4..16)
@param clockDivide bit clock rate in relation to the main (PLL1) frequency
@param sselPin (hi byte: port major, low byte: port minor, major 16 = clocks, unused = 0xffff) 
@param sckPin (hi byte: port major, low byte: port minor, major 16 = clocks, unused = 0xffff) 
@param misoPin (hi byte: port major, low byte: port minor, major 16 = clocks, unused = 0xffff) 
@param mosiPin (hi byte: port major, low byte: port minor, major 16 = clocks, unused = 0xffff) */ 

void ssp_init(SSP_STRUCT* hw, SSP_FRAMEFORMAT format, uint8_t spiMode, bool master,
	int8_t framelength, uint16_t clockDivide,
	uint16_t sselPin, uint16_t sckPin, uint16_t misoPin, uint16_t mosiPin);

/** synchronus (blocking) SSP transfer, 8-bit buffer version.
Only use for frame lengths of up to 8 bits.
@param hw ssp peripheral to use (should be properly inited...)
@param length number of frames to transfer
@param writeBuf buffer to transmit. Set to null to send zero frames.
@param readBuf buffer to hold received data. Set to null to ignore read frames.
@param lsbFirst set to true to reverse bytes before sending and after receiving. If false, bytes are transferred msb first. Only works for 8 bit frames.
@return true on success, false otherwise */ 
bool ssp_write_read_sync8(SSP_STRUCT* hw, uint16_t length, const uint8_t* writeBuf, uint8_t* readBuffer, bool lsbFirst);

#endif