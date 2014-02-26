#ifndef _PROGFAULT_
#define _PROGFAULT_

typedef enum {
	ILLEGAL_ARGUMENT = 1,
	NOT_IMPLEMENTED = 2
} PROGFAULT_CODE;

/** function define for programming faults - errors that can always be avoided
by the programmer (i.e. contract violations). Programming faults are not returned
as a runtime result, but jump to this point. This facilitates error handling
in normal situations, since programming errors don't need to be handled during
runtime. progfault defaults to an endless loop, can be overridden by the app. */

#include "startup.h"

void standard_progfault(int error) {
	while (1) {};
}

void progfault(int error) DEFAULTS_TO(standard_progfault);

#endif