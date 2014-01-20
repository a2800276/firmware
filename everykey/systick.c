#include "platformsettings.h"
#include "systick.h"

void start_systick(uint32_t clocks) {
	//TODO: Check if systick clock source is main clock
	SYSTICK->CTRL = 0;
	SYSTICK->LOAD = clocks;
	SYSTICK->VAL = 0;
	SYSTICK->CTRL = 7;
}

void start_systick_10ms() {
	start_systick(MAIN_CLOCK_MHZ*10000-1);
}

void stop_systick() {
	SYSTICK->CTRL = 0;
}
