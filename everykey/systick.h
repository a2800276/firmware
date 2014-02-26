#ifndef _SYSTICK_
#define _SYSTICK_

#include "types.h"
// -----------------------------------
// --- SYSTICK  ----------------------
// -----------------------------------

/** SYSTICK is common to all Cortex M3 controllers, documented in the ARM documentation */
typedef struct {
	HW_UU UNUSED[4];	//Padding
	HW_RW CTRL;			//Control and status register
	HW_RW LOAD;			//Reload value
	HW_RW VAL;			//Current value
	HW_RO CALIB;		//Calibration
} SYSTICK_STRUCT;

#define SYSTICK_HW ((SYSTICK_STRUCT*)(0xe000e000))

/** This value can be used to start the systick timer in the recommended interval of 100Hz when running in full speed */
#define SYSTICK_10MS (MAIN_CLOCK_MHZ*10000-1)

/** Start systick using system clock
@param cycles 24bit value of clocks to elapse between systick invocations (-1) */
void systick_start(uint32_t clocks);

/** Stops systick */
void systick_stop(void);

#endif
