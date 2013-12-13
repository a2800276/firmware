#ifndef _GPIO_
#define _GPIO_

#include "types.h"

typedef struct {	// a pin group
	HW_RW SFS[20];			//20 pins per pin group
	HW_UU unused1[12];		//padding to make size = 0x80
} __attribute__((packed)) SCU_PIN_GROUP;

typedef struct {
	SCU_PIN_GROUP GROUP[0x10];	//pin group 0 to f
	HW_UU unused1[0x100];	//skip from 0x800 to 0xc00
	HW_RW SFSCLK[4];		//CLKn pins
	HW_UU unused2[0x1c];	//skip to 0xc80
	HW_RW SFSUSB;
	HW_RW SFSI2C0;
	HW_RW ENAI[3];
	HW_UU unused3[0x1b];	//skip to 0xd00
	HW_RW EMCDELAYCLK;
	HW_UU unused4[0x1f];	//skip to 0xd80
	HW_RW SDDELAY;
	HW_UU unused5[0x1f];	//skip to 0xe00
	HW_RW PINTSEL[2];
} __attribute__((packed)) SCU_STRUCT;

#define SCU ((SCU_STRUCT*)(0x40086000))

typedef struct {
	HW_RW8 PIN[0x20];
} __attribute__((packed)) GPIO_B_STRUCT;

typedef struct {
	HW_RW PIN[0x20];
} __attribute__((packed)) GPIO_W_STRUCT;

typedef struct {
	GPIO_B_STRUCT B[8];
	GPIO_W_STRUCT W[8];
	HW_UU unused1[0x300];		//skip from 0x1400 to 0x2000
	HW_RW DIR[8];
	HW_UU unused2[0x18];		//skip from 0x2020 to 0x2080
	HW_RW MASK[8];
	HW_UU unused3[0x18];		//skip from 0x20a0 to 0x2100
	HW_RW PIN[8];
	HW_UU unused4[0x18];		//skip from 0x2120 to 0x2180
	HW_RW MPIN[8];
	HW_UU unused5[0x18];		//skip from 0x21a0 to 0x2200
	HW_RW SET[8];
	HW_UU unused6[0x18];		//skip from 0x2220 to 0x2280
	HW_RW CLR[8];
	HW_UU unused7[0x18];		//skip from 0x22a0 to 0x2300
	HW_RW NOT[8];
} __attribute__((packed)) GPIO_STRUCT;

#define GPIO ((GPIO_STRUCT*)(0x400f4000))

/* set pin function. See table 169 in user manual (UM10430) */
void SetPinMode(uint8_t group, uint8_t idx, uint8_t mode);

void EnablePinPullup(uint8_t group, uint8_t idx, bool enable);

void EnablePinPulldown(uint8_t group, uint8_t idx, bool enable);

void SetPinSlewRate(uint8_t group, uint8_t idx, bool fast);

/** must be enabled for inputs to work */
void EnablePinInBuffer(uint8_t group, uint8_t idx, bool enable);

void EnablePinGlitchFilter(uint8_t group, uint8_t idx, bool enable);

/** strength: 0 (weakest) to 3 (strongest) - only on pins:
P1_17, P2_3, P2_4, P2_5, P8_0, P8_1, P8_2, PA_1, PA_2, PA_3 */
void SetPinDriveStrength(uint8_t group, uint8_t idx, uint8_t strength);

typedef enum {
	INPUT = 0,
	OUTPUT = 1
} PIN_DIR;

void SetPinDir(uint8_t port, uint8_t pin, PIN_DIR dir);

bool ReadDigitalPin(uint8_t port, uint8_t pin);

void WriteDigitalPin(uint8_t port, uint8_t pin, bool value);


#endif
