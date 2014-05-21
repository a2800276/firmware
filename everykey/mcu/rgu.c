#include "rgu.h"


void rgu_trigger_reset(RGU_RESET_INDEX idx) {
	RGU->RESET_CTRL[idx/32] = 1 << (idx % 32);
}

bool rgu_reset_active(RGU_RESET_INDEX idx) {
	return (RGU->RESET_ACTIVE_STATUS[idx/32] & (1 << (idx % 32))) != 0;
}
