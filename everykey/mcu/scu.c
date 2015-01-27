#include "scu.h"
#include "gpio.h"
#include "nvic.h"

/* set pin function. See table 169 in user manual (UM10430) */
void scu_set_pin_mode(uint8_t group, uint8_t idx, uint8_t mode) {
	uint32_t val = SCU->GROUP[group].SFS[idx];
	val &= ~0x07;
	val |= (mode & 0x07);
	SCU->GROUP[group].SFS[idx] = val;
}

void scu_set_pin_pullup(uint8_t group, uint8_t idx, bool enable) {
	if (enable) SCU->GROUP[group].SFS[idx] |= 0x10;
	else SCU->GROUP[group].SFS[idx] &= ~0x10;
}

void scu_set_pin_pulldown(uint8_t group, uint8_t idx, bool enable) {
	if (enable) SCU->GROUP[group].SFS[idx] |= 0x08;
	else SCU->GROUP[group].SFS[idx] &= ~0x08;
}

void scu_set_pin_slew_rate(uint8_t group, uint8_t idx, bool fast) {
	if (fast) SCU->GROUP[group].SFS[idx] |= 0x20;
	else SCU->GROUP[group].SFS[idx] &= ~0x20;
}

void scu_enable_pin_in_buffer(uint8_t group, uint8_t idx, bool enable) {
	if (enable) SCU->GROUP[group].SFS[idx] |= 0x40;
	else SCU->GROUP[group].SFS[idx] &= ~0x40;
}

void scu_enable_pin_glitch_filter(uint8_t group, uint8_t idx, bool enable) {
	if (enable) SCU->GROUP[group].SFS[idx] |= 0x80;
	else SCU->GROUP[group].SFS[idx] &= ~0x80;
}

void scu_set_pin(uint8_t group, uint8_t idx, uint8_t mode, bool pullup, bool pulldown, bool hs_slew, bool in_buffer, bool glitch_filter) {
	scu_set_pin_mode(group, idx, mode);
	scu_set_pin_pullup(group, idx, pullup);
	scu_set_pin_pulldown(group, idx, pulldown);
	scu_set_pin_slew_rate(group, idx, hs_slew);
	scu_enable_pin_in_buffer(group, idx, in_buffer);
	scu_enable_pin_glitch_filter(group, idx, glitch_filter);
}

void scu_set_pin_drive_strength(uint8_t group, uint8_t idx, uint8_t strength) {
	uint32_t val = SCU->GROUP[group].SFS[idx];
	val &= ~0x300;
	val |= ((strength & 0x03) << 8);
	SCU->GROUP[group].SFS[idx] = val;
}

void scu_set_i2c0_pinmode(bool enable, bool fastplus, bool glitch, bool longglitch) {
	SCU->SFSI2C0 = enable ?
		(0x0808 | (fastplus ? 0x0404 : 0) | (glitch ? 0x8080 : 0) | (longglitch ? 0x0101 : 0)) : 0;
}

void scu_set_clock_pin_mode(uint8_t idx, uint8_t func, bool pd, bool pu, bool hs, bool buf, bool glitch) {
	SCU->SFSCLK[idx] =
		(func &0x07) | (pd ? 0x8 : 0) | (pu ? 0 : 0x10) | (hs ? 0x20 : 0) | (buf ? 0x40 : 0) | (glitch ? 0x80 : 0);
}

void scu_enable_pin_interrupt(uint8_t idx, uint8_t port, uint8_t pin, PIN_INTERRUPT_MODE mode) {
	scu_disable_pin_interrupt(idx);
	if (idx < 8) {
		uint8_t pintsel_idx = idx / 4;
		uint8_t shift = (idx % 4) * 8;
		uint32_t val = ((port << 5) | pin) << shift;
		uint32_t mask = ~(0xff << shift);
		SCU->PINTSEL[pintsel_idx] = (SCU->PINTSEL[pintsel_idx] & mask) | val;
	} 
	bool level = (mode == PIN_INTERRUPT_LOW) || (mode == PIN_INTERRUPT_HIGH) || (mode == PIN_INTERRUPT_NONE);
	bool rising = (mode == PIN_INTERRUPT_RISING) || (mode == PIN_INTERRUPT_RISINGFALLING) ||
	              (mode == PIN_INTERRUPT_HIGH) || (mode == PIN_INTERRUPT_LOW);
	bool falling = (mode == PIN_INTERRUPT_FALLING) || (mode == PIN_INTERRUPT_RISINGFALLING) ||
	               (mode == PIN_INTERRUPT_HIGH);
	uint32_t mask = ~(1 << idx);
	GPIO_INTERRUPT_HW->ISEL = (GPIO_INTERRUPT_HW->ISEL & mask) | ((level?1:0) << idx);
	GPIO_INTERRUPT_HW->IENR = (GPIO_INTERRUPT_HW->IENR & mask) | ((rising?1:0) << idx);
	GPIO_INTERRUPT_HW->IENF = (GPIO_INTERRUPT_HW->IENF & mask) | ((falling?1:0) << idx);
	nvic_enable_interrupt(NVIC_PIN_INT0 + idx);
}

/** disables one of eight pin-specific interrupt registers
@param idx interrupt index */
void scu_disable_pin_interrupt(uint8_t idx) {
	uint32_t mask = ~(1 << idx);
	GPIO_INTERRUPT_HW->ISEL &= mask;
	GPIO_INTERRUPT_HW->IENR &= mask;
	GPIO_INTERRUPT_HW->IENF &= mask;
	nvic_disable_interrupt(NVIC_PIN_INT0 + idx);
}

void scu_clear_edge_pin_interrupt(uint8_t idx) {
	GPIO_INTERRUPT_HW->IST |= (1 << idx);
}

