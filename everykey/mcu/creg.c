#include "creg.h"
#include "clocks.h"

void creg_init(void) {
    clock_enable(CLK_Mx_CREG, true);
}

void creg_set_eth_interface(bool rmii) {
  CREG_HW->CREG6 = (CREG_HW->CREG6 & (~((uint32_t)CREG_CREG6_ETHMODE_MASK))) | (rmii ? CREG_CREG6_ETHMODE_RMII : CREG_CREG6_ETHMODE_MII);
}
