#ifndef _GAINSPAN_
#define _GAINSPAN_

#include "everykey/everykey.h"

bool gs_init(void);

bool gs_reset(bool prog_mode);

bool gs_ping_module();

bool gs_poll(void);

bool gs_program_firmware(void);

#endif