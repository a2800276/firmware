/* High level API for the sparrow board */

#include "sparrow.h"
#include "clocks.h"
#include "board.h"


void sparrow_init() {
	set_digital_output(BOOST_5V_EN_PIN);
	set_digital_output(PORT_5V_EN_PIN);
	set_digital_output(ETH_POWER_EN_PIN);
	set_digital_output(AUDIO_POWER_EN_PIN);
	set_digital_output(WIFI_POWER_EN_PIN );
	set_digital_output(AUDIO_NRESET);
	set_digital_output(LED1_PIN);
	set_digital_output(LED2_PIN);
	set_digital_output(LED3_PIN);

	write_pin(BOOST_5V_EN_PIN, false);
	write_pin(PORT_5V_EN_PIN, false);
	write_pin(ETH_POWER_EN_PIN, false);
	write_pin(AUDIO_POWER_EN_PIN, false);
	write_pin(WIFI_POWER_EN_PIN, false );
	write_pin(AUDIO_NRESET, false);
	write_pin(LED1_PIN, false);
	write_pin(LED2_PIN, false);
	write_pin(LED3_PIN, false);

	set_digital_input(BUTTON_PIN, true, false, false);

	//init base clock to xtal, main clock, via pll1
	clock_set_xtal_core_freq(MAIN_CLOCK_MHZ/XTAL_MHZ, 1);

	//set APB1 and APB 3 clocks to main clock, needed for I2C0 and I2C1
	clock_set_source(BASE_APB1_CLK, true, CLKSRC_PLL1);
	clock_set_source(BASE_APB3_CLK, true, CLKSRC_PLL1);

	//SPI init
	//I2C init
}


void audio_on() {
	audio_off();	//Turn off to make sure we do a proper reset
	delay(1000000);
	write_pin(AUDIO_POWER_EN_PIN, true);
	delay(1000000);
	write_pin(AUDIO_NRESET, true);
	//TODO: tlv320aic3100 startup sequence
}

void audio_off() {
	write_pin(AUDIO_POWER_EN_PIN, false);
	write_pin(AUDIO_NRESET, false);
}