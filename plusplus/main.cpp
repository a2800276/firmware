#include "everykey/everykey.h"

extern "C" {

void progfault(int err) {
	write_pin(LED3_PIN, true);
	while(1);
}

}

class Blinker {
	private:
		pin_id pinid;
		bool toggle;
	public:
		Blinker(pin_id pin);
		~Blinker();
		void blink();
};

Blinker::Blinker(uint32_t pin) {
	this->pinid = pin;
	toggle = false;
	write_pin(LED1_PIN, toggle);
}

Blinker::~Blinker() {

}

void Blinker::blink() {
	toggle=!toggle;
	write_pin(pinid, toggle);
}


Blinker blinker1(LED1_PIN);

int main(void) {
	sparrow_init();
	Blinker blinker2(LED2_PIN);

	while (1) {
		delay(5000000);
		blinker1.blink();
		blinker2.blink();
}
//	systick_start(SYSTICK_10MS);
}


/*
void systick() {
	static uint32_t counter;
	counter++;
	write_pin(LED1_PIN, (counter / 25) & 1);

	if ((counter % 50) == 0) {
		audio_on();
	}
}
*/
