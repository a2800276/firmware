#include "everykey/everykey.h"
#include "animation.h"

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

#define GAME_MAX_SEQUENCE_LENGTH 500
#define GAME_MAX_LISTEN_TICKS 6000	/* 1 min */
#define GAME_DEFAULT_TONE_TICKS 50
#define GAME_DEFAULT_SAY_TICKS 70
#define GAME_LENGTH_FINISHED_FEEDBACK_TICKS 70
#define GAME_LENGTH_FINISHED_CONTINUE_TICKS 180


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

// ----------------------
// --- audio settings ---
// ----------------------

#define AUDIO_BUFFER_SIZE 2048
#define HALF_AUDIO_BUFFER_SIZE 1024
#define AUDIO_BLOCKS_PER_S 86
#define AUDIO_BLOCKS_OFFSET 10
#define AUDIO_BLOCKS_PER_FX 172
#define AUDIO_FX_DURATION_S 1
#define AUDIO_NUM_FX 8

SD_CARD_INFO card_info;

static uint32_t audio_buffer[AUDIO_BUFFER_SIZE];
static volatile uint32_t audio_playbackIdx = 0;
static volatile uint32_t audio_readBlockIdx = 10;
static volatile uint32_t audio_readBlockEndIdx = 0;
static volatile uint8_t audio_lastFilledHalfBuffer = 1;
static volatile uint8_t audio_playingHalfBuffer = 0;
static volatile bool audio_playing = false;

void audio_tick() {
	audio_playingHalfBuffer = audio_playbackIdx / HALF_AUDIO_BUFFER_SIZE;
	if (audio_lastFilledHalfBuffer == audio_playingHalfBuffer) {
		uint8_t bufferToFill = 1-audio_lastFilledHalfBuffer;
		uint32_t* base = (&(audio_buffer[HALF_AUDIO_BUFFER_SIZE*bufferToFill]));
		if (audio_readBlockIdx >= audio_readBlockEndIdx) {
			audio_playing = false;
		}
		if (audio_playing) {
			bool read = sd_read_blocks(base, audio_readBlockIdx, 8, &card_info, true);
			audio_readBlockIdx+=8;
		} else {
			int i;
			for (i=0; i<HALF_AUDIO_BUFFER_SIZE; i++) {
				base[i] = 0;
			}
		}
		audio_lastFilledHalfBuffer = bufferToFill;
	}
}

void audio_play_effect(uint8_t idx) {
	audio_playing = false;
	audio_readBlockIdx = AUDIO_BLOCKS_PER_FX * idx + AUDIO_BLOCKS_OFFSET;
	audio_readBlockEndIdx = audio_readBlockIdx + AUDIO_FX_DURATION_S * AUDIO_BLOCKS_PER_S;
	audio_playing = true;
}

void audio_stop_effect() {
	audio_playing = false;
}

//NRF reading - should go to ^^dkl ****
//NRF command format: 0x42 <datalen> <cmd> <data>
//datalen includes cmd


typedef enum {
	USER_KEY_RED     = 1,
	USER_KEY_YELLOW  = 2,
	USER_KEY_GREEN   = 3,
	USER_KEY_CYAN    = 4,
	USER_KEY_BLUE    = 5,
	USER_KEY_PURPLE  = 6,
	USER_KEY_REPLAY  = 7,
	USER_KEY_NEWGAME = 8
} USER_KEY;

typedef enum {
	GAME_TONE_RED    = 0,
	GAME_TONE_YELLOW = 1,
	GAME_TONE_GREEN  = 2,
	GAME_TONE_CYAN   = 3,
	GAME_TONE_BLUE   = 4,
	GAME_TONE_PURPLE = 5,
	GAME_TONE_WON    = 6,
	GAME_TONE_LOST   = 7,
	GAME_TONE_GOOD   = 8
} GAME_TONE;

const uint8_t game_tone_durations[] = {
	50,50,50,50,50,50,100,100,100
};

typedef enum {
	NRF_USER_KEY_PRESSED = 1,	//data = user key (uint8)
} NRF_COMMAND_CODE;

typedef enum {
	GAME_STATE_NOGAME  = 0,	//no game running, can play freely
	GAME_STATE_SAYING  = 1, //we're saying
	GAME_STATE_LISTENING = 2,
	GAME_STATE_LENGTH_FINISHED = 3,
	GAME_STATE_LOST = 4,
	GAME_STATE_WON = 5
} GAME_STATE;

// ----------------------------
// --- very poor LCG random ---
// ----------------------------

uint32_t rnd_seed = 0;

void srand(uint32_t seed) {
	rnd_seed = seed;
}

