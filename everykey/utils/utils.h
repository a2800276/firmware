#ifndef _UTILS_
#define _UTILS_

#define NOP { __asm volatile ( "NOP\n"); }

#include "../core/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** puts the CPU to sleep until an interrupt occurs */
void waitForInterrupt();

/** temporarily disable interrupts (for atomic access - only do this for a short time) */
void disableInterrupts();

/** re-enable interrupts */
void enableInterrupts();

/** simple busy wait
@param count delay to wait. Timing is compiler-dependent, at least 3 clock cycles per count */
void delay(uint32_t count);

#ifdef __cplusplus
}
#endif

#endif
