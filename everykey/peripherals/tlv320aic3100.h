#ifndef _TLV320AIC3100_
#define _TLV320AIC3100_

#include "../core/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** configure playback. TODO: Refactor to a more general form
@param i2c i2c bus */
void tlv_configure(uint8_t i2c);

#ifdef __cplusplus
}
#endif


#endif
