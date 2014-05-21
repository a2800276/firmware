/* High level API for the sparrow board. This abstraction sits on top of the standard library. Low level defines
required by the generic library functions should go to board.h */

#ifndef _SPARROW_
#define _SPARROW_

#include "core/types.h"
#include "utils/simpleio.h"

//GPIO pins

#define LED1_PIN P7_4_AS_GPIO_3_12
#define LED2_PIN P7_5_AS_GPIO_3_13
#define LED3_PIN P7_6_AS_GPIO_3_14
#define BUTTON_PIN P2_7_AS_GPIO_0_7

#define BOOST_5V_EN_PIN P2_6_AS_GPIO_5_6
#define PORT_5V_EN_PIN P1_7_AS_GPIO_1_0
#define ETH_POWER_EN_PIN P1_14_AS_GPIO_1_7
#define AUDIO_POWER_EN_PIN P9_6_AS_GPIO_4_11
#define WIFI_POWER_EN_PIN P1_8_AS_GPIO_1_1

#define AUDIO_NRESET P3_5_AS_GPIO_1_15

//I2C1 pin config

#define I2C1_SDA_PIN_GROUP 2
#define I2C1_SDA_PIN_IDX 3
#define I2C1_SDA_PIN_MODE 1

#define I2C1_SCL_PIN_GROUP 2
#define I2C1_SCL_PIN_IDX 4
#define I2C1_SCL_PIN_MODE 1

//I2S0 pin config

#define I2S0_TX_MCLK_GROUP 3
#define I2S0_TX_MCLK_IDX 0
#define I2S0_TX_MCLK_MODE 3

#define I2S0_TX_WS_GROUP 7
#define I2S0_TX_WS_IDX 1
#define I2S0_TX_WS_MODE 2

#define I2S0_TX_SCK_GROUP 4
#define I2S0_TX_SCK_IDX 7
#define I2S0_TX_SCK_MODE 7

#define I2S0_TX_SD_GROUP 7
#define I2S0_TX_SD_IDX 2
#define I2S0_TX_SD_MODE 2

#define I2S0_RX_MCLK_GROUP 1
#define I2S0_RX_MCLK_IDX 19
#define I2S0_RX_MCLK_MODE 6

#define I2S0_RX_WS_GROUP 6
#define I2S0_RX_WS_IDX 1
#define I2S0_RX_WS_MODE 3

#define I2S0_RX_SCK_GROUP 6
#define I2S0_RX_SCK_IDX 0
#define I2S0_RX_SCK_MODE 4

#define I2S0_RX_SD_GROUP 6
#define I2S0_RX_SD_IDX 2
#define I2S0_RX_SD_MODE 3

// other settings

#define SPARROW_I2S 0

// Sparrow API

#ifdef __cplusplus
extern "C" {
#endif

//reset all, disable all peripherals
void sparrow_init();

//Power on audio block, init codec
void audio_on();

//Power off audio block
void audio_off();

//start audio playback
void audio_play(uint8_t numChannels, uint8_t bitsPerSample, uint32_t sampleRate);

#ifdef __cplusplus
}
#endif

#endif
