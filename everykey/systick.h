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

#define SYSTICK ((SYSTICK_STRUCT*)(0xe000e000))

/** Start systick using system clock
@param cycles 24bit value of clocks to elapse between systick invocations  */
void start_systick(uint32_t clocks);

/** Starts systick in a 10ms interval. This function assumes the processor in full speed. */
void start_systick_10ms();

/** Stops systick */
void stop_systick(void);

#endif
