/* High level API for the sparrow board. This abstraction sits on top of the standard library. Low level defines
required by the generic library functions should go to board.h */

#ifndef _SPARROW_
#define _SPARROW_

#include "core/types.h"
#include "utils/simpleio.h"
#include "mcu/i2s.h"

// -------------------------------
// --- Board pins and settings ---
// -------------------------------

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

//SD card pin settings

#define SD_CLK_CLK 2
#define SD_CLK_MODE 4

#define SD_CD_GROUP 1
#define SD_CD_PIN 13
#define SD_CD_MODE 7

#define SD_WP_GROUP 4
#define SD_WP_PIN 9
#define SD_WP_MODE 4
#define SD_WP_GPIO_PORT 5
#define SD_WP_GPIO_IDX 13

#define SD_CMD_GROUP 1
#define SD_CMD_PIN 6
#define SD_CMD_MODE 7

#define SD_D0_GROUP 1
#define SD_D0_PIN 9
#define SD_D0_MODE 7

#define SD_D1_GROUP 1
#define SD_D1_PIN 10
#define SD_D1_MODE 7

#define SD_D2_GROUP 1
#define SD_D2_PIN 11
#define SD_D2_MODE 7

#define SD_D3_GROUP 1
#define SD_D3_PIN 12
#define SD_D3_MODE 7

// ethernet pin config

#define ETH_CLK_CLK 0
#define ETH_CLK_MODE 7 /* hs slew, buffer on, glitch filter off, no pull */

#define ETH_RXD0_GROUP 1
#define ETH_RXD0_PIN 15
#define ETH_RXD0_MODE 3 /* hs slew, buffer on, glitch filter off, no pull */

#define ETH_RXD1_GROUP 0
#define ETH_RXD1_PIN 0
#define ETH_RXD1_MODE 2 /* hs slew, buffer on, glitch filter off, no pull */

#define ETH_TXD0_GROUP 1
#define ETH_TXD0_PIN 18
#define ETH_TXD0_MODE 3 /* hs slew, no pull, no glitch filter */

#define ETH_TXD1_GROUP 1
#define ETH_TXD1_PIN 20
#define ETH_TXD1_MODE 3 /* hs slew, no pull, no glitch filter */

#define ETH_MDC_GROUP 7
#define ETH_MDC_PIN 7
#define ETH_MDC_MODE 6  /* hs slew, no pull, no glitch filter */

#define ETH_TXEN_GROUP 0
#define ETH_TXEN_PIN 1
#define ETH_TXEN_MODE 6 /* hs slew, no pull, no glitch filter */

#define ETH_CRS_DV_GROUP 1
#define ETH_CRS_DV_PIN 16
#define ETH_CRS_DV_MODE 7 /* 7 for DV/CRS (RMII): hs slew, no pull, buffer on, glitch filter off */

#define ETH_MDIO_GROUP 1
#define ETH_MDIO_PIN 17
#define ETH_MDIO_MODE 3 /* hs slew, no pull, boffer on, glitch filter off */

#define ETH_NINT_PIN P3_1_AS_GPIO_5_8
#define ETH_NRST_PIN P9_5_AS_GPIO_5_18

// Sparrow UART/USART pins

#define USART0_TXD_GROUP  6
#define USART0_TXD_IDX    4
#define USART0_TXD_MODE   2
#define USART0_RXD_GROUP  6
#define USART0_RXD_IDX    5
#define USART0_RXD_MODE   2
#define USART0_UCLK_GROUP 2
#define USART0_UCLK_IDX   2
#define USART0_UCLK_MODE  1

#define UART1_TXD_GROUP   5
#define UART1_TXD_IDX     6
#define UART1_TXD_MODE    4
#define UART1_RXD_GROUP   5
#define UART1_RXD_IDX     7
#define UART1_RXD_MODE    4
#define UART1_CTS_GROUP   5
#define UART1_CTS_IDX     4
#define UART1_CTS_MODE    4
#define UART1_RTS_GROUP   5
#define UART1_RTS_IDX     2
#define UART1_RTS_MODE    4

#define USART2_TXD_GROUP  2
#define USART2_TXD_IDX   10
#define USART2_TXD_MODE   2
#define USART2_RXD_GROUP  2
#define USART2_RXD_IDX   11
#define USART2_RXD_MODE   2
#define USART2_UCLK_GROUP 2
#define USART2_UCLK_IDX  12
#define USART2_UCLK_MODE  7
#define USART2_DIR_GROUP  2
#define USART2_DIR_IDX   13
#define USART2_DIR_MODE   7

#define USART3_TXD_GROUP  4
#define USART3_TXD_IDX    1
#define USART3_TXD_MODE   6
#define USART3_RXD_GROUP  4
#define USART3_RXD_IDX    2
#define USART3_RXD_MODE   6
#define USART3_UCLK_GROUP 4
#define USART3_UCLK_IDX   0
#define USART3_UCLK_MODE  6
#define USART3_DIR_GROUP  4
#define USART3_DIR_IDX    4
#define USART3_DIR_MODE   6
#define USART3_BAUD_GROUP 4
#define USART3_BAUD_IDX   3
#define USART3_BAUD_MODE  6


#define SPARROW_I2S 0


#ifdef __cplusplus
extern "C" {
#endif

// -------------------
// --- Sparrow API ---
// -------------------

/** reset all, configure pins, disable all peripherals
Should be called once after startup or reset-like wakeup */
void sparrow_init();

// ----------------------------------------
// --- AUDIO: I2S, TLV320AIC3100, Board ---
// ----------------------------------------

/** Power on audio block, init codec */
void audio_on();

/** Power off audio block */
void audio_off();

/** start audio playback, per-sample callback
@param numChannels 1 for mono, 2 for stereo
@param bitsPerSample resolution in bits per sample (for one channel)
@param sampleRate sample rate in Hz. 44100 and 48000 are tested to work.
@param cb sample getter callback. Called at interrupt time, should return asap. */
void audio_play(uint8_t numChannels, uint8_t bitsPerSample, uint32_t sampleRate, I2S_PLAY_SAMPLE_CALLBACK cb);

// --------------------------------------------
// --- ETHERNET: ETH, LAN8720A, LWIP, Board ---
// --------------------------------------------

/** power on ethernet block, init */
void ethernet_on();

/** power off ethernet block */
void ethernet_off();

// ----------------------------------------------------------------------
// --- LED STRIPE (WS2812Bs connected to USART0 via 5V level shifter) ---
// ----------------------------------------------------------------------

void ledstripe_on();
/** turns on led stripe */

/** turns off led stripe */
void ledstripe_off();

#ifdef __cplusplus
}
#endif

#endif
