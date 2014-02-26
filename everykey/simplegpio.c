#include "simplegpio.h"
#include "scu.h"
#include "gpio.h"

#define PIN_MINOR(pin_id)((pin_id)&0x1f)
#define PIN_MAJOR(pin_id)(((pin_id)>>8)&0x0f)
#define PIN_GPIO_IDX(pin_id)(((pin_id)>>16)&0x1f)
#define PIN_GPIO_PORT(pin_id)(((pin_id)>>24)&0x0f)
#define PIN_FUNC(pin_id)(((pin_id)>>28)&0x07)

void set_digital_output(pin_id pin) {
	scu_set_pin_mode(PIN_MAJOR(pin), PIN_MINOR(pin), PIN_FUNC(pin));
	gpio_set_pin_dir(PIN_GPIO_PORT(pin), PIN_GPIO_IDX(pin), OUTPUT);
}

void set_strong_output(pin_id pin) {
	scu_set_pin_mode(PIN_MAJOR(pin), PIN_MINOR(pin), PIN_FUNC(pin));
	scu_set_pin_drive_strength(PIN_MAJOR(pin), PIN_MINOR(pin), 3);
	gpio_set_pin_dir(PIN_GPIO_PORT(pin), PIN_GPIO_IDX(pin), OUTPUT);
}

void write_pin(pin_id pin, bool val) {
	gpio_write_digital_pin(PIN_GPIO_PORT(pin), PIN_GPIO_IDX(pin), val);
}

void set_digital_input(pin_id pin, bool pullup, bool pulldown, bool glitchfilter) {
	scu_set_pin_mode(PIN_MAJOR(pin), PIN_MINOR(pin), PIN_FUNC(pin));
	gpio_set_pin_dir(PIN_GPIO_PORT(pin), PIN_GPIO_IDX(pin), INPUT);
	scu_set_pin_pullup(PIN_MAJOR(pin), PIN_MINOR(pin), pullup);
	scu_set_pin_pulldown(PIN_MAJOR(pin), PIN_MINOR(pin), pulldown);
	scu_enable_pin_glitch_filter(PIN_MAJOR(pin), PIN_MINOR(pin), glitchfilter);
	scu_enable_pin_in_buffer(PIN_MAJOR(pin), PIN_MINOR(pin), true);
}

bool read_pin(pin_id pin) {
	gpio_read_digital_pin(PIN_GPIO_PORT(pin), PIN_GPIO_IDX(pin));
}
