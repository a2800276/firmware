#include "anykey/anykey.h"

void delay(int count) {
	volatile int i; 
	for (i=0; i<count; i++) {} 
}

#define LED_GROUP 1
#define LED_IDX 17
#define LED_FUNC 0
#define LED_PORT 0
#define LED_PIN 12


void main(void) {
//	SetPinMode(LED_GROUP, LED_IDX, LED_FUNC);
//	SetPinDriveStrength(LED_GROUP, LED_IDX, 3);

	HW_RW* SFSP1_17 = ((HW_RW*)(0x400860C4));
	*SFSP1_17 = 0x300;

	HW_RW* GPIO_DIR0 = ((HW_RW*)(0x400f6000));
	*GPIO_DIR0 = 0x1000;

	HW_RW* GPIO_W0_12 = ((HW_RW*)(0x400f5030));

	HW_RW* SET0 = ((HW_RW*)(0x400f6200));
	HW_RW* CLR0 = ((HW_RW*)(0x400f6280));
	HW_RW* NOT0 = ((HW_RW*)(0x400f6300));

//	SetPinDir(LED_PORT, LED_PIN, OUTPUT);

	while (true) {
//		WriteDigitalPin(LED_PORT, LED_PIN, true);
		*NOT0 = 0xffffffff;
		delay(1000000);
//		WriteDigitalPin(LED_PORT, LED_PIN, false);
//		*CLR0 = 0xffffffff;
//		delay(1000000);
	}
}

