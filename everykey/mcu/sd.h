#ifndef _SD_
#define _SD_

#include "../core/types.h"

typedef struct {
	uint16_t rca;
	bool hc_card;
	bool inited;
	uint32_t cid[4];
} SD_CARD_INFO;

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
} __attribute__((aligned(4))) SDMMC_STRUCT;

#define SDMMC_HW ((SDMMC_STRUCT*)(0x40004000))


typedef enum {
	SD_INT_CDET = 1,                              //card detect
	SD_INT_RE = 2,                                //response error
	SD_INT_CDONE = 4,                             //command done
	SD_INT_DTO = 8,                               //data transfer over
	SD_INT_TXDR = 0x10,                           //transmit FIFO data request
	SD_INT_RXDR = 0x20,                           //receive FIFO data request
	SD_INT_RCRC = 0x40,                           //response CRC error
	SD_INT_DCRC = 0x80,                           //data CRC error
	SD_INT_RTO_BAR = 0x100,                       //Response timeout
	SD_INT_DRTO_BDS = 0x200,                      //Data read timeout
	SD_INT_HTO = 0x400,                           //Data starvation by host
	SD_INT_FRUN = 0x800,                          //FIFO underrun / overrun
	SD_INT_HLE = 0x1000,                          //Hardware locked error
	SD_INT_SBE = 0x2000,                          //Start bit error
	SD_INT_ACD = 0x4000,                          //Auto command done
	SD_INT_EBE = 0x8000,                          //End bit error
	SD_INT_SDIO_INT = 0x10000,                    //interrupt from SDIO card
} SD_INT;


typedef enum {
	SD_CMD_GO_IDLE_STATE = 0,                     //return to idle state. No response
	SD_CMD_SEND_IF_COND = 8,                      //send interface condition, needed for high capacity. short response R7
	SD_CMD_APP_CMD = 55,                          //next cmd is an app-specific command (ACMD). Short response R1
	SD_CMD_SEND_OP_COND = 41,                     //request sdmem card init. Follows APP_CMD. Short response R3
	SD_CMD_ALL_SEND_CID = 2,                      //ask for 126 bit card ID - long response R2
	SD_CMD_SEND_RELATIVE_ADDR = 3,                //ask for card's relative address - short response R6
	SD_CMD_SEND_STATUS = 13,                      //send card status and state - short response R1
	SD_CMD_SELECT_CARD = 7,                       //toggle between stby and tran state - short response R1b
	SD_CMD_READ_MULTIPLE_BLOCK = 18               //read from sdmem - short response R1
} SD_CMD;

typedef enum {
	SD_CMDFLAG_EXPECT_RESPONSE = 0x40,            //response from card expected
	SD_CMDFLAG_LONG_RESPONSE = 0x80,              //expect long response
	SD_CMDFLAG_CHECK_RESPONSE_CRC = 0x100,        //enable response CRC check
	SD_CMDFLAG_DATA_EXPECTED = 0x200,             //command has data
	SD_CMDFLAG_DATA_WRITE = 0x400,                //data dir is out
	SD_CMDFLAG_DATA_STREAM = 0x800,               //data is in stream mode (not in block mode)
	SD_CMDFLAG_AUTO_STOP = 0x1000,                //automatically send stop after transfer
	SD_CMDFLAG_WAIT_PRV_DATA_COMPLETE = 0x2000,   //wait previous data complete. Unset e.g. for status query or abort
	SD_CMDFLAG_STOP_ABORT = 0x4000,               //this is a stop or abort command from data, reset CIU afterwards
	SD_CMDFLAG_PREPEND_INIT = 0x8000              //Add card init sequence (80 clocks) before this command
	// Note: Some CMD flags missing
} SD_CMD_FLAGS;

#define SD_CMDFLAG_UPDATE_CLOCK_ONLY 0x200000			 /* ignore actual command, just update clock - don't set from user code */
#define SD_CMDFLAG_START 0x80000000                 /* start bit - don't set from user code */

typedef enum {
	SD_CMD_ERR_OK = 0,   //Success
	SD_CMD_ERR_RE,       //Response error
	SD_CMD_ERR_RCRC,     //Response CRC error
	SD_CMD_ERR_DCRC,     //Data CRC error
	SD_CMD_ERR_RTO,      //Response timeout
	SD_CMD_ERR_DRTO,     //Data read timeout
	SD_CMD_ERR_HTO,      //Data starvation by host timeout
	SD_CMD_ERR_FRUN,     //FIFO overrun / underrun
	SD_CMD_ERR_HLE,      //Hardware locked error
	SD_CMD_ERR_SBE,      //Start bit error
	SD_CMD_ERR_EBE,      //End bit error
	SD_CMD_ERR_OTHER     //Unknown / other error. Should not happen.
} SD_CMD_ERR;

typedef enum {
	SD_DMA_DIC = 0x2,    //disable interrupt on completion
	SD_DMA_LD = 0x4,     //last descriptor
	SD_DMA_FS = 0x8,     //first descriptor
	SD_DMA_CH = 0x10,    //second address chained
	SD_DMA_ER = 0x20,    //end of ring
} SD_DMA_FLAGS;

#define SD_DMA_OWN 0x80000000 /* own flag */

#ifdef __cplusplus
extern "C" {
#endif

/** sets the power state (external pin if available)
@param on on or off */
void sd_set_power(bool on);

/** sets the clock. Forwards to CIU.
@param divider divider to set */
void sd_set_clock_divider(uint8_t divider);

/** returns the card detect status
@return true if a card was detected, false otherwise */
bool sd_card_detected();

/** send a command
@param cmd command to send
@param cndArg command args
@param OR-ed SD_CMD_FLAGS
@param response buffer to a response buffer. Must be large enough to hold the response.
@param waitData set to true to wait until the data phase is done. */
SD_CMD_ERR sd_cmd(SD_CMD cmd, uint32_t cmdArg, uint32_t flags, uint32_t* response, bool waitData);

/** initializes an inserted SD card
@param info pointer to user-provided (uninitialized) info structure. Must live during card use.
@return success */
bool sd_init_card(SD_CARD_INFO* info);

/** reads data from a memory card. CURRENTLY ONLY 4096 BYTES (8 BLOCKS) LENGTH !!!!!!!!!!!
@param to pointer to a RAM memory buffer to store the read data
@param start_block block to start reading (standard block size = 512 bytes)
@param block_count number of blocks to read (standard block size = 512 bytes)
@param info info block filled from successful sd_init_card().
@param wait wait for completion
@return success */
bool sd_read_blocks(void* to, uint32_t start_block, uint32_t block_count, SD_CARD_INFO* info, bool wait);

//raw property getters - may go later
void sd_set_card_type(uint32_t type);
void sd_set_block_size(uint32_t size);
void sd_set_byte_count(uint32_t count);
uint32_t sd_get_raw_interrupt_status();
void sd_clear_interrupt_status(uint32_t mask);
void sd_set_interrupt_mask(uint32_t mask);


#ifdef __cplusplus
}
#endif

#endif
