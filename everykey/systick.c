#include "platformsettings.h"
#include "systick.h"

void systick_start(uint32_t clocks) {
	//TODO: Check if systick clock source is main clock
	SYSTICK_HW->CTRL = 0;
	SYSTICK_HW->LOAD = clocks;
	SYSTICK_HW->VAL = 0;
	SYSTICK_HW->CTRL = 7;
}

void systick_stop() {
	SYSTICK_HW->CTRL = 0;
}
