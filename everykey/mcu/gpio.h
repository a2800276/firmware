#ifndef _GPIO_
#define _GPIO_

#include "../core/types.h"

typedef struct {
	HW_RW8 PIN[0x20];
} __attribute__((aligned(1))) GPIO_B_STRUCT;

typedef struct {
	HW_RW PIN[0x20];
} __attribute__((aligned(4))) GPIO_W_STRUCT;

typedef struct {
    HW_RW ISEL;
    HW_RW IENR;
    HW_WO SIENR;
    HW_WO CIENR;
    HW_RW IENF;
    HW_WO SIENF;
    HW_WO CIENF;
    HW_RW RISE;
    HW_RW FALL;
    HW_RW IST;
} __attribute__((aligned(4))) GPIO_INTERRUPT_STRUCT; 

#define GPIO_INTERRUPT_HW ((GPIO_INTERRUPT_STRUCT*)(0x40087000))

typedef struct {
	GPIO_B_STRUCT B[8];
	HW_UU unused1[0x3c0];		//skip from 0x0100 to 0x1000
	GPIO_W_STRUCT W[8];
	HW_UU unused2[0x300];		//skip from 0x1400 to 0x2000
	HW_RW DIR[8];
	HW_UU unused3[0x18];		//skip from 0x2020 to 0x2080
	HW_RW MASK[8];
	HW_UU unused4[0x18];		//skip from 0x20a0 to 0x2100
	HW_RW PIN[8];
	HW_UU unused5[0x18];		//skip from 0x2120 to 0x2180
	HW_RW MPIN[8];
	HW_UU unused6[0x18];		//skip from 0x21a0 to 0x2200
	HW_RW SET[8];
	HW_UU unused7[0x18];		//skip from 0x2220 to 0x2280
	HW_RW CLR[8];
	HW_UU unused8[0x18];		//skip from 0x22a0 to 0x2300
	HW_RW NOT[8];
} __attribute__((aligned(4))) GPIO_STRUCT;

#define GPIO ((GPIO_STRUCT*)(0x400f4000))

typedef enum {
	INPUT = 0,
	OUTPUT = 1
} PIN_DIR;

#ifdef __cplusplus
extern "C" {
#endif

void gpio_set_pin_dir(uint8_t port, uint8_t pin, PIN_DIR dir);

bool gpio_read_digital_pin(uint8_t port, uint8_t pin);

void gpio_write_digital_pin(uint8_t port, uint8_t pin, bool value);



#ifdef __cplusplus
}
#endif

#endif
