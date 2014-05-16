/* High level API for the sparrow board. This abstraction sits on top of the standard library. Low level defines 
required by the generic library functions should go to board.h */

#ifndef _SPARROW_
#define _SPARROW_

#include "simpleio.h"

//reset all, disable all peripherals
void sparrow_init();

//Power on audio block, init codec
void audio_on();

//Power off audio block
void audio_off();



#endif