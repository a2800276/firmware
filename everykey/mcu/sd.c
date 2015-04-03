#include "sd.h"
#include "nvic.h"
#include "clocks.h"

#include "../core/runtime.h"
#include "../utils/progfault.h"

//jsut for debug
#include "../dkl.h"
#include "../utils/simpleio.h"
#include "../utils/utils.h"

#define SD_BLOCK_SIZE 0x200

#define SD_BMOD_SWR 0x1
#define SD_BMOD_FB 0x2
#define SD_BMOD_DSL4 0x10
#define SD_BMOD_DE 0x80
#define SD_BMOD_PBL4 0x100

#define SD_CTRL_CONTROLLER_RESET              0x1
#define SD_CTRL_FIFO_RESET                    0x2
#define SD_CTRL_DMA_RESET                     0x4
#define SD_CTRL_INT_ENABLE                    0x10
#define SD_CTRL_READ_WAIT                     0x40
#define SD_CTRL_SEND_IRQ_RESPONSE             0x80
#define SD_CTRL_ABORT_READ_DATA               0x100
#define SD_CTRL_SEND_CCSD                     0x200
#define SD_CTRL_SEND_AUTO_STOP_CCSD           0x400
#define SD_CTRL_CEATA_DEVICE_INTERRUPT_STATUS 0x800
#define SD_CTRL_USE_INTERNAL_DMAC             0x2000000

//MSize = 4, TX_WMark = 16, RX_WMark = 15
#define SD_FIFOTH_DEFAULT (16 | (15 << 16) | (1 << 28))


void sd_set_power(bool on) {
  SDMMC_HW->PWREN = on ? 1 : 0;
}

bool sd_card_detected() {
  return (SDMMC_HW->CDETECT & 1) ? false : true;
}

void sd_set_card_type(uint32_t type) {
  SDMMC_HW->CTYPE = type;
}

void sd_set_block_size(uint32_t size) {
  SDMMC_HW->BLKSIZ = size;
}

void sd_set_byte_count(uint32_t count) {
  SDMMC_HW->BYTCNT = count;
}

uint32_t sd_get_raw_interrupt_status() {
  return SDMMC_HW->RINTSTS;
}

void sd_clear_interrupt_status(uint32_t mask) {
  SDMMC_HW->RINTSTS = mask;
}

void sd_set_interrupt_mask(uint32_t mask) {
  SDMMC_HW->INTMASK = mask;
}

void sd_update_clock () {
  SDMMC_HW->CMDARG = 0;
  SDMMC_HW->CMD = SD_CMDFLAG_UPDATE_CLOCK_ONLY | SD_CMDFLAG_WAIT_PRV_DATA_COMPLETE | SD_CMDFLAG_START;
  while (SDMMC_HW->CMD & SD_CMDFLAG_START) {}  //wait until command was accepted. TODO: Timeout
}

//set the clock to a given divider
void sd_set_clock_divider(uint8_t divider) {

  //check if there's something to do
  if ((SDMMC_HW->CLKDIV == divider) && SDMMC_HW->CLKENA) return;

  // disable clock
  SDMMC_HW->CLKENA = 0;
  SDMMC_HW->CLKSRC = 0;
  sd_update_clock();

  // set parameters
  SDMMC_HW->CLKSRC = 0;
  SDMMC_HW->CLKDIV = divider;
  sd_update_clock();

  // enable clock again
  SDMMC_HW->CLKENA = 1;
  sd_update_clock();
}

