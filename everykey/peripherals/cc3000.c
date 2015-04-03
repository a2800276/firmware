#include "cc3000.h"
#include "../dkl.h"
#include "../mcu/scu.h"
#include "../mcu/nvic.h"

typedef struct {
	uint8_t opcode;
	uint8_t len_hi;
	uint8_t len_lo;
	uint8_t busy1;
	uint8_t busy2;
	uint8_t payload[1];	//should be longer - pad to 2 bytes
} CC3000_SPI_PACKET;

typedef enum {
	CC3000_State_Off = 0,
	CC3000_State_Starting,
	CC3000_State_Started,
	CC3000_State_Idle,
	CC3000_State_Transfer_Start,
	CC3000_State_Ready_For_Data,
	CC3000_State_Data_To_Send
} CC3000_State;

static CC3000_State state = CC3000_State_Off;

void gpio0_handler(void) {
	nvic_clear_interrupt_pending(NVIC_PIN_INT0 + CC3000_IRQ_PIN_INTERRUPT_IDX);
	scu_clear_edge_pin_interrupt(CC3000_IRQ_PIN_INTERRUPT_IDX);
	switch (state) {
		case CC3000_State_Starting:
			state = CC3000_State_Started;
			break;
		case CC3000_State_Idle:
			state = CC3000_State_Data_To_Send;
			break;
		case CC3000_State_Transfer_Start:
			state = CC3000_State_Ready_For_Data;
			break;
	}
}

void cc3000_enable_irq(bool on) {
	if (on) {
		scu_enable_pin_interrupt(CC3000_IRQ_PIN_INTERRUPT_IDX,
			PINID_GPIO_PORT(CC3000_IRQ_PIN),
			PINID_GPIO_PIN(CC3000_IRQ_PIN),
			PIN_INTERRUPT_FALLING);
	} else {
		scu_disable_pin_interrupt(CC3000_IRQ_PIN_INTERRUPT_IDX);
	}
}

void cc3000_transfer_packet(CC3000_SPI_PACKET* packet) {

}	

//public

void cc3000_init() {
	state = CC3000_State_Starting;
	cc3000_enable_irq(true);
}

void cc3000_shutdown() {
	cc3000_enable_irq(false);
	state = CC3000_State_Off;
}


void cc3000_connect(uint8_t* ssid, uint8_t ssid_len, CC3000_SEC_TYPE sec_type, uint8_t* key, uint8_t key_len) {
	//TODO ***********
}


