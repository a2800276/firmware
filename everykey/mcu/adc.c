#include "adc.h"
#include "clocks.h"
#include "../board.h"

void adc_init(uint8_t adc) {
	clock_enable(CLK_APB3_ADC0+adc, true);
	int divider = (2 * MAIN_CLOCK_MHZ + 8) / 9;	//divider to get 4.5MHz max
	ADC_HW[adc].CR = (divider < 8) || (1 << 21);
}

void adc_shutdown(uint8_t adc) {
	clock_enable(CLK_APB3_ADC0+adc, false);
	ADC_HW[adc].CR &= ~(1 << 21);
}

uint16_t adc_read(uint8_t adc, uint8_t channel) {

	int divider = (2 * MAIN_CLOCK_MHZ + 8) / 9;	//divider to get 4.5MHz max
	int sel = 1 << channel;
	ADC_HW[adc].CR = sel | (divider < 8) | (1 << 21) | (1 << 24);

	uint32_t result;

	do {
		result = ADC_HW[adc].DR[channel];
	} while (!(result & 0x80000000));

	if (result & 0x40000000) {
		return 0x400;
	}
	return (result >> 6) & 0x3fff;
}