uint32_t rand() {
  rnd_seed = (1103515245 * rnd_seed + 12345);	//C99 recommendation vor LCG
  return rnd_seed;
}

// ------------
// --- game ---
// ------------

GAME_STATE game_state = GAME_STATE_NOGAME;
uint32_t game_total_ticks = 0;
uint16_t game_state_ticks = 0;       //time in game state
uint16_t tone_playing_ticks = 0;     //time in tone play or 0 if not playing
uint16_t tone_stop_ticks = 0;        //duration of tone
uint16_t game_say_ticks = 60;
uint16_t game_sequence_length = 1;   //current sequence length
uint16_t game_sequence_idx = 0;	//position we're listening
uint8_t game_sequence[GAME_MAX_SEQUENCE_LENGTH];

void game_stop_tone() {
	tone_playing_ticks = 0;
	anim_fill_color(0,0,0);
	audio_stop_effect();
}

#define BBB 0x00,0x00,0x00
#define GGG 0x00,0xff,0x00
#define RRR 0xff,0x00,0x00

const uint8_t smile_matrix[] = {
	GGG,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,GGG,
	BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,
	BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,
	BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,
	BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,
	BBB,GGG,BBB,BBB,BBB,BBB,BBB,BBB,GGG,BBB,
	BBB,BBB,GGG,BBB,BBB,BBB,BBB,GGG,BBB,BBB,
	BBB,BBB,BBB,GGG,BBB,BBB,GGG,BBB,BBB,BBB,
	BBB,BBB,BBB,BBB,GGG,GGG,BBB,BBB,BBB,BBB,
};

const uint8_t sad_face_matrix[] = {
	RRR,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,RRR,
	BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,
	BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,
	BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,
	BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,BBB,
	BBB,BBB,BBB,BBB,RRR,RRR,BBB,BBB,BBB,BBB,
	BBB,BBB,BBB,RRR,BBB,BBB,RRR,BBB,BBB,BBB,
	BBB,BBB,RRR,BBB,BBB,BBB,BBB,RRR,BBB,BBB,
	BBB,RRR,BBB,BBB,BBB,BBB,BBB,BBB,RRR,BBB,
};

void game_play_tone(GAME_TONE tone) {
	if (tone_playing_ticks) {
		game_stop_tone();
	}
	uint8_t on = 100;
	switch (tone) {
		case GAME_TONE_RED:    anim_fill_color(on, 0, 0); break;
		case GAME_TONE_YELLOW: anim_fill_color(on,on, 0); break;
		case GAME_TONE_GREEN:  anim_fill_color( 0,on, 0); break;
		case GAME_TONE_CYAN:   anim_fill_color( 0,on,on); break;
		case GAME_TONE_BLUE:   anim_fill_color( 0, 0,on); break;
		case GAME_TONE_PURPLE: anim_fill_color(on, 0,on); break;
		case GAME_TONE_WON:    anim_set_matrix(smile_matrix); break;
		case GAME_TONE_LOST:   anim_set_matrix(sad_face_matrix); break;
		case GAME_TONE_GOOD:   anim_set_matrix(smile_matrix); break;
		default:               anim_fill_color(on,on,on); break;

	}
	tone_playing_ticks = 1;
	if (tone < AUDIO_NUM_FX) {
		audio_play_effect(tone);
	}
	tone_stop_ticks = game_tone_durations[tone];
}

void game_won() {
	game_state = GAME_STATE_NOGAME;
	game_play_tone(GAME_TONE_WON);
	game_state_ticks = 0;
	return;
}

void game_lost() {
	game_state = GAME_STATE_NOGAME;
	game_play_tone(GAME_TONE_LOST);
	game_state_ticks = 0;
	return;
}

void game_say() {	//start saying the current sequence
	game_state = GAME_STATE_SAYING;
	game_state_ticks = 0;
	game_sequence_idx = 0;
}

void game_next_length() { //generate and start the next sequence
	if (game_sequence_length >= GAME_MAX_SEQUENCE_LENGTH) {
		game_won();
		return;
	}
	game_sequence_length++;
	game_say();
}

void game_start() {	//Start a new game
	uint16_t i;
	srand(rand()+game_total_ticks);
	for (i=0; i<GAME_MAX_SEQUENCE_LENGTH; i++) {
		game_sequence[i] = ((rand() >> 2) % 6);
	}
	game_sequence_length = 0;
	game_say_ticks = GAME_DEFAULT_SAY_TICKS;
	game_next_length();
}

