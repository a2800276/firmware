#ifndef _SIMPLEIO_
#define _SIMPLEIO_

#include "../core/types.h"

/* A pin_id represents package pin name (P<major>_<minor>), the scu function, the GPIO port and index in one value.
33222222 22221111 11111100 00000000
10987654 32109876 54321098 76543210
0<F><PT> 000<IDX> 000<MAJ> 000<MIN>

PT: GPIO Port
F: SCU function
IDX: GPIO index within port
MAJ: Pin major (first number).
MIN: Pin minor

A pin_id may be used to identify other pins and functions than GPIO. In this case, IDX should be ff. MAJ 10 = clock pins.
*/

#define PINID_SCU_FUNC(pinid) (((pinid)>>28)&0x07)
#define PINID_GPIO_PORT(pinid) (((pinid)>>24)&0x0f)
#define PINID_GPIO_PIN(pinid) (((pinid)>>16)&0x1f)
#define PINID_SCU_GROUP(pinid) (((pinid)>>8)&0x1f)
#define PINID_SCU_IDX(pinid) ((pinid)&0x1f)

typedef uint32_t pin_id;

#define P0_0_AS_GPIO_0_0   0x00000000
#define P0_1_AS_GPIO_0_1   0x00010001
#define P1_0_AS_GPIO_0_4   0x00040100
#define P1_1_AS_GPIO_0_8   0x00080101
#define P1_2_AS_GPIO_0_9   0x00090102
#define P1_3_AS_GPIO_0_10  0x000a0103
#define P1_4_AS_GPIO_0_11  0x000b0104
#define P1_5_AS_GPIO_1_8   0x01080105
#define P1_6_AS_GPIO_1_9   0x01090106
#define P1_7_AS_GPIO_1_0   0x01000107
#define P1_8_AS_GPIO_1_1   0x01010108
#define P1_9_AS_GPIO_1_2   0x01020109
#define P1_10_AS_GPIO_1_3  0x0103010a
#define P1_11_AS_GPIO_1_4  0x0104010b
#define P1_12_AS_GPIO_1_5  0x0105010c
#define P1_13_AS_GPIO_1_6  0x0106010d
#define P1_14_AS_GPIO_1_7  0x0107010e
#define P1_17_AS_GPIO_0_12 0x000c0111
#define P1_18_AS_GPIO_0_13 0x010d0112
// P1_19 has no GPIO function
#define P1_20_AS_GPIO_0_15 0x000f0114
#define P2_0_AS_GPIO_5_0   0x45000200
#define P2_1_AS_GPIO_5_1   0x45010201
#define P2_2_AS_GPIO_5_2   0x45020202
#define P2_3_AS_GPIO_5_3   0x45030203
#define P2_4_AS_GPIO_5_4   0x45040204
#define P2_5_AS_GPIO_5_5   0x45050205
#define P2_6_AS_GPIO_5_6   0x45060206
#define P2_7_AS_GPIO_0_7   0x00070207
#define P2_8_AS_GPIO_5_7   0x45070208
#define P2_9_AS_GPIO_1_10  0x010a0209
#define P2_10_AS_GPIO_0_14 0x000e020a
#define P2_11_AS_GPIO_1_11 0x010b020b
#define P2_12_AS_GPIO_1_12 0x010c020c
#define P2_13_AS_GPIO_1_13 0x010d020d
// P3_0 has no GPIO function
#define P3_1_AS_GPIO_5_8   0x45080301
#define P3_2_AS_GPIO_5_9   0x45090302
// P3_3 has no GPIO function
#define P3_4_AS_GPIO_1_14  0x010e0304
#define P3_5_AS_GPIO_1_15  0x010f0305
#define P3_6_AS_GPIO_0_6   0x00060306
#define P3_7_AS_GPIO_5_10  0x450a0307
#define P3_8_AS_GPIO_5_11  0x450b0308
#define P4_0_AS_GPIO_2_0   0x02000400
#define P4_1_AS_GPIO_2_1   0x02010401
#define P4_2_AS_GPIO_2_2   0x02020402
#define P4_3_AS_GPIO_2_3   0x02030403
#define P4_4_AS_GPIO_2_4   0x02040404
#define P4_5_AS_GPIO_2_5   0x02050405
#define P4_6_AS_GPIO_2_6   0x02060406
// P4_7 has no GPIO function
#define P4_8_AS_GPIO_5_12  0x450c0408
#define P4_9_AS_GPIO_5_13  0x450d0409
#define P4_10_AS_GPIO_5_14 0x450e040a
#define P5_0_AS_GPIO_2_9   0x02090500
#define P5_1_AS_GPIO_2_10  0x020a0501
#define P5_2_AS_GPIO_2_11  0x020b0502
#define P5_3_AS_GPIO_2_12  0x020c0503
#define P5_4_AS_GPIO_2_13  0x020d0504
#define P5_5_AS_GPIO_2_14  0x020e0505
#define P5_6_AS_GPIO_2_15  0x020f0506
#define P5_7_AS_GPIO_2_7   0x02070507
// P6_0 has no GPIO function
#define P6_1_AS_GPIO_3_0   0x03000601
#define P6_2_AS_GPIO_3_1   0x03010602
#define P6_3_AS_GPIO_3_2   0x03020603
#define P6_4_AS_GPIO_3_3   0x03030604
#define P6_5_AS_GPIO_3_4   0x03040605
#define P6_6_AS_GPIO_0_5   0x00050606
#define P6_7_AS_GPIO_5_15  0x450f0607
#define P6_8_AS_GPIO_5_16  0x45100608
#define P6_9_AS_GPIO_3_5   0x03050609
#define P6_10_AS_GPIO_3_6  0x0306060a
#define P6_11_AS_GPIO_3_7  0x0307060b
#define P6_12_AS_GPIO_2_8  0x0208060c
#define P7_0_AS_GPIO_3_8   0x03080700
#define P7_1_AS_GPIO_3_9   0x03090701
#define P7_2_AS_GPIO_3_10  0x030a0702
#define P7_3_AS_GPIO_3_11  0x030b0703
#define P7_4_AS_GPIO_3_12  0x030c0704
#define P7_5_AS_GPIO_3_13  0x030d0705
#define P7_6_AS_GPIO_3_14  0x030e0706
#define P7_7_AS_GPIO_3_15  0x030f0707
#define P9_5_AS_GPIO_5_18  0x45120905
#define P9_6_AS_GPIO_4_11  0x040b0906
// PF_4 has no GPIO function

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif
