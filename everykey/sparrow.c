/* High level API for the sparrow board */

#include "board.h"
#include "sparrow.h"
#include "mcu/clocks.h"
#include "mcu/i2c.h"
#include "mcu/i2s.h"
#include "peripherals/tlv.h"
#include "utils/utils.h"

#define TLV_I2C_BUS 0


void sparrow_init() {

	//First: Turn off external power blocks
	set_digital_output(BOOST_5V_EN_PIN);
	set_digital_output(PORT_5V_EN_PIN);
	set_digital_output(ETH_POWER_EN_PIN);
	set_digital_output(AUDIO_POWER_EN_PIN);
	set_digital_output(WIFI_POWER_EN_PIN );
	set_digital_output(AUDIO_NRESET);
	write_pin(BOOST_5V_EN_PIN, false);
	write_pin(PORT_5V_EN_PIN, false);
	write_pin(ETH_POWER_EN_PIN, false);
	write_pin(AUDIO_POWER_EN_PIN, false);
	write_pin(WIFI_POWER_EN_PIN, false );
	write_pin(AUDIO_NRESET, false);

	//init base clock to xtal, main clock, via pll1
	clock_set_xtal_core_freq(MAIN_CLOCK_MHZ/XTAL_MHZ, 1);

	//enable derived base clocks for shared use. Dedicated clocks are enabled by the
	//respective peripheral interface
	clock_set_source(BASE_APB1_CLK, true, CLKSRC_PLL1);    //I2C0
	clock_set_source(BASE_APB3_CLK, true, CLKSRC_PLL1);    //I2C1

	//GPIO
	set_digital_output(LED1_PIN);
	set_digital_output(LED2_PIN);
	set_digital_output(LED3_PIN);
	set_digital_input(BUTTON_PIN, true, false, false);
	write_pin(LED1_PIN, false);
	write_pin(LED2_PIN, false);
	write_pin(LED3_PIN, false);

	//I2C
	i2c_configure_pin(I2C1_SDA_PIN_GROUP, I2C1_SDA_PIN_IDX, I2C1_SDA_PIN_MODE);
	i2c_configure_pin(I2C1_SCL_PIN_GROUP, I2C1_SCL_PIN_IDX, I2C1_SCL_PIN_MODE);
	i2c_init(0, I2C_MODE_FAST);
	i2c_init(1, I2C_MODE_FAST);
	//TODO: I2C1 might also be used as GPIOs

	//I2S
	i2s_configure_pin(I2S0_TX_MCLK_GROUP, I2S0_TX_MCLK_IDX, I2S0_TX_MCLK_MODE);
	i2s_configure_pin(I2S0_TX_WS_GROUP, I2S0_TX_WS_IDX, I2S0_TX_WS_MODE);
	i2s_configure_pin(I2S0_TX_SCK_GROUP, I2S0_TX_SCK_IDX, I2S0_TX_SCK_MODE);
	i2s_configure_pin(I2S0_TX_SD_GROUP, I2S0_TX_SD_IDX, I2S0_TX_SD_MODE);
	//i2s_configure_pin(I2S0_RX_MCLK_GROUP, I2S0_RX_MCLK_IDX, I2S0_RX_MCLK_MODE);
	//i2s_configure_pin(I2S0_RX_WS_GROUP, I2S0_RX_WS_IDX, I2S0_RX_WS_MODE);
	//i2s_configure_pin(I2S0_RX_SCK_GROUP, I2S0_RX_SCK_IDX, I2S0_RX_SCK_MODE);
	//i2s_configure_pin(I2S0_RX_SD_GROUP, I2S0_RX_SD_IDX, I2S0_RX_SD_MODE);

	//SD
	scu_set_pin_mode(SD_CD_GROUP, SD_CD_PIN, SD_CD_MODE);
	scu_enable_pin_in_buffer(SD_CD_GROUP, SD_CD_PIN, true);
	scu_set_pin_mode(SD_WP_GROUP, SD_WP_PIN, SD_WP_MODE);
	//TODO: configure WP pin
	scu_set_pin_mode(SD_CMD_GROUP, SD_CMD_PIN, SD_CMD_MODE);
	scu_set_pin_mode(SD_D0_GROUP, SD_D0_PIN, SD_D0_MODE);
	scu_set_pin_mode(SD_D1_GROUP, SD_D1_PIN, SD_D1_MODE);
	scu_set_pin_mode(SD_D2_GROUP, SD_D2_PIN, SD_D2_MODE);
	scu_set_pin_mode(SD_D3_GROUP, SD_D3_PIN, SD_D3_MODE);
	scu_set_clock_pin_mode(SD_CLK_CLK, SD_CLK_MODE, false, false, true, false, false);



}


void audio_on() {
	audio_off();	//Turn off to make sure we do a proper reset
	delay(1000000);

	i2s_init(SPARROW_I2S);
	write_pin(AUDIO_POWER_EN_PIN, true);
	delay(1000000);		//TLV320AIC3100 spec: Reset low for at least 10ns
	write_pin(AUDIO_NRESET, true);
	delay(1000000);		//TLV320AIC3100 spec: 1ms min after reset
}

void audio_off() {
	write_pin(AUDIO_POWER_EN_PIN, false);
	write_pin(AUDIO_NRESET, false);
	i2s_shutdown(SPARROW_I2S);
}

void audio_play(uint8_t numChannels, uint8_t bitsPerSample, uint32_t sampleRate) {
  i2s_start_play(SPARROW_I2S, numChannels, bitsPerSample, sampleRate);
  tlv_configure(TLV_I2C_BUS);

	// tlv_set_clock_input(TLV_I2C_BUS);
	// tlv_set_clock_src_bclk(TLV_I2C_BUS);
	// tlv_set_dac_power(TLV_I2C_BUS, true, true, false);
	// tlv_set_dac_volume(TLV_I2C_BUS, 128);
}
