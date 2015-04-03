#include "everykey/everykey.h"
#include "animation.h"

void DEBUG(bool on) {	// THIS SHOULD GO LATER ****************
	write_pin(LED2_PIN, on);
}

#define NRF0_0_PIN P5_6_AS_GPIO_2_15    /* NXP TXD, NRF RXD */
#define NRF0_1_PIN P5_7_AS_GPIO_2_7     /* NXP RXD, NRF TXD */
#define NRF0_2_PIN P5_2_AS_GPIO_2_11    /*RTS*/
#define NRF0_3_PIN P5_4_AS_GPIO_2_13	/*CTS*/

//RX and TX seen from our side
#define NRF_UART_IDX 1
#define NRF_UART_TX_GROUP 5
#define NRF_UART_TX_IDX 6
#define NRF_UART_TX_MODE 4
#define NRF_UART_RX_GROUP 5
#define NRF_UART_RX_IDX 7
#define NRF_UART_RX_MODE 4

#define NUM_LEDS 58
#define MATRIX_WIDTH 10
#define MATRIX_HEIGHT 9

// map the LED stripe order to indexes in a 10x9 matrix 
const uint16_t stripe_led_lookup[] = {
	81,82,83,84,85,86,87,88,
	78,77,76,75,74,73,72,71,
	61,62,63,64,65,66,67,68,
	58,57,56,55,54,53,52,51,
	41,42,43,44,45,46,47,48,
	38,37,36,35,34,33,32,31,
	20,10, 0,24,25,14, 4, 29, 19, 9 };

uint8_t rgb[3*MATRIX_WIDTH*MATRIX_HEIGHT];
uint8_t led_buffer[LEDSTRIPE_BUFFER_CAPACITY(NUM_LEDS)];

//NRF reading - should go to ^^dkl ****
//NRF command format: 0x42 <datalen> <cmd> <data>
//datalen includes cmd


typedef enum {
	NRF_COMMAND_SET_COLOR = 1,	//data = r,g,b
	NRF_COMMAND_SET_PIXEL = 2,	//data = x,y,r,g,b
} NRF_COMMAND_CODE;

void nrf_handle_command(uint8_t* data, uint8_t len) {
	// static bool on = false;
	// on = !on;
	DEBUG(true);
	if (len < 1) return;
	switch (data[0]) {
		case NRF_COMMAND_SET_COLOR:
			if (len < 4) break;
			anim_fill_color(data[1],data[2],data[3]);
			break;
		case NRF_COMMAND_SET_PIXEL:
			if (len < 6) break;
			anim_set_pixel(data[1],data[2],data[3],data[4],data[5]);
			break;
		default:
			break;
	}
}

#define NRF_CMD_MAX_SIZE 20
#define NRF_MAGIC 0x42

typedef enum {
	NRF_WAITING_FOR_MAGIC,
	NRF_WAITING_FOR_LENGTH,
	NRF_COPYING_DATA
} NRF_RECEIVE_STATE;

static NRF_RECEIVE_STATE nrf_receive_state = NRF_WAITING_FOR_MAGIC;

void nrf_data_received(uint8_t uart_idx) {
	static uint8_t cmd_len = 0;
	static uint8_t cmd_idx = 0;
	static uint8_t cmd[NRF_CMD_MAX_SIZE];

	while (true) {
		uint8_t ch;
		uint8_t len = usart_read_avail(NRF_UART_IDX, &ch, 1);
		if (len < 1) break;
		switch (nrf_receive_state) {
			case NRF_WAITING_FOR_MAGIC:
				if (ch == NRF_MAGIC) {
					nrf_receive_state = NRF_WAITING_FOR_LENGTH;
				}
				break;
			case NRF_WAITING_FOR_LENGTH:
				if (ch > NRF_CMD_MAX_SIZE) {
					nrf_receive_state = NRF_WAITING_FOR_MAGIC;
				} else {
					cmd_len = ch;
					cmd_idx = 0;
					nrf_receive_state = NRF_COPYING_DATA;
				}
				break;
			case NRF_COPYING_DATA:
				cmd[cmd_idx++] = ch;
				if (cmd_idx == cmd_len) {
					nrf_handle_command(cmd,cmd_len);
					nrf_receive_state = NRF_WAITING_FOR_MAGIC;
				}
				break;
		}
	}
}

void nrf_comm_err(uint8_t uart_idx, uint8_t reason) {
	//Ignore command, flush read buffer
	usart_read_avail(NRF_UART_IDX, NULL, 16);
	nrf_receive_state = NRF_WAITING_FOR_MAGIC;
}

void main(void) {
	dkl_init();

	anim_init(MATRIX_WIDTH, MATRIX_HEIGHT, rgb);

	anim_fill_color(255,0,0);

	ledstripe_on(NUM_LEDS, led_buffer);

	set_digital_input(NRF0_3_PIN, true, false, true);

	usart_configure_pin(NRF_UART_RX_GROUP, NRF_UART_RX_IDX, NRF_UART_RX_MODE, true, false);
	usart_configure_pin(NRF_UART_TX_GROUP, NRF_UART_TX_IDX, NRF_UART_TX_MODE, false, false);

	//TODO: clock should be at 115200, but finding clock dividers doesnt seem to work
	usart_init_async(NRF_UART_IDX, 8, 115200, UART_PARITY_NONE, false, 128,
					USART_FCR_RXTRIGLVL_8, nrf_data_received, NULL,
					nrf_comm_err);

	systick_start(SYSTICK_10MS*3);
}

void systick() {
	anim_tick();
	ledstripe_sendRGB(rgb,stripe_led_lookup);

	write_pin(LED3_PIN, read_pin(NRF0_3_PIN));
}
