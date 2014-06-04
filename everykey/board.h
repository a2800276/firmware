/* Low level board settings (pin assignment, frequencies etc.). These may be included from the standard firmware. */
#ifndef _BOARD_
#define _BOARD_

#define XTAL_MHZ 12
#define XTAL_IS_CRYSTAL 1
#define MAIN_CLOCK_MHZ 180

#define ETH_PHY_ADDR 0  /* Ethernet PHY address */

//This should go later: A pin with a LED to blink for debugging
#define DEBUG_LED_PIN P7_6_AS_GPIO_3_14

#endif