SD_CMD_ERR sd_cmd(
        SD_CMD cmd,
        uint32_t cmdArg,
        uint32_t flags,
        uint32_t* response,
        bool waitData) {

  //if command triggers data transfer: clear fifos
  if (flags & SD_CMDFLAG_DATA_EXPECTED) {
    SDMMC_HW->CTRL |= SD_CTRL_FIFO_RESET;
    while (SDMMC_HW->CTRL & SD_CTRL_FIFO_RESET) {}
  }

  //clear interrupts
  SDMMC_HW->RINTSTS = 0xffffffff;

  //arm interrupts
    uint32_t wanted_mask = waitData ? SD_INT_DTO : SD_INT_CDONE;

    uint32_t int_mask = wanted_mask;
    int_mask |= SD_INT_EBE | SD_INT_SBE | SD_INT_HLE | SD_INT_RTO_BAR | SD_INT_RCRC | SD_INT_RE;
    if (waitData) int_mask |= SD_INT_FRUN | SD_INT_HTO | SD_INT_DTO | SD_INT_DCRC;
    SDMMC_HW->INTMASK = int_mask;

//  sd_arm_interrupt(int_mask | 0xfffe);

  //transmit command, wait for completion
  uint32_t cmdVal = SD_CMDFLAG_START | ((uint32_t)cmd) | flags;

  SDMMC_HW->CMDARG = cmdArg;
  SDMMC_HW->CMD = cmdVal;
  while (SDMMC_HW->CMD & SD_CMDFLAG_START) {}  //wait until command was accepted. TODO: Timeout
  uint32_t result = 0;

  while (!(result & wanted_mask)) {  //wait for command completion. TODO: Timeout
    result = SDMMC_HW->RINTSTS;
  }

//  uint32_t result = sd_wait_for_interrupt();   //wait for completion. TODO: Timeout
  //check result
  uint32_t error_result = (SD_INT_RE | SD_INT_RCRC | SD_INT_DCRC |
            SD_INT_RTO_BAR | SD_INT_DRTO_BDS | SD_INT_HTO | SD_INT_FRUN | SD_INT_HLE |
            SD_INT_SBE | SD_INT_EBE);
  if (result & error_result) {
    if (result & SD_INT_RE) return SD_CMD_ERR_RE;
    if (result & SD_INT_RCRC) return SD_CMD_ERR_RCRC;
    if (result & SD_INT_DCRC) return SD_CMD_ERR_DCRC;
    if (result & SD_INT_RTO_BAR) return SD_CMD_ERR_RTO;
    if (result & SD_INT_DRTO_BDS) return SD_CMD_ERR_DRTO;
    if (result & SD_INT_HTO) return SD_CMD_ERR_HTO;
    if (result & SD_INT_FRUN) return SD_CMD_ERR_FRUN;
    if (result & SD_INT_HLE) return SD_CMD_ERR_HLE;
    if (result & SD_INT_SBE) return SD_CMD_ERR_SBE;
    if (result & SD_INT_EBE) return SD_CMD_ERR_EBE;
    return SD_CMD_ERR_OTHER;
  }

  //get response
  if (flags & SD_CMDFLAG_EXPECT_RESPONSE) {
    response[0] = SDMMC_HW->RESP[0];
    if (flags & SD_CMDFLAG_LONG_RESPONSE) {
      response[1] = SDMMC_HW->RESP[1];
      response[2] = SDMMC_HW->RESP[2];
      response[3] = SDMMC_HW->RESP[3];
    }
  }
  return SD_CMD_ERR_OK;
}

void sd_init() {
  clock_set_source(BASE_SDIO_CLK, true, CLKSRC_PLL1);    //SD
  clock_enable(CLK_Mx_SDIO, true);
  clock_enable(CLK_SDIO, true);

  SDMMC_HW->BMOD = SD_BMOD_SWR;   //software reset

  SDMMC_HW->CTRL = SD_CTRL_CONTROLLER_RESET | SD_CTRL_FIFO_RESET | SD_CTRL_DMA_RESET;  //reset blocks
  while (SDMMC_HW->CTRL & (SD_CTRL_CONTROLLER_RESET | SD_CTRL_FIFO_RESET | SD_CTRL_DMA_RESET)) {}  //wait for completion

  SDMMC_HW->CTRL = SD_CTRL_USE_INTERNAL_DMAC | SD_CTRL_INT_ENABLE;  //enable DMA and interrupts

  SDMMC_HW->INTMASK = 0;           //disable all interrupts
  SDMMC_HW->RINTSTS = 0xffffffff;  //clear all interrupts
  SDMMC_HW->TMOUT = 0xffffffff;    //set max timeout

  SDMMC_HW->FIFOTH = SD_FIFOTH_DEFAULT;

  // Enable internal DMA, set DSL to 4
  SDMMC_HW->BMOD = SD_BMOD_DE | SD_BMOD_PBL4 | SD_BMOD_DSL4;

  // set clock to invalid, will be set during card init
  SDMMC_HW->CLKENA = 0;
  SDMMC_HW->CLKSRC = 0;
}

void sd_shutdown() {
//  clock_set_source(BASE_SDIO_CLK, false, CLKSRC_PLL1);
  clock_enable(CLK_Mx_SDIO, false);
//  clock_enable(CLK_SDIO, false);
}




#define SD_ARG_VHS 0x100 /* 2.7-3.6V */
#define SD_ARG_CHECKPATTERN 0xDE /* check pattern, arbitrarily chosen */
#define SD_ARG_HCS 0x40000000 /* request / response for high capacity (SDHC/SDXC) cards */
#define SD_ARG_OCR_VOLTAGE_WINDOW 0xff8000 /* voltage window 2.7 - 3.6V */

