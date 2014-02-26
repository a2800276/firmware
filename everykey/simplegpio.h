#ifndef _SIMPLEGPIO_
#define _SIMPLEGPIO_

#include "types.h"

/* A pin_id represents package pin name (P<major>_<minor>), the scu function, the GPIO port and index in one value. 
33222222 22221111 11111100 00000000
10987654 32109876 54321098 76543210
0<F><PT> 000<IDX> 000<MAJ> 000<MIN>

PT: GPIO Port
IDX: GPIO index within port
F: SCU function
MAJ: Pin major (first number).
MIN: Pin minor

A pin_id may be used to identify other pins and functions than GPIO. In this case, IDX should be ff. MAJ 10 = clock pins.
*/

typedef uint32_t pin_id;


#define P1_7_AS_GPIO_1_0   0x01000107
#define P1_8_AS_GPIO_1_1   0x01010108
#define P1_9_AS_GPIO_1_2   0x01020109
#define P1_10_AS_GPIO_1_3  0x0103010a
#define P1_11_AS_GPIO_1_4  0x0104010b
#define P1_12_AS_GPIO_1_5  0x0105010c
#define P1_13_AS_GPIO_1_6  0x0106010d
#define P1_14_AS_GPIO_1_7  0x0107010e
#define P1_17_AS_GPIO_0_12 0x000c0111
#define P2_7_AS_GPIO_0_7   0x00070207
#define P2_11_AS_GPIO_1_11 0x010b020b
#define P3_4_AS_GPIO_1_14  0x010e0304
#define P3_5_AS_GPIO_1_15  0x010f0305
#define P3_6_AS_GPIO_0_6   0x00060306

/** configures a GPIO pin_id, output */
void set_digital_output(pin_id pin);

/** configures a GPIO pin_id, strong drive output. Only works with pins
with strong drive capability (see UM10430 / UM10503) */
void set_strong_output(pin_id pin);

/** sets the value of a digital out pin
@param pin to write to
@param val value to set */
void write_pin(pin_id pin, bool val);

/** configures a GPIO pin_id, input
@param pin pin to configure
@param pullup set to true to enable the pin's pullup
@param pulldown set to true to enable the pin's pullup
@param glitchfilter set to true to enable the pin's glitch filter */
void set_digital_input(pin_id pin, bool pullup, bool pulldown, bool glitchfilter);

/** reads the value of a digital in pin
@param pin pin to read
@return the pin's value */
bool read_pin(pin_id pin);

#endif