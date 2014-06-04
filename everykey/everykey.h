/* Umbrella header for the entire firmware library to be included by applications */

#ifndef _EVERYKEY_
#define _EVERYKEY_

#include "board.h"
#include "core/types.h"
#include "core/runtime.h"
#include "mcu/vectortable.h"
#include "mcu/nvic.h"
#include "mcu/scu.h"
#include "mcu/rgu.h"
#include "mcu/clocks.h"
#include "mcu/gpio.h"
#include "mcu/systick.h"
#include "mcu/ssp.h"
#include "mcu/i2c.h"
#include "mcu/i2s.h"
#include "mcu/sd.h"
#include "mcu/eth.h"

#include "utils/utils.h"
#include "utils/endian.h"
#include "utils/utils.h"
#include "utils/simpleio.h"
#include "utils/progfault.h"

#include "sparrow.h"

#endif
