#include "lan8720a.h"
#include "../mcu/eth.h"
#include "../utils/utils.h"

//Just for debug
#include "../utils/simpleio.h"
#include "../board.h"


#define LAN_RETRY_DELAY DELAY_MS_UNITS

void lan_init() {
  write_pin(DEBUG_LED_PIN, true);
  //Trigger soft reset, wait until finish
  eth_mii_write(LAN8720A_REG_BCR, LAN8720A_BCR_SR);
  while (true) {
    uint16_t val = eth_mii_read(LAN8720A_REG_BCR);
    if (!(val & (LAN8720A_BCR_SR | LAN8720A_BCR_PD))) break;
    delay(DELAY_MS_UNITS);
  }
  //enable auto-negotiation
  eth_mii_write(LAN8720A_REG_BCR, LAN8720A_BCR_ANE);
  write_pin(DEBUG_LED_PIN, false);
}
