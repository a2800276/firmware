/* High level API for the sparrow board */

#include "sparrow.h"
#include "clocks.h"
#include "i2c.h"
#include "board.h"
#include "peripherals/tlv.h"

#define TLV_I2C_BUS 0


void sparrow_init() {
	//configure pins
	set_digital_output(BOOST_5V_EN_PIN);
	set_digital_output(PORT_5V_EN_PIN);
	set_digital_output(ETH_POWER_EN_PIN);
	set_digital_output(AUDIO_POWER_EN_PIN);
	set_digital_output(WIFI_POWER_EN_PIN );
	set_digital_output(AUDIO_NRESET);
	set_digital_output(LED1_PIN);
	set_digital_output(LED2_PIN);
	set_digital_output(LED3_PIN);
	set_digital_input(BUTTON_PIN, true, false, false);

	write_pin(BOOST_5V_EN_PIN, false);
	write_pin(PORT_5V_EN_PIN, false);
	write_pin(ETH_POWER_EN_PIN, false);
	write_pin(AUDIO_POWER_EN_PIN, false);
	write_pin(WIFI_POWER_EN_PIN, false );
	write_pin(AUDIO_NRESET, false);
	write_pin(LED1_PIN, false);
	write_pin(LED2_PIN, false);
	write_pin(LED3_PIN, false);

	i2c_configure_pin(I2C1_SDA_PIN_GROUP, I2C1_SDA_PIN_IDX, I2C1_SDA_PIN_MODE);
	i2c_configure_pin(I2C1_SCL_PIN_GROUP, I2C1_SCL_PIN_IDX, I2C1_SCL_PIN_MODE);

	i2s_configure_pin(I2S0_TX_MCLK_GROUP, I2S0_TX_MCLK_IDX, I2S0_TX_MCLK_MODE);
	i2s_configure_pin(I2S0_TX_WS_GROUP, I2S0_TX_WS_IDX, I2S0_TX_WS_MODE);
	i2s_configure_pin(I2S0_TX_SCK_GROUP, I2S0_TX_SCK_IDX, I2S0_TX_SCK_MODE);
	i2s_configure_pin(I2S0_TX_SD_GROUP, I2S0_TX_SD_IDX, I2S0_TX_SD_MODE);
	i2s_configure_pin(I2S0_RX_MCLK_GROUP, I2S0_RX_MCLK_IDX, I2S0_RX_MCLK_MODE);
	i2s_configure_pin(I2S0_RX_WS_GROUP, I2S0_RX_WS_IDX, I2S0_RX_WS_MODE);
	i2s_configure_pin(I2S0_RX_SCK_GROUP, I2S0_RX_SCK_IDX, I2S0_RX_SCK_MODE);
	i2s_configure_pin(I2S0_RX_SD_GROUP, I2S0_RX_SD_IDX, I2S0_RX_SD_MODE);

	//init base clock to xtal, main clock, via pll1
	clock_set_xtal_core_freq(MAIN_CLOCK_MHZ/XTAL_MHZ, 1);

	//set APB1 and APB 3 clocks to main clock, needed for I2C0 and I2C1
	clock_set_source(BASE_APB1_CLK, true, CLKSRC_PLL1);
	clock_set_source(BASE_APB3_CLK, true, CLKSRC_PLL1);

	//SPI init

	//I2C init
	i2c_init(0, I2C_MODE_FAST);
	i2c_init(1, I2C_MODE_FAST);


}


void audio_on() {
	audio_off();	//Turn off to make sure we do a proper reset
	delay(1000000);

	i2s_init(SPARROW_I2S);
	write_pin(AUDIO_POWER_EN_PIN, true);
	delay(1000000);		//TLV320AIC3100 spec: Reset low for at least 10ns
	write_pin(AUDIO_NRESET, true);
	delay(1000000);		//TLV320AIC3100 spec: 1ms min after reset
	tlv_sw_reset(TLV_I2C_BUS);
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
