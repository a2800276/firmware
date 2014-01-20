#include "gpio.h"

void gpio_set_pin_dir(uint8_t port, uint8_t pin, PIN_DIR dir) {
	if (dir == OUTPUT) GPIO->DIR[port] |= (1 << pin);
	else GPIO->DIR[port] &= ~(1 << pin);
}

bool gpio_read_digital_pin(uint8_t port, uint8_t pin) {
	return (GPIO->W[port].PIN[pin] != 0);
}

void gpio_write_digital_pin(uint8_t port, uint8_t pin, bool value) {
	GPIO->W[port].PIN[pin] = value;
}


