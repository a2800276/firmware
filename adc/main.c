#include "everykey/everykey.h"

/** Very basic example for reading the ADC. Connect some analog source (e.g. a loudness sensor)
to ADC pin 0. The value will be displayed on the built-in LEDs. */


#define LED1_PINGROUP 7
#define LED1_PINIDX 4
#define LED1_FUNC 0
#define LED1_GPIOPORT 3
#define LED1_GPIOIDX 12

#define LED2_PINGROUP 7
#define LED2_PINIDX 5
#define LED2_FUNC 0
#define LED2_GPIOPORT 3
#define LED2_GPIOIDX 13

#define LED3_PINGROUP 7
#define LED3_PINIDX 6
#define LED3_FUNC 0
#define LED3_GPIOPORT 3
#define LED3_GPIOIDX 14

void main(void) {
	//Boot without all the board stuff, just ADC and LEDs
	scu_set_pin_mode(LED1_PINGROUP, LED1_PINIDX, LED1_FUNC);
	gpio_set_pin_dir(LED1_GPIOPORT, LED1_GPIOIDX, OUTPUT);
	gpio_write_digital_pin(LED1_GPIOPORT, LED1_GPIOIDX, false);

	scu_set_pin_mode(LED2_PINGROUP, LED2_PINIDX, LED2_FUNC);
	gpio_set_pin_dir(LED2_GPIOPORT, LED2_GPIOIDX, OUTPUT);
	gpio_write_digital_pin(LED2_GPIOPORT, LED2_GPIOIDX, false);

	scu_set_pin_mode(LED3_PINGROUP, LED3_PINIDX, LED3_FUNC);
	gpio_set_pin_dir(LED3_GPIOPORT, LED3_GPIOIDX, OUTPUT);
	gpio_write_digital_pin(LED3_GPIOPORT, LED3_GPIOIDX, false);

	clock_set_xtal_core_freq(15, 1);

	adc_init(0);	//Turn on ADC unit 0 (don't confuse with input channels)

	systick_start(SYSTICK_10MS);
}

void systick() {
	//Read ADC 0 channel 0
	int adc = adc_read(0,0);

	//turn on LEDs if value is above certain thresholds. This allows a bar-meter type of display
	gpio_write_digital_pin(LED1_GPIOPORT, LED1_GPIOIDX, adc > 200);
	gpio_write_digital_pin(LED2_GPIOPORT, LED2_GPIOIDX, adc > 220);
	gpio_write_digital_pin(LED3_GPIOPORT, LED3_GPIOIDX, adc > 240);

}

