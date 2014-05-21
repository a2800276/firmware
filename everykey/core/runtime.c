// -------------------------------------
// --- Missing runtime and ABI stuff ---
// -------------------------------------

/** exception unwinding */
//void __aeabi_unwind_cpp_pr0() { }

/** exception unwinding */
//void __aeabi_unwind_cpp_pr1() { }

/** hook for global scope destructors. We leave it empty, it's not going to be called anyway. */
void __aeabi_atexit() { }

/** handle for the dynamic shared object. We only have one. */
void* __dso_handle;
