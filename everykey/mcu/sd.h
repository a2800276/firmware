#ifndef _SD_
#define _SD_

#include "../core/types.h"

typedef struct {
	HW_RW CTRL;      //Control
	HW_RW PWREN;     //Power enable
	HW_RW CLKDIV;    //Clock divider
	HW_RW CLKSRC;    //Clock source
	HW_RW CLKENA;    //Clock enable
	HW_RW TMOUT;     //Timeout
	HW_RW CTYPE;     //Card type
	HW_RW BLKSIZ;    //Block size
	HW_RW BYTCNT;    //Byte count
	HW_RW INTMASK;   //Interrupt mask
	HW_RW CMDARG;    //Command argument
	HW_RW CMD;       //Command
	HW_RO RESP[4];   //Response
	HW_RO MINTSTS;   //Masked interrupt status
	HW_RW RINTSTS;   //Raw interrupt status
	HW_RO STATUS;    //Status
	HW_RW FIFOTH;    //FIFO threshold watermark
	HW_RO CDETECT;   //Card detect
	HW_RO WRTPRT;    //Write protect
	HW_UU reserved1;
	HW_RO TCBCNT;    //Transferred CIU card byte count
	HW_RO TBBCNT;    //Transferred host to BIU-FIFO byte count
  HW_RW DEBNCE;    //Debounce count
	HW_UU reserved2[4];
	HW_RW RST_N;     //Hardware reset
	HW_UU reserved3;
	HW_RW BMOD;      //Bus mode
	HW_WO PLDMND;    //Poll demand
	HW_RW DBADDR;    //Descriptor list base address
	HW_RW IDSTS;     //Internal DMAC status
	HW_RW IDINTEN;   //Internal DMAC interrupt enable
	HW_RO DSCADDR;   //Current host descriptor address
	HW_RO BUFADDR;   //Current buffer descriptor address
  HW_RW DATA;      //Data from here
} __attribute__((packed)) SDMMC_STRUCT;

#define SDMMC_HW ((SDMMC_STRUCT*)(0x40004000))

#ifdef __cplusplus
extern "C" {
#endif

/** sets the power state (external pin if available)
@param on on or off */
void sd_set_power(bool on);

/** sets one of the clock dividers
@param idx clock divider to set (0-3). Currently, only 0 is allowed.
@param divider divider to set */
void sd_set_clock_divider(uint8_t idx, uint8_t divider);

/** returns the card detect status
@return true if a card was detected, false otherwise */
bool sd_card_detected();

void sd_set_card_type(uint32_t type);
void sd_set_block_size(uint32_t size);
void sd_set_byte_count(uint32_t count);
uint32_t sd_get_raw_interrupt_status();
void sd_clear_interrupt_status(uint32_t mask);
void sd_set_interrupt_mask(uint32_t mask);

/*
Chip_SDIF_GetIntStatus
Chip_SDIF_ClrIntStatus
Chip_SDIF_SetIntMask
Chip_SDIF_PowerOn
*/

#ifdef __cplusplus
}
#endif

#endif
