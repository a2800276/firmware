//System control unit (SCU)

#ifndef _SCU_
#define _SCU_

#include "../core/types.h"

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

#ifdef __cplusplus
extern "C" {
#endif

/* set pin function. See table 169 in user manual (UM10430) */
void scu_set_pin_mode(uint8_t group, uint8_t idx, uint8_t mode);

void scu_set_pin_pullup(uint8_t group, uint8_t idx, bool enable);

void scu_set_pin_pulldown(uint8_t group, uint8_t idx, bool enable);

void scu_set_pin_slew_rate(uint8_t group, uint8_t idx, bool fast);

/** must be enabled for inputs to work */
void scu_enable_pin_in_buffer(uint8_t group, uint8_t idx, bool enable);

void scu_enable_pin_glitch_filter(uint8_t group, uint8_t idx, bool enable);

/** strength: 0 (weakest) to 3 (strongest) - only on pins:
P1_17, P2_3, P2_4, P2_5, P8_0, P8_1, P8_2, PA_1, PA_2, PA_3 */
void scu_set_pin_drive_strength(uint8_t group, uint8_t idx, uint8_t strength);

/** Sets the i2c0 pins mode
@param enable true to turn on, false to turn off
@param fastplus true for fast mode plus (1mbps), false otherwise
@param true to enable glitch filter (both lines)
@param true to enable long glicth filter (50ns instead of 3ns), only useful if glitch=true */
void scu_set_i2c0_pinmode(bool enable, bool fastplus, bool glitch, bool longglitch);

#ifdef __cplusplus
}
#endif

#endif
