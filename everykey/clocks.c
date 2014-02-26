#include "clocks.h"
#include "utils.h"
#include "platformsettings.h"

void clock_enable_xtal(bool enable) {

#if (XTAL_IS_CRYSTAL)
	CGU_HW->XTAL_OSC_CTRL &= ~XTAL_OSC_CTRL_BYPASS;
#else 
	CGU_HW->XTAL_OSC_CTRL |= XTAL_OSC_CTRL_BYPASS;
#endif

#if (XTAL_MHZ > 17)
	CGU_HW->XTAL_OSC_CTRL |= XTAL_OSC_CTRL_HF;
#else 
	CGU_HW->XTAL_OSC_CTRL &= ~XTAL_OSC_CTRL_HF;
#endif

	if (enable) CGU_HW->XTAL_OSC_CTRL &= ~XTAL_OSC_CTRL_ENABLE;
	else CGU_HW->XTAL_OSC_CTRL |= XTAL_OSC_CTRL_ENABLE;
}

bool clock_xtal_is_enabled() {
	return (CGU_HW->XTAL_OSC_CTRL & XTAL_OSC_CTRL_ENABLE)  ? false : true;
}

bool clock_pll_locked(CGU_PLL_ID pll) {
	switch (pll) {
		case CGU_PLL0USB: return ((CGU_HW->PLL0USB_STAT & PLL0_STAT_LOCK_MASK) == PLL0_STAT_LOCKED) ? true : false;
		case CGU_PLL0AUDIO: return ((CGU_HW->PLL0AUDIO_STAT & PLL0_STAT_LOCK_MASK) == PLL0_STAT_LOCKED) ? true : false;
		case CGU_PLL1: return ((CGU_HW->PLL1_STAT & PLL1_STAT_LOCK_MASK) == PLL1_STAT_LOCKED) ? true : false;
		default: return false;
	}
}

void clock_set_pll(CGU_PLL_ID pll, bool enable, CGU_CLKSRC src, uint32_t preDiv, uint32_t fbDiv, uint32_t postDiv, bool fbsel) {
	if (pll == CGU_PLL1) {
		if (enable) {
			uint32_t psel = 0;
			bool direct = false;
			if (postDiv >= 16) psel = 3;
			else if (postDiv >= 8) psel = 2;
			else if (postDiv >= 4) psel = 1;
			else if (postDiv >= 2) psel = 0;
			else direct = true;
			uint32_t nsel = preDiv - 1;
			if (nsel > 3) nsel = 3;
			uint32_t msel = fbDiv - 1;

			CGU_HW->PLL1_CTRL = (fbsel ? PLL1_FBSEL : 0) | (direct ? PLL1_DIRECT : 0) | (psel << 8) /*| PLL1_AUTOBLOCK*/ |
								(nsel << 12) | (msel << 16) | (src << 24);
		} else {
			CGU_HW->PLL1_CTRL = PLL1_PD | PLL1_BYPASS;
		}
	} else {
		// TODO: PLL0USB and PLL0AUDIO **************

	}
}

void clock_set_idiv(uint8_t idivIdx, bool enable, CGU_CLKSRC src, uint8_t div) {
	if (idivIdx > 4) return;
	HW_RW* ctrl_base = &(CGU_HW->IDIVA_CTRL);
	if (enable) {
		ctrl_base[idivIdx] = ((div-1) << 2) | IDIV_AUTOBLOCK | (src << 24);
	} else {
		ctrl_base[idivIdx] = IDIV_PD;
	}
}

void clock_set_source(CGU_BASE_CLOCK clock, bool enable, CGU_CLKSRC src) {
	HW_RW* clk_base = (HW_RW*)(&(CGU_HW->BASE_SAFE_CLK));
	if (enable) {
		clk_base[clock] = CLKOUT_AUTOBLOCK | (src << 24);
	} else {
		clk_base[clock] = CLKOUT_PD;
	}
}

/** sets PLL1 and core MCU speed (BASE_Mx_CLK) to a given multiple of the XTAL frequency.
Note that this might take some microseconds longer than absolutely needed
@param n nominator of multiplier. Values: 1..256. Must result in a valid frequency
@param d denominator of multiplier. Values: 1..4. Must result in a valid frequency
*/
void clock_set_xtal_core_freq(uint8_t n, uint8_t d) {

	uint32_t mhz = (XTAL_MHZ * n) / d;
	bool needRamp = (mhz > 110);
	clock_set_source(BASE_Mx_CLK, true, CLKSRC_IRC);	//assume 12MHz

	if (!clock_xtal_is_enabled()) {
		clock_enable_xtal(true);
		delay(4000);	//250us @ 12MHz
	}
	if (needRamp) {
		clock_set_pll(CGU_PLL1, true, CLKSRC_XTAL, d, n, 2, false);
		while (!clock_pll_locked(CGU_PLL1)) {};
		clock_set_source(BASE_Mx_CLK, true, CLKSRC_PLL1);
		delay(8000);	//min 50us @ 120MHz
	}

	clock_set_pll(CGU_PLL1, true, CLKSRC_XTAL, d, n, 1, false);
	while (!clock_pll_locked(CGU_PLL1)) {};
	clock_set_source(BASE_Mx_CLK, true, CLKSRC_PLL1);
}

void clock_enable(CCU_BRANCH_CLOCK clock, bool on) {
	HW_RW* cfg = &(((HW_RW*)CCU1_HW)[2*clock+64]);
	if (on) *cfg |= CCU_RUN;
	else *cfg &= ~CCU_RUN;
}

void clock_set_auto(CCU_BRANCH_CLOCK clock, bool on) {
	HW_RW* cfg = &(((HW_RW*)CCU1_HW)[2*clock+64]);
	if (on) *cfg |= CCU_AUTO;
	else *cfg &= ~CCU_AUTO;
}

void clock_set_wakeup(CCU_BRANCH_CLOCK clock, bool on) {
	HW_RW* cfg = &(((HW_RW*)CCU1_HW)[2*clock+64]);
	if (on) *cfg |= CCU_WAKEUP;
	else *cfg &= ~CCU_WAKEUP;
}

bool clock_get_enable(CCU_BRANCH_CLOCK clock) {
	return ((((HW_RO*)CCU1_HW)[2*clock+65]) & CCU_RUN) ? true : false;
}

bool clock_get_auto(CCU_BRANCH_CLOCK clock) {
	return ((((HW_RO*)CCU1_HW)[2*clock+65]) & CCU_AUTO) ? true : false;
}

bool clock_get_wakeup(CCU_BRANCH_CLOCK clock) {
	return ((((HW_RO*)CCU1_HW)[2*clock+65]) & CCU_WAKEUP) ? true : false;
}

void clock_init_default() {
	clock_set_xtal_core_freq(MAIN_CLOCK_MHZ/XTAL_MHZ, 1);
}

