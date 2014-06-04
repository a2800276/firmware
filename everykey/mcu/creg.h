#ifndef _CREG_
#define _CREG_

/** MCU configuration registers */

#include "../core/types.h"

typedef struct {
  HW_UU reserved1;
  HW_RW CREG0;
  HW_UU reserved2[62];
  HW_RW MXMEMMAP;
  HW_UU reserved3[5];
  HW_RW CREG5;
  HW_RW DMAMUX;
  HW_RW FLASHCFGA;
  HW_RW FLASHCFGB;
  HW_RW ETBCFG;
  HW_RW CREG6;
  HW_UU reserved4[52];
  HW_RO DHIPID;
  HW_UU reserved5[191];
  HW_RW USB0FLADJ;
  HW_UU reserved6[63];
  HW_RW USB1FLADJ;
} __attribute__((aligned(4))) CREG_STRUCT;

#define CREG_HW ((CREG_STRUCT*)(0x40043000))

typedef enum {
  CREG_CREG6_ETHMODE_MASK = 0x7,
  CREG_CREG6_ETHMODE_MII = 0x0,
  CREG_CREG6_ETHMODE_RMII = 0x4,
  CREG_CREG6_CTOUTCTRL = 0x10,
  CREG_CREG6_I2S0_TX_SCK_IN_SEL = 0x1000,
  CREG_CREG6_I2S0_RX_SCK_IN_SEL = 0x2000,
  CREG_CREG6_I2S1_TX_SCK_IN_SEL = 0x4000,
  CREG_CREG6_I2S1_RX_SCK_IN_SEL = 0x8000,
  CREG_CREG6_EMC_CLK_SEL = 0x10000
} CREG_GREG6_BITS;
#ifdef __cplusplus
extern "C" {
#endif

/** starts up the CREG unit. Should be called before accessing any of the CREG functions */
void creg_init(void);

/** sets the ethernet interface to MII or RMII. Ethernet peripheral should be reset afterwards.
@param rmii set true to select RMII, false for MII */
void creg_set_eth_interface(bool rmii);



#ifdef __cplusplus
}
#endif

#endif
