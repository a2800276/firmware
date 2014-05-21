/* Simple linker script for LPC1343 */

/* MEMORY describes the device's memory blocks and their addresses */

MEMORY {
  flash (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
/*  flash (rx)  : ORIGIN = 0x1a000000, LENGTH = 256K */
  ram   (rwx) : ORIGIN = 0x10000000, LENGTH =  32K
}

_LD_STACK_TOP = 0x10007ff0; /* Stack should start here */

/* SECTIONS lists where various parts data and code should go.
   Since they all just relate to the addressed defined above,
   porting code to other MCUs usually does not requre changes. */

SECTIONS {

    /* boot section at 0x00000000 */
    vectors : {
      KEEP(*(.vectors .vectors.*))        /* Must be first - we want this to be at 0. */
    } >flash

    init_array : {
      _LD_INIT_ARRAY_START = .;     /* remember constructors start position */
      KEEP(*(.init_array .init_array.*))
      _LD_INIT_ARRAY_END =   .;     /* remember constructors end position */
    } >flash

    /* .text section contains code and other readonly (const) stuff */
    text : {
        *(.text .text.*)
        *(.rodata .rodata.*)


        _LD_END_OF_TEXT = .;     /* Remember position (=start of RAM mirror in FLASH). Must be last entry in text. */
    } >flash

    /* data section contains read-write (non-const) globals and static variables
     * initialized to a value other than zero. The addresses are mapped to RAM
     * (the code needs to write to it), but initial values put to a mirror region
     * in FLASH (because we need to have the values at startup). Bootstrap code
     * will copy the values from FLASH to RAM. Note that bootstrap code wants this
     * section to follow the text section directly. */
    data : {
        _LD_START_OF_DATA = .;   /* Remember position (=start of initialized RAM) */
        *(.data .data.*);
        _LD_END_OF_DATA = .;     /* Remember position (=end of initialized RAM) */
    } >ram AT >flash

    /* bss section contains globals and static variables either initialized to zero
     * or not initialized at all. These are not mirrored in FLASH. Their memory
     * needs to be zeroed at startup. We'll clear it in the bootstrap code. Note that
     * bootstrap code wants this section to follow the data section directly. */
    bss : {
        _LD_START_OF_BSS = .;   /* Remember position (=start of uninitialized RAM) */
        *(.bss .bss.*)
        _LD_END_OF_BSS = .;     /* Remember position (=end of uninitialized RAM) */
    } > ram
}
