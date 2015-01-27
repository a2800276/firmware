/* High level API for the sparrow board */

#include "board.h"
#include "sparrow.h"
#include "mcu/clocks.h"
#include "mcu/i2c.h"
#include "mcu/i2s.h"
#include "mcu/usart.h"
#include "mcu/ssp.h"
#include "peripherals/tlv320aic3100.h"
#include "peripherals/lan8720a.h"
#include "utils/utils.h"

#define TLV_I2C_BUS 0


void sparrow_init() {

	//First: Turn off external power blocks
	set_digital_output(BOOST_5V_EN_PIN);
	set_digital_output(PORT_5V_EN_PIN);

//TODO: Why does this pin not work? For now, the input pullup keeps the eth domain powered up permamently.
//Setting OUT and writing this bit caused the device to hang. Might be correct now, check again.
//	set_digital_output(ETH_POWER_EN_PIN);

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

	//SPI
	scu_set_pin(SPI0_MISO_GROUP, SPI0_MISO_IDX, SPI0_MISO_MODE, false, false, true, true, true);
	scu_set_pin(SPI0_MOSI_GROUP, SPI0_MOSI_IDX, SPI0_MOSI_MODE, false, false, true, false, true);
	scu_set_pin(SPI0_SCK_GROUP, SPI0_SCK_IDX, SPI0_SCK_MODE, false, false, true, false, true);
	scu_set_pin(SPI0_SSEL_GROUP, SPI0_SSEL_IDX, SPI0_SSEL_MODE, false, false, true, false, true);
	
	scu_set_pin(SPI1_MISO_GROUP, SPI1_MISO_IDX, SPI1_MISO_MODE, false, false, true, true, true);
	scu_set_pin(SPI1_MOSI_GROUP, SPI1_MOSI_IDX, SPI1_MOSI_MODE, false, false, true, false, true);
	scu_set_pin(SPI1_SCK_GROUP, SPI1_SCK_IDX, SPI1_SCK_MODE, false, false, true, false, true);
	scu_set_pin(SPI1_SSEL_GROUP, SPI1_SSEL_IDX, SPI1_SSEL_MODE, false, false, true, false, true);



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
	scu_set_pin_pullup(SD_CMD_GROUP, SD_CMD_PIN, true);
	scu_enable_pin_in_buffer(SD_CMD_GROUP, SD_CMD_PIN, true);
	scu_set_pin_mode(SD_D0_GROUP, SD_D0_PIN, SD_D0_MODE);
	scu_set_pin_pullup(SD_D0_GROUP, SD_D0_PIN, true);
	scu_enable_pin_in_buffer(SD_D0_GROUP, SD_D0_PIN, true);
	scu_set_pin_mode(SD_D1_GROUP, SD_D1_PIN, SD_D1_MODE);
	scu_set_pin_pullup(SD_D1_GROUP, SD_D1_PIN, true);
	scu_enable_pin_in_buffer(SD_D1_GROUP, SD_D1_PIN, true);
	scu_set_pin_mode(SD_D2_GROUP, SD_D2_PIN, SD_D2_MODE);
	scu_set_pin_pullup(SD_D2_GROUP, SD_D2_PIN, true);
	scu_enable_pin_in_buffer(SD_D2_GROUP, SD_D2_PIN, true);
	scu_set_pin_mode(SD_D3_GROUP, SD_D3_PIN, SD_D3_MODE);
	scu_set_pin_pullup(SD_D3_GROUP, SD_D3_PIN, true);
	scu_enable_pin_in_buffer(SD_D3_GROUP, SD_D3_PIN, true);
	scu_set_clock_pin_mode(SD_CLK_CLK, SD_CLK_MODE, false, false, true, false, false);


	//Ethernet RMII
	creg_init();
	creg_set_eth_interface(true);
	scu_set_clock_pin_mode(ETH_CLK_CLK, ETH_CLK_MODE, false, false, true, true, false);
	scu_set_pin(ETH_RXD0_GROUP,   ETH_RXD0_PIN,   ETH_RXD0_MODE,   false, false, true, true,  false);
	scu_set_pin(ETH_RXD1_GROUP,   ETH_RXD1_PIN,   ETH_RXD1_MODE,   false, false, true, true,  false);
	scu_set_pin(ETH_TXD0_GROUP,   ETH_TXD0_PIN,   ETH_TXD0_MODE,   false, false, true, false, false);
	scu_set_pin(ETH_TXD1_GROUP,   ETH_TXD1_PIN,   ETH_TXD1_MODE,   false, false, true, false, false);
	scu_set_pin(ETH_MDC_GROUP,    ETH_MDC_PIN,    ETH_MDC_MODE,    false, false, true, false, false);
	scu_set_pin(ETH_TXEN_GROUP,   ETH_TXEN_PIN,   ETH_TXEN_MODE,   false, false, true, false, false);
	scu_set_pin(ETH_CRS_DV_GROUP, ETH_CRS_DV_PIN, ETH_CRS_DV_MODE, false, false, true, true,  false);
	scu_set_pin(ETH_MDIO_GROUP,   ETH_MDIO_PIN,   ETH_MDIO_MODE,   false, false, true, true,  false);
	set_digital_input(ETH_NINT_PIN, true, false, true);
	set_digital_output(ETH_NRST_PIN);
	write_pin(ETH_NRST_PIN, false);	//put into reset

	//CC3000 WIFI
	set_digital_output(CC3000_SW_EN_PIN);
	write_pin(CC3000_SW_EN_PIN, false);
	set_digital_input(CC3000_IRQ_PIN, false, false, true);

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

void audio_play(uint8_t numChannels, uint8_t bitsPerSample, uint32_t sampleRate, I2S_PLAY_SAMPLE_CALLBACK cb) {
	i2s_start_play(SPARROW_I2S, numChannels, bitsPerSample, sampleRate, cb);
  	tlv_configure(TLV_I2C_BUS);

	// tlv_set_clock_input(TLV_I2C_BUS);
	// tlv_set_clock_src_bclk(TLV_I2C_BUS);
	// tlv_set_dac_power(TLV_I2C_BUS, true, true, false);
	// tlv_set_dac_volume(TLV_I2C_BUS, 128);
}

void ethernet_on() {
	ethernet_off();	//turn off for a proper reset
	delay(10*DELAY_MS_UNITS);
//	set_digital_input(ETH_POWER_EN_PIN, true, false, false);
//	write_pin(ETH_POWER_EN_PIN, true);
	delay(10*DELAY_MS_UNITS);
	write_pin(ETH_NRST_PIN, true);
	delay(10*DELAY_MS_UNITS);
	eth_init();
  lan_init();

}

void ethernet_off() {
	eth_shutdown();
//	write_pin(ETH_POWER_EN_PIN, false);
	write_pin(ETH_NRST_PIN, false);
}

/* WS2812B interface:
LEDs are daisy-chained. After a reset (data low for at least 50us),
color values are transferred. Each LED consumes 24 bits (8 bits G,R,B each, MSB first)
and forwards all remaining bits to the next LED.
A 0 is encoded as a high pulse (350ns+-150ns) and a low pulse (900ns+-150ns)
A 1 is encoded as a high pulse (900ns+-150ns) and a low pulse (350ns+-150ns)

A bit clock of 312.5ns (3.2MBaud) with 4 uart bits per LED bit come close to these values, well within tolerances.

We operate in synchronous master mode (to disable start and stop bits),
but don't use the clock pin. Each LED bit is encoded into 4 UART bits.
We encode 2 LED bits into one UART character, resulting in a character length of 6 bits.

LPC sends LSB first, resulting in the following encoding:
LED bits		UART character
00					00010001
01					01110001
10					00010111
11					01110111
*/

static uint8_t* ledstripe_buffer = NULL;
static uint16_t ledstripe_idx = 0;
static uint16_t ledstripe_buffer_len;

void DEBUG(bool on);

void ledstripe_refill(uint8_t idx) {
	if (!ledstripe_buffer) return;
	int16_t remaining = ledstripe_buffer_len-ledstripe_idx;
	if (remaining < 1) return;
	uint16_t sent = usart_write_avail(LEDSTRIPE_USART_IDX, ledstripe_buffer+ledstripe_idx, remaining);
	ledstripe_idx += sent;
}

void ledstripe_on(uint16_t num_leds, uint8_t* in_ledstripe_buffer) {
	ledstripe_buffer = in_ledstripe_buffer;
	ledstripe_buffer_len = LEDSTRIPE_BUFFER_CAPACITY(num_leds);
	ledstripe_idx = ledstripe_buffer_len;
	usart_configure_pin(USART0_TXD_GROUP, USART0_TXD_IDX, USART0_TXD_MODE, false, true);
	usart_configure_pin(USART0_RXD_GROUP, USART0_RXD_IDX, USART0_RXD_MODE, true, true);
	usart_configure_pin(USART0_UCLK_GROUP, USART0_UCLK_IDX, USART0_UCLK_MODE, false, true);
	write_pin(PORT_5V_EN_PIN, true);
	usart_init_sync_master(LEDSTRIPE_USART_IDX, 8, 3200000, 16, USART_FCR_RXTRIGLVL_8, NULL, NULL/*ledstripe_refill*/, NULL);
}

void ledstripe_off() {
	write_pin(PORT_5V_EN_PIN, false);
	ledstripe_buffer = NULL;
	ledstripe_buffer_len = 0;
}

void ledstripe_sendRGB(const uint8_t* rgb, const uint16_t* reorder) {
	if (!ledstripe_buffer) return;
	uint8_t lookup[] = {0x11, 0x71, 0x17, 0x77};
	uint16_t num_leds = LEDSTRIPE_LED_COUNT(ledstripe_buffer_len);
	uint16_t i;
	for (i = 0; i < LEDSTRIPE_BREAK_SIZE; i++) {
		ledstripe_buffer[i] = 0;	
	}
	for (i=0; i < num_leds; i++) {
		uint16_t pixIdx = (reorder) ? reorder[i] : i;
		uint8_t r = rgb[3*pixIdx];
		uint8_t g = rgb[3*pixIdx+1];
		uint8_t b = rgb[3*pixIdx+2];
		ledstripe_buffer[LEDSTRIPE_BREAK_SIZE + 12*i+ 0] = lookup[(g >> 6) & 0x03];
		ledstripe_buffer[LEDSTRIPE_BREAK_SIZE + 12*i+ 1] = lookup[(g >> 4) & 0x03];
		ledstripe_buffer[LEDSTRIPE_BREAK_SIZE + 12*i+ 2] = lookup[(g >> 2) & 0x03];
		ledstripe_buffer[LEDSTRIPE_BREAK_SIZE + 12*i+ 3] = lookup[(g     ) & 0x03];
		ledstripe_buffer[LEDSTRIPE_BREAK_SIZE + 12*i+ 4] = lookup[(r >> 6) & 0x03];
		ledstripe_buffer[LEDSTRIPE_BREAK_SIZE + 12*i+ 5] = lookup[(r >> 4) & 0x03];
		ledstripe_buffer[LEDSTRIPE_BREAK_SIZE + 12*i+ 6] = lookup[(r >> 2) & 0x03];
		ledstripe_buffer[LEDSTRIPE_BREAK_SIZE + 12*i+ 7] = lookup[(r     ) & 0x03];
		ledstripe_buffer[LEDSTRIPE_BREAK_SIZE + 12*i+ 8] = lookup[(b >> 6) & 0x03];
		ledstripe_buffer[LEDSTRIPE_BREAK_SIZE + 12*i+ 9] = lookup[(b >> 4) & 0x03];
		ledstripe_buffer[LEDSTRIPE_BREAK_SIZE + 12*i+10] = lookup[(b >> 2) & 0x03];
		ledstripe_buffer[LEDSTRIPE_BREAK_SIZE + 12*i+11] = lookup[(b     ) & 0x03];
	}

	ledstripe_idx = 0;
	usart_write_sync(LEDSTRIPE_USART_IDX, ledstripe_buffer, ledstripe_buffer_len);

//	ledstripe_refill(LEDSTRIPE_USART_IDX);
}

void wifi_on() {
	write_pin(CC3000_SW_EN_PIN, false);
	delay(10000000);	
	write_pin(WIFI_POWER_EN_PIN, true);
	ssp_init(CC3000_SPI_HW, SSP_FRAMEFORMAT_SPI, 0, true, 8, 20);
	write_pin(CC3000_SW_EN_PIN, true);
	cc3000_init(CC3000_IRQ_PIN);
}

void wifi_off() {
	wifi_shutdown();
	write_pin(CC3000_SW_EN_PIN, false);
	write_pin(WIFI_POWER_EN_PIN, false);

}



