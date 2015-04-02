#ifndef _ADC_
#define _ADC_

#include "../core/types.h"

typedef struct {
	HW_RW CR;               //Control register
	HW_RO GDR;              //Global data register (last conversion value)
	HW_UU unused1;
	HW_RW INTEN;            //Interrupt enable
	HW_RO DR[8];            //Last channel 0-7 conversion
	HW_RO STAT;             //status
	HW_UU unused2[1011];    //padding to next ADC block
} __attribute__((aligned(4))) ADC_STRUCT;

#define ADC_HW ((ADC_STRUCT*)(0x400E3000))

#ifdef __cplusplus
extern "C" {
#endif

/** starts up ADC 
@param adc ADC index (0 or 1) */
void adc_init(uint8_t adc);

/** shuts down ADC 
@param adc ADC index (0 or 1) */
void adc_shutdown(uint8_t adc);

/** reads ADC. Pin is currently always the dedicated ADC pin
@param adc ADC index (0 or 1)
@param channel ADC channel (0-7) */
uint16_t adc_read(uint8_t adc, uint8_t channel);

#ifdef __cplusplus
}
#endif



#endif