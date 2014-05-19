#ifndef _TLV320AIC3100_
#define _TLV320AIC3100_

#include "../types.h"

/** issues a soft reset to the device */
void tlv_sw_reset(uint8_t i2c);

void tlv_configure(uint8_t i2c);


#endif
