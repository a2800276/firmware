//System control unit (SCU)

#ifndef _SCU_
#define _SCU_

#include "../core/types.h"

typedef struct {	// a pin group
	HW_RW SFS[20];			//20 pins per pin group
	HW_UU unused1[12];		//padding to make size = 0x80
} __attribute__((aligned(4))) SCU_PIN_GROUP;

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
} __attribute__((aligned(4))) SCU_STRUCT;

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

/** convenience for common pin settings */
void scu_set_pin(uint8_t group, uint8_t idx, uint8_t mode, bool pullup, bool pulldown, bool hs_slew, bool in_buffer, bool glitch_filter);

/** strength: 0 (weakest) to 3 (strongest) - only on pins:
P1_17, P2_3, P2_4, P2_5, P8_0, P8_1, P8_2, PA_1, PA_2, PA_3 */
void scu_set_pin_drive_strength(uint8_t group, uint8_t idx, uint8_t strength);

/** Sets the mode of the dedicated i2c0 pins
@param enable true to turn on, false to turn off
@param fastplus true for fast mode plus (1mbps), false otherwise
@param true to enable glitch filter (both lines)
@param true to enable long glicth filter (50ns instead of 3ns), only useful if glitch=true */
void scu_set_i2c0_pinmode(bool enable, bool fastplus, bool glitch, bool longglitch);

/** sets the mode of the dedicated clock pins
@param idx clock pin idx (0-3)
@param func pin function
@param pd true to enable pulldown
@param pu true to enable pullup
@param hs true to enable high speed
@param buf true to enable input buffer
@param glitch true to enable glitch filter */
void scu_set_clock_pin_mode(uint8_t idx, uint8_t func, bool pd, bool pu, bool hs, bool buf, bool glitch);

typedef enum {
	PIN_INTERRUPT_NONE          = 0,
    PIN_INTERRUPT_RISING        = 1,
    PIN_INTERRUPT_FALLING       = 2,
    PIN_INTERRUPT_RISINGFALLING = 3,
    PIN_INTERRUPT_HIGH          = 4,
    PIN_INTERRUPT_LOW           = 5
} PIN_INTERRUPT_MODE;

/** enables one of eight pin-specific interrupt registers
@param idx pin interrupt index (0-7) 
@param port gpio port
@param pin gpio pin
@param mode trigger mode to set */
void scu_enable_pin_interrupt(uint8_t idx, uint8_t port, uint8_t pin, PIN_INTERRUPT_MODE mode);

/** disables one of eight pin-specific interrupt registers
@param idx pin interrupt index (0-7)  */
void scu_disable_pin_interrupt(uint8_t idx);

/** clears rising and falling edge detection for edge-sensitive pins. For level-sensitive pins,
this toggles the sensed level (LOW to HIGH, HIGH to LOW)
@param idx pin interrupt index (0-7) */
void scu_clear_edge_pin_interrupt(uint8_t idx);

#ifdef __cplusplus
}
#endif

#endif