void game_user_tone(GAME_TONE tone) { //game has entered a tone.
	if (game_state == GAME_STATE_NOGAME) {
		game_play_tone(tone);
	} else if (game_state == GAME_STATE_LISTENING) {
		GAME_TONE expected_tone = game_sequence[game_sequence_idx];
		if (expected_tone == tone) {	//right choice
			game_play_tone(tone);
			game_sequence_idx++;
			if (game_sequence_idx >= game_sequence_length) {
				game_state = GAME_STATE_LENGTH_FINISHED;
				game_state_ticks = 0;
			}
		} else {
			game_lost();
		}
	}
}

void game_handle_user_key(USER_KEY key) {
	switch (key) {
		case USER_KEY_NEWGAME:
			game_start();
			break;
		case USER_KEY_REPLAY:
			if (game_state == GAME_STATE_LISTENING) {
				game_say();
			}
		case USER_KEY_RED:
		case USER_KEY_YELLOW:
		case USER_KEY_GREEN:
		case USER_KEY_CYAN:
		case USER_KEY_BLUE:
		case USER_KEY_PURPLE:
			game_user_tone(key-1);
			break;
		default:
			break;
	}
}

void game_tick() {
	game_total_ticks++;
	game_state_ticks++;
	if (tone_playing_ticks) {
		tone_playing_ticks++;
		if (tone_playing_ticks >= tone_stop_ticks) {
			game_stop_tone();
		}
	}
	switch (game_state) {
		case GAME_STATE_SAYING:
			if (game_state_ticks % game_say_ticks == 1) {	//say next tone
				game_sequence_idx = game_state_ticks / game_say_ticks;
				if (game_sequence_idx >= game_sequence_length) {	//done saying
					game_state = GAME_STATE_LISTENING;
					game_state_ticks = 0;
					game_sequence_idx = 0;
				} else {
					game_play_tone(game_sequence[game_sequence_idx]);
				}
			}
			break;
		case GAME_STATE_LISTENING:
			if (game_state_ticks > GAME_MAX_LISTEN_TICKS) {
				game_lost();
			}
			break;
		case GAME_STATE_LENGTH_FINISHED:
			if (game_state_ticks == GAME_LENGTH_FINISHED_FEEDBACK_TICKS) {
				game_play_tone(GAME_TONE_GOOD);
			}
			if (game_state_ticks > GAME_LENGTH_FINISHED_CONTINUE_TICKS) {
				game_next_length();
			}

	}

}


void nrf_handle_command(uint8_t* data, uint8_t len) {
	// static bool on = false;
	// on = !on;

	if (len < 1) return;
	switch (data[0]) {
		case NRF_USER_KEY_PRESSED:
			if (len < 2) break;
			game_handle_user_key(data[1]);
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


uint32_t getNextSample() {
	audio_playbackIdx = (audio_playbackIdx+1) % AUDIO_BUFFER_SIZE;
	return audio_buffer[audio_playbackIdx];
}


void main(void) {
	dkl_init();

	//init LED matrix
	anim_init(MATRIX_WIDTH, MATRIX_HEIGHT, rgb);
	anim_fill_color(0,0,0);
	ledstripe_on(NUM_LEDS, led_buffer);

	//init nordic communication
	set_digital_input(NRF0_3_PIN, true, false, true);
	usart_configure_pin(NRF_UART_RX_GROUP, NRF_UART_RX_IDX, NRF_UART_RX_MODE, true, false);
	usart_configure_pin(NRF_UART_TX_GROUP, NRF_UART_TX_IDX, NRF_UART_TX_MODE, false, false);
	//TODO: clock should be at 115200, but finding clock dividers doesnt seem to work
	usart_init_async(NRF_UART_IDX, 8, 115200, UART_PARITY_NONE, false, 128,
					USART_FCR_RXTRIGLVL_8, nrf_data_received, NULL,
					nrf_comm_err);

	//init sd card
	sd_init();
	while (!sd_card_detected()) {}
	sd_set_power(true);
	bool success = sd_init_card(&card_info);

	audio_readBlockIdx = 10;

	audio_on();
	audio_play(1, 16, 44100, getNextSample);

	systick_start(SYSTICK_10MS);

	ledstripe_sendRGB(rgb,stripe_led_lookup);

}

void systick() {
	static uint32_t dispatch = 0;
	dispatch++;
	if ((dispatch % 5) == 0) {
		ledstripe_sendRGB(rgb,stripe_led_lookup);
	} else {
		audio_tick();
		game_tick();
//		anim_tick();
	}
	
	// switch (dispatch % 3) {
	// 	case 0:
	// 		break;
	// 	case 1:
	// 		break;
	// 	case 2:
	// 		break;
	// }
}
