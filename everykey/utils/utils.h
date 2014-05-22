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

/** delay value for 1ms. Tested with GCC 4.7.3, unoptimized code @ 180MHz. USE WITH CAUTION! */
#define DELAY_MS_UNITS 32800

#ifdef __cplusplus
}
#endif

#endif
