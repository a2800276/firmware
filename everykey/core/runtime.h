#ifndef _RUNTIME_H__
#define _RUNTIME_H_

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

void* memset(void* b, int c, size_t len);

void* memcpy(void* s1, const void* s2, size_t n);

#ifdef __cplusplus
}
#endif

#endif
