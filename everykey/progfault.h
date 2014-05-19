#ifndef _PROGFAULT_
#define _PROGFAULT_

typedef enum {
	ILLEGAL_ARGUMENT = 1,
	NOT_IMPLEMENTED = 2,
	PERIPHERAL_FAILURE = 3
} PROGFAULT_CODE;

/** function define for programming faults - errors that can always be avoided
by the programmer (i.e. contract violations). Programming faults are not returned
as a runtime result, but jump to this point. This facilitates error handling
in normal situations, since programming errors don't need to be handled during
runtime.

progfault can be implemented by application code. If not, it defaults to to an
endless loop. The default implementation can be found in defaulthandlers.h */

void progfault(int error);

#endif
