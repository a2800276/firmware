#include "gpio.h"

/* set pin function. See table 169 in user manual (UM10430) */
void SetPinMode(uint8_t group, uint8_t idx, uint8_t mode) {
	uint32_t val = SCU->GROUP[group].SFS[idx];
	val &= ~0x07;
	val |= (mode & 0x07);
	SCU->GROUP[group].SFS[idx] = val;
}

void EnablePinPullup(uint8_t group, uint8_t idx, bool enable) {
	if (enable) SCU->GROUP[group].SFS[idx] |= 0x08;
	else SCU->GROUP[group].SFS[idx] &= ~0x08;
}

void EnablePinPulldown(uint8_t group, uint8_t idx, bool enable) {
	if (enable) SCU->GROUP[group].SFS[idx] |= 0x10;
	else SCU->GROUP[group].SFS[idx] &= ~0x10;
}

void SetPinSlewRate(uint8_t group, uint8_t idx, bool fast) {
	if (fast) SCU->GROUP[group].SFS[idx] |= 0x20;
	else SCU->GROUP[group].SFS[idx] &= ~0x20;
}

void EnablePinInBuffer(uint8_t group, uint8_t idx, bool enable) {
	if (enable) SCU->GROUP[group].SFS[idx] |= 0x40;
	else SCU->GROUP[group].SFS[idx] &= ~0x40;
}

void EnablePinGlitchFilter(uint8_t group, uint8_t idx, bool enable) {
	if (enable) SCU->GROUP[group].SFS[idx] |= 0x80;
	else SCU->GROUP[group].SFS[idx] &= ~0x80;
}

void SetPinDriveStrength(uint8_t group, uint8_t idx, uint8_t strength) {
	uint32_t val = SCU->GROUP[group].SFS[idx];
	val &= ~0x300;
	val |= ((strength & 0x03) << 8);
	SCU->GROUP[group].SFS[idx] = val;
}


void SetPinDir(uint8_t port, uint8_t pin, PIN_DIR dir) {
	if (dir == OUTPUT) GPIO->DIR[port] |= (1 << pin);
	else GPIO->DIR[port] &= ~(1 << pin);
}

bool ReadDigitalPin(uint8_t port, uint8_t pin) {
	return (GPIO->W[port].PIN[pin] != 0);
}

void WriteDigitalPin(uint8_t port, uint8_t pin, bool value) {
	GPIO->W[port].PIN[pin] = value;
}


