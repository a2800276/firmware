#include "nvic.h"

void nvic_set_mask(volatile uint32_t* base, NVIC_INTERRUPT_INDEX interrupt) {
	uint8_t reg = interrupt / 32;
	uint32_t mask = 1 << (interrupt % 32);
	base[reg] = mask;
}

bool nvic_get_mask(volatile uint32_t* base, NVIC_INTERRUPT_INDEX interrupt) {
	uint8_t reg = interrupt / 32;
	uint32_t mask = 1 << (interrupt % 32);
	return (base[reg] & mask) ? true : false;
}

void nvic_enable_interrupt(NVIC_INTERRUPT_INDEX interrupt) {
	nvic_set_mask(&(NVIC->ISER0),interrupt);
}

void nvic_disable_interrupt(NVIC_INTERRUPT_INDEX interrupt) {
	nvic_set_mask(&(NVIC->ICER0),interrupt);
}

bool nvic_is_interrupt_enabled(NVIC_INTERRUPT_INDEX interrupt) {
	return nvic_get_mask(&(NVIC->ISER0),interrupt);
}

void nvic_set_interrupt_pending(NVIC_INTERRUPT_INDEX interrupt) {
	nvic_set_mask(&(NVIC->ISPR0),interrupt);
}

void nvic_clear_interrupt_pending(NVIC_INTERRUPT_INDEX interrupt) {
	nvic_set_mask(&(NVIC->ICPR0),interrupt);
}

bool nvic_is_interrupt_pending(NVIC_INTERRUPT_INDEX interrupt) {
	return nvic_get_mask(&(NVIC->ISPR0),interrupt);
}

bool nvic_is_interrupt_active(NVIC_INTERRUPT_INDEX interrupt) {
	return nvic_get_mask(&(NVIC->IABR0),interrupt);
}

void nvic_set_interrupt_priority(NVIC_INTERRUPT_INDEX interrupt, uint8_t prio) {
	uint8_t reg = interrupt / 4;
	uint8_t shift = 8 * (interrupt % 4);
	volatile uint32_t* base = &(NVIC->IPR0);
	uint32_t val = base[reg];
	val &= ~(0xff << shift);
	val |= prio << shift;
	base[reg] = val;
}

uint8_t nvic_get_interrupt_priority(NVIC_INTERRUPT_INDEX interrupt) {
	uint8_t reg = interrupt / 4;
	uint8_t shift = 8 * (interrupt % 4);
	volatile uint32_t* base = &(NVIC->IPR0);
	return (base[reg] >> shift) & 0xff;
}

void nvic_trigger_interrupt(NVIC_INTERRUPT_INDEX interrupt) {
	NVIC->STIR = interrupt;
}

uint8_t nvic_get_interrupt_group_priority_bits() {
	return 7 - ((SCB->AIRCR & AIRCR_PRIGROUP_MASK) >> 8);
}

void nvic_set_interrupt_group_priority_bits(uint8_t groupLength) {
	SCB->AIRCR = AIRCR_VECTKEY | ((7-groupLength) << 8);
}

void nvic_reset_system() {
	SCB->AIRCR = AIRCR_VECTKEY | AIRCR_SYSRESETREQ;
}