#define SD_R7_CHECKPATTERN_MASK 0xFF
#define SD_R7_VHS_MASK 0x700

#define SD_CARD_READY_MAX_RETRIES 11 /* delay inbetween = 100ms, specs say 1s max */

bool sd_init_card(SD_CARD_INFO* info) {
  if (!info) progfault(ILLEGAL_ARGUMENT);

  //init all card-related properties to default values
  info->rca = 0;
  info->hc_card = false;
  info->inited = false;
  info->cid[0] = 0;
  info->cid[1] = 0;
  info->cid[2] = 0;
  info->cid[3] = 0;

  SDMMC_HW->CTYPE = 0;  //reset card type
  sd_set_clock_divider(5); //set to 20 MHz. TODO: Don't hard-code this

  SD_CMD_ERR err;
  uint32_t response[4];
  delay(DELAY_MS_UNITS * 100);

  err = sd_cmd(
        SD_CMD_GO_IDLE_STATE,
        SD_ARG_VHS | SD_ARG_CHECKPATTERN,
        SD_CMDFLAG_PREPEND_INIT,
        NULL,
        false);

  if (err != SD_CMD_ERR_OK) return false;

  err = sd_cmd(
      SD_CMD_SEND_IF_COND,
      SD_ARG_VHS | SD_ARG_CHECKPATTERN,
      SD_CMDFLAG_EXPECT_RESPONSE,
      response,
      false);

  if (err != SD_CMD_ERR_OK) return false;
  if ((response[0] & SD_R7_CHECKPATTERN_MASK) != SD_ARG_CHECKPATTERN) return false;
  if ((response[0] & SD_R7_VHS_MASK) != SD_ARG_VHS) return false;

  uint16_t retries = 0;
  bool card_ready = false;

   for (retries = 0; retries < SD_CARD_READY_MAX_RETRIES; retries++) {
    err = sd_cmd(
      SD_CMD_APP_CMD,
      (((uint32_t)info->rca) << 16),
      SD_CMDFLAG_EXPECT_RESPONSE,
      response,
      false);
    if (err != SD_CMD_ERR_OK) return false;

    err = sd_cmd(
      SD_CMD_SEND_OP_COND,
      SD_ARG_HCS | SD_ARG_OCR_VOLTAGE_WINDOW,
      SD_CMDFLAG_EXPECT_RESPONSE,
      response,
      false);
    if (err != SD_CMD_ERR_OK) return false;

    card_ready = response[0] & 0x80000000;
    if (card_ready) {
      info->hc_card = (response[0] & 0x40000000) ? true : false;
      break;
    }
    delay(DELAY_MS_UNITS * 100);
  }

  if (!card_ready) return false;

  err = sd_cmd(
    SD_CMD_ALL_SEND_CID,
    0,
    SD_CMDFLAG_EXPECT_RESPONSE | SD_CMDFLAG_LONG_RESPONSE,
    response,
    false);

  if (err != SD_CMD_ERR_OK) return false;
  memcpy(info->cid, response, 16);

  err = sd_cmd(
    SD_CMD_SEND_RELATIVE_ADDR,
    0,
    SD_CMDFLAG_EXPECT_RESPONSE,
    response,
    false);

  if (err != SD_CMD_ERR_OK) return false;

  info->rca = (response[0] >> 16) & 0xffff;
  info->inited = true;

  return true;

}

typedef enum {
  SD_CARD_STATE_IDLE = 0,
  SD_CARD_STATE_READY = 1,
  SD_CARD_STATE_IDENT = 2,
  SD_CARD_STATE_STBY = 3,
  SD_CARD_STATE_TRAN = 4,
  SD_CARD_STATE_DATA = 5,
  SD_CARD_STATE_RCV = 6,
  SD_CARD_STATE_PRG = 7,
  SD_CARD_STATE_DIS = 8,
  SD_CARD_STATE_INVALID = 99  /* card comm error */
} SD_CARD_STATE;

SD_CARD_STATE sd_get_card_state(SD_CARD_INFO* info) {
  uint32_t response;
  SD_CMD_ERR err = sd_cmd(
      SD_CMD_SEND_STATUS,
      info->rca << 16,
      SD_CMDFLAG_EXPECT_RESPONSE,
      &response,
      false);
  if (err != SD_CMD_ERR_OK) return false;
  return (response >> 9) & 0x0f;
}

