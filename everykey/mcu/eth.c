#include "eth.h"
#include "rgu.h"
#include "clocks.h"
#include "../board.h"
#include "../utils/utils.h"
#include "../utils/progfault.h"

//Debug
#include "../utils/simpleio.h"

//Clock divider. This value works for a core clock of 15-250 MHz.
#define ETH_MAC_MII_ADDR_CR ETH_MAC_MII_ADDR_CR_102


void eth_init() {
  //turn on clock
  clock_enable(CLK_Mx_ETHERNET, true);

  //trigger reset
  rgu_trigger_reset(ETHERNET_RST);
  while (rgu_reset_active(ETHERNET_RST)) {};

  //reset DMA engine
  ETH_HW->DMA_BUS_MODE |= ETH_DMA_BUS_MODE_SWR;
  delay(DELAY_MS_UNITS);

	// Set DMA bus mode. Long descriptors, burst length = 1 (RPBL should not need to be set)
	ETH_HW->DMA_BUS_MODE = ETH_DMA_BUS_MODE_ATDS | (1 << ETH_DMA_BUS_MODE_PBL_SHIFT) | (1 << ETH_DMA_BUS_MODE_RPBL_SHIFT);

	// Set MAC configuration: full duplex, 100Mbps, port select,
  // 64 bits inter-frame gap, disable receive own in half duplex
  ETH_HW->MAC_CONFIG =
      ETH_MAC_CONFIG_BL_10
    | ETH_MAC_CONFIG_DO
    | ETH_MAC_CONFIG_FES
    | ETH_MAC_CONFIG_PS
    | ETH_MAC_CONFIG_IFG_64;

	// Set default filter: all
	ETH_HW->MAC_FRAME_FILTER = ETH_MAC_FRAME_FILTER_PR | ETH_MAC_FRAME_FILTER_RA;

	// Clear transmit FIFO
	ETH_HW->DMA_OP_MODE = ETH_DMA_OP_MODE_FTF;
  while (ETH_HW->DMA_OP_MODE & ETH_DMA_OP_MODE_FTF) {}

	// Set DMA thresholds for receive (32 bytes) and transmit (64 bytes)
	ETH_HW->DMA_OP_MODE = ETH_DMA_OP_MODE_RTH_32 | ETH_DMA_OP_MODE_TTH_64;

	// Reset MAC interrupts
	ETH_HW->DMA_STAT = ETH_DMA_STAT_ALL;

	// Disable MAC interrupts
	ETH_HW->DMA_INT_EN = 0;
}

void eth_shutdown() {
  //TODO
}

void eth_mii_write(uint8_t reg, uint16_t val) {
  while (ETH_HW->MAC_MII_ADDR & ETH_MAC_MII_ADDR_GB) {}
  ETH_HW->MAC_MII_ADDR = ETH_MAC_MII_ADDR_CR
                          | (ETH_PHY_ADDR << ETH_MAC_MII_ADDR_PA_SHIFT)
                          | ETH_MAC_MII_ADDR_W
                          | (reg << ETH_MAC_MII_ADDR_GR_SHIFT);
  ETH_HW->MAC_MII_DATA = val;
  ETH_HW->MAC_MII_ADDR |= ETH_MAC_MII_ADDR_GB;
  while (ETH_HW->MAC_MII_ADDR & ETH_MAC_MII_ADDR_GB) {
    delay(DELAY_MS_UNITS);
  }
}

uint16_t eth_mii_read(uint8_t reg) {
  while (ETH_HW->MAC_MII_ADDR & ETH_MAC_MII_ADDR_GB) {}
  ETH_HW->MAC_MII_ADDR = ETH_MAC_MII_ADDR_CR
                        | (ETH_PHY_ADDR << ETH_MAC_MII_ADDR_PA_SHIFT)
                        | (reg << ETH_MAC_MII_ADDR_GR_SHIFT);
  ETH_HW->MAC_MII_ADDR |= ETH_MAC_MII_ADDR_GB;
  while (ETH_HW->MAC_MII_ADDR & ETH_MAC_MII_ADDR_GB) {}
  return (uint16_t)(ETH_HW->MAC_MII_DATA);
}
