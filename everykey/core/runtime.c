// -------------------------------------
// --- Missing runtime and ABI stuff ---
// -------------------------------------

#include "types.h"

/** exception unwinding */
//void __aeabi_unwind_cpp_pr0() { }

/** exception unwinding */
//void __aeabi_unwind_cpp_pr1() { }

/** hook for global scope destructors. We leave it empty, it's not going to be called anyway. */
void __aeabi_atexit() { }

/** handle for the dynamic shared object. We only have one. */
void* __dso_handle;

/** stupid implementations of builtins */
void* memset(void* b, int c, size_t len) {
  uint8_t* base = (uint8_t*)b;
  size_t i;
  for (i=0; i<len; i++) {
    base[i] = c;
  }
  return b;
}

void* memcpy(void* s1, const void* s2, size_t n) {
  uint8_t* to = (uint8_t*)s1;
  uint8_t* from = (uint8_t*)s2;
  size_t i;
  for (i=0; i<n; i++) to[i] = from[i];
  return s1;
}