typedef struct {
  volatile uint32_t des0;						/*!< Control and status */
  volatile uint32_t des1;						/*!< Buffer size(s) */
  volatile uint32_t des2;						/*!< Buffer address pointer 1 */
  volatile uint32_t des3;						/*!< Buffer address pointer 2 */
} SD_DMA_STRUCT;

SD_DMA_STRUCT sd_dma[1];

bool sd_read_blocks(void* to, uint32_t start_block, uint32_t block_count, SD_CARD_INFO* info, bool wait) {
  write_pin(LED1_PIN, true);
  if (block_count != 8) progfault(NOT_IMPLEMENTED);

  uint32_t len_bytes = (block_count) * SD_BLOCK_SIZE;
  uint32_t start_card_units = info->hc_card ? start_block : (SD_BLOCK_SIZE * start_block);
  uint32_t len_card_units = info->hc_card ? block_count : len_bytes;

  //TODO: Check card capacity **************

  SD_CMD_ERR err;
  uint32_t response;
  // determine card state, put into tran state if needed
  SD_CARD_STATE card_state = sd_get_card_state(info);

  if (card_state == SD_CARD_STATE_STBY) {  //standby: put into tran
    err = sd_cmd(
      SD_CMD_SELECT_CARD,
      info->rca << 16,
      SD_CMDFLAG_EXPECT_RESPONSE,
      &response,
      false);
      if (err != SD_CMD_ERR_OK) return false;

      //TODO: Do we need to test new state? We're optimistic right now...
  } else if (card_state != SD_CARD_STATE_TRAN) return false;  //wrong state

  // set number of bytes to read
  SDMMC_HW->BYTCNT = len_bytes;

  // Reset DMA
  SDMMC_HW->CTRL |= SD_CTRL_DMA_RESET | SD_CTRL_FIFO_RESET;
  while (SDMMC_HW->CTRL & SD_CTRL_DMA_RESET) {}

  //Setup DMA.
  //Note: We currently have len fixed at 4096, so we only need one DMA descriptor
  //TODO: Build DMA chain for longer transfers
  sd_dma[0].des1 = len_bytes;    //buffer 1 size = buffer length
  sd_dma[0].des2 = (uint32_t)to; //buffer 1 address
  sd_dma[0].des3 = 0;            //buffer 2 address
  sd_dma[0].des0 = SD_DMA_FS | SD_DMA_LD | SD_DMA_CH | SD_DMA_OWN;  //flags
  SDMMC_HW->DBADDR = (uint32_t) (&(sd_dma[0]));

  err = sd_cmd(
    SD_CMD_READ_MULTIPLE_BLOCK,
    start_card_units,
    SD_CMDFLAG_DATA_EXPECTED | SD_CMDFLAG_AUTO_STOP | SD_CMDFLAG_EXPECT_RESPONSE,
    &response,
    true);

  if (!wait) {
    return true;
  }
  
  while (sd_get_card_state(info) != SD_CARD_STATE_TRAN) {}

  return (err == SD_CMD_ERR_OK) ? true : false;
}

/*

  // use block indexing for high capacity cards
  uint32_t start = (info->hc_card) ? start_block : (start_block * SD_BLOCK_SIZE);

  Chip_SDIF_DmaSetup(&g_card_info->sdif_dev, (uint32_t)to, byte_count);

  // Read with single / multiple read commands depending on size
  if (num_blocks == 1) {
    status = sdmmc_execute_command(CMD_READ_SINGLE, index, 0 | MCI_INT_DATA_OVER);
  }
  else {
    status = sdmmc_execute_command(CMD_READ_MULTIPLE, index, 0 | MCI_INT_DATA_OVER);
  }

  // Wait for card program to finish
  while (Chip_SDMMC_GetState() != SDMMC_TRAN_ST) {}

  return (status == SD_CMD_ERR_OK) ? true : false;





// SD wait flag
static volatile bool sd_interrupt_fired = false;

static void sd_arm_interrupt(uint32_t mask) {
  nvic_clear_interrupt_pending(NVIC_SDIO);
  sd_interrupt_fired = false;
  sd_set_interrupt_mask(mask);
  nvic_enable_interrupt(NVIC_SDIO);
}

// waits for sd interrupt trigger
static uint32_t sd_wait_for_interrupt(void) {
  while (!sd_interrupt_fired) {}  //wait for interrupt

  uint32_t status = sd_get_raw_interrupt_status();
  sd_clear_interrupt_status(status);
  sd_set_interrupt_mask(0);

  return status;
}

// actual sdio handler
void sdio_handler(void) {
  nvic_disable_interrupt(NVIC_SDIO);
  sd_interrupt_fired = true;
}
*/
