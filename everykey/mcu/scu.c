#include "scu.h"

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
