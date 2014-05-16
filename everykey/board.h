/* Low level board settings (pin assignment, frequencies etc.). These may be included from the standard firmware. */
#ifndef _BOARD_
#define _BOARD_

#define XTAL_MHZ 12
#define XTAL_IS_CRYSTAL 1
#define MAIN_CLOCK_MHZ 180

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

#define I2C1_SDA_PIN_GROUP 2
#define I2C1_SDA_PIN_IDX 3
#define I2C1_SDA_PIN_MODE 1
#define I2C1_SCL_PIN_GROUP 2
#define I2C1_SCL_PIN_IDX 4
#define I2C1_SCL_PIN_MODE 1

#endif