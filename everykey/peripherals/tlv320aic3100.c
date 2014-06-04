#include "tlv320aic3100.h"
#include "../board.h"

#include "../mcu/i2c.h"
#include "../mcu/gpio.h"

#include "../utils/simpleio.h"
#include "../utils/progfault.h"
#include "../utils/utils.h"

#define TLV_I2C_SLAVE_ADDRESS 0x18

#define TLV_CONTROL_PAGE 0

#define TLV_RESET_REG 1
#define TLV_CLOCK_MUX 4
#define TLV_DAC_FLAG_REG 37
#define TLV_DAC_VOL_CTRL_REG 64
#define TLV_DAC_LEFT_VOL_REG 65
#define TLV_DAC_RIGHT_VOL_REG 66

void tlv_select_page(uint8_t i2c, uint8_t page) {
	uint8_t selectpage[2] = {0,page};
	I2C_STATUS status = i2c_write_sync(i2c, TLV_I2C_SLAVE_ADDRESS, 2, selectpage);
	if (status != I2C_STATUS_OK) progfault(PERIPHERAL_FAILURE);
}

void tlv_write_register (uint8_t i2c, uint8_t page, uint8_t reg, uint8_t value) {
	//TODO: We don't need to set the page each time, could remember the page and change it only if necessary *********
//	uint8_t selectpage[2] = {0,page};
//	I2C_STATUS status = i2c_write_sync(i2c, TLV_I2C_SLAVE_ADDRESS, 2, selectpage);
//	if (status != I2C_STATUS_OK) progfault(PERIPHERAL_FAILURE);

	uint8_t writevalue[2] = {reg, value};
	I2C_STATUS status = i2c_write_sync(i2c, TLV_I2C_SLAVE_ADDRESS, 2, writevalue);
	if (status != I2C_STATUS_OK) progfault(PERIPHERAL_FAILURE);
}

void tlv_write_registers (uint8_t i2c, uint8_t page, uint8_t firstReg, uint8_t numRegs, uint8_t* values) {
//	uint8_t selectpage[2] = {0,page};
//	I2C_STATUS status = i2c_write_sync(i2c, TLV_I2C_SLAVE_ADDRESS, 2, selectpage);
//	if (status != I2C_STATUS_OK) progfault(PERIPHERAL_FAILURE);

	uint8_t writevalues[numRegs+1];
	writevalues[0] = firstReg;
	uint8_t i;
	for (i=0; i<numRegs; i++) writevalues[i+1] = values[i];
	I2C_STATUS status = i2c_write_sync(i2c, TLV_I2C_SLAVE_ADDRESS, numRegs+1, writevalues);
	if (status != I2C_STATUS_OK) progfault(PERIPHERAL_FAILURE);
}

void tlv_configure(uint8_t i2c) {

	tlv_select_page(i2c, 0);
	tlv_write_register(i2c, TLV_CONTROL_PAGE, TLV_RESET_REG, 1);
	delay(1000000);

	//Setting for 48kHz, 16 bit, stereo, I2C, clocks per channel: 16, BCLK as input

	//Note that several values are hard coded for now

	//Set stream format
	tlv_write_register(i2c, 0, 27, 0x00); //CODEC_INTF_CTRL: I2S, 16 bits, slave mode

	//Init PLL, enable with last command
	//PLL on, PLLP=1, PLLR=4, PLLJ=16, PLLD=0, MDAC = 8, NDAC = 2, DOSR=128
	tlv_select_page(i2c, 0);
  tlv_write_register(i2c, 0,  4, 0x07); //CLK_GEN_MUX:     PLL_XLKIN=BCLK CODEC_CLKIN = PLL_CLK
  tlv_write_register(i2c, 0,  6, 0x10); //PLL_J:           PLLJ = 16 (D5-0:J5-0)
  uint8_t plldConf[] = { 0x00, 0x00};
	tlv_write_registers(i2c, 0, 7, 8, plldConf); //PLL_D:    PLLD = 0
  tlv_write_register(i2c, 0,  5, 0x94); //PLL_P_R_VAL:     PLL on, PLLP=1, PLLR=4 (B7:on,B6-4:P2-0,B3-0:R3-0)

	//Init DAC clocks and DOSR
	uint8_t dacClockConf[] = { 0x88, 0x82, 0x00, 0x80};
	tlv_write_registers(i2c, 0, 11, 4, dacClockConf);	//NDAC=8, MDAC=2, DOSR = 128


	//Select and configure processing block
  tlv_write_register(i2c, 0, 60, 0x0b); //DAC_INST_SET:    DAC PRB_P11 (IIR -> 6xBiquad -> Interp B)
  tlv_select_page(i2c, 8);
	tlv_write_register(i2c, 8,  1, 0x04); //DAC_COEFF_BUF_CTRL: Enable adaptive filtering

	//Misc page 0 controls
	tlv_select_page(i2c, 0);
	tlv_write_register(i2c, 0,116, 0x00); //VOL_CTRL_PIN:    Volume control pin disable
	tlv_write_register(i2c, 0, 68, 0x00); //DRC_CTRL1:       DRC disable

	//Analog block
	tlv_select_page(i2c, 1);
	tlv_write_register(i2c, 1, 31, 0x04); //HP_DRIVER:       HP driver off, common mode voltage = 1.35V
	tlv_write_register(i2c, 1, 33, 0x3e/*0x4e*/); //HPOUT_POP:       Parrallel powerdown, rampup 1.22s, rampdown 3.9ms
	//tlv_write_register(i2c, 1, 35, 0x88); //DACLR_MIX_ROUTE: DACL to HPL, DACR to HPR, other features off
	tlv_write_register(i2c, 1, 35, 0x44); //DACLR_MIX_ROUTE: DACL to MIXL, DACR to MIXR, other features off
	//should be 0x88 but try 35->0x44 ******
	tlv_write_register(i2c, 1, 40, 0x06); //HPL_DRIVER:      HPL gain 0dB, HPL not muted, high imp in PD, not done yet
	tlv_write_register(i2c, 1, 41, 0x06); //HPR_DRIVER:      HPR gain 0dB, HPR not muted, high imp in PD, not done yet
	tlv_write_register(i2c, 1, 42, 0x1c); //SPK_DRIVER:      Speaker gain 18 dB, not muted, not done

	//Power up output drivers
	tlv_write_register(i2c, 1, 31, 0xC4); //HP_DRIVER:       HP driver on, common mode voltage = 1.35V
	//should be c4 but try 31->0xC2
	tlv_write_register(i2c, 1, 32, 0x86); //SPK_AMP:         Speaker driver power on
	//TRY 31->0xC6
	tlv_write_register(i2c, 1, 36, 0x00); //HPL_ANALOG_GAIN: Don't route analog volume to driver, 0dB
	tlv_write_register(i2c, 1, 37, 0x00); //HPR_ANALOG_GAIN: Don't route analog volume to driver, 0dB
	tlv_write_register(i2c, 1, 38, 0x00); //SPK_ANALOG_GAIN: Don't route analog volume to driver, 0dB

	delay(10000000);	//Wait a bit until depop and stuff has settled

	//Turn on DAC left and right channels, set gain
	tlv_select_page(i2c, 0);
	tlv_write_register(i2c, 0, 63, 0xd4); //DAC_DATA_PATH:   LDAC on, RDAC on, Left = left, right = right, soft-stepping
	tlv_write_register(i2c, 0, 64, 0x00); //DAC_VOL_CTRL:    LDAC not muted, RDAC not muted, LR vol independent
	uint8_t volume[] = { 0x00, 0x00};
	tlv_write_registers(i2c, 0, 65, 2, volume);
}
