#include "sd.h"
#include "../utils/progfault.h"

void sd_set_power(bool on) {
  SDMMC_HW->PWREN = on ? 1 : 0;
}

/** sets one of the clock dividers
@param idx clock divider to set (0-3). Currently, only 0 is allowed.
@param divider divider to set */
void sd_set_clock_divider(uint8_t idx, uint8_t divider) {
  if (idx != 0) progfault(ILLEGAL_ARGUMENT);
  SDMMC_HW->CLKDIV = divider;
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
