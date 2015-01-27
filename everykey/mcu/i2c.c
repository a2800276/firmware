
#include "i2c.h"
#include "clocks.h"
#include "nvic.h"
#include "startup.h"
#include "rgu.h"
#include "scu.h"
#include "../board.h"
#include "../utils/progfault.h"


I2C_State i2c_state[2];	//all inited to 0

void i2c_handler(uint8_t i2c) {
	uint32_t status = 0xf8 & I2C[i2c].STAT;


	I2C_STATUS userStatus = I2C_STATUS_OK; 	//status code to return

	switch (status) {
		case I2C_STAT_START_SENT:			//Start condition: load address+read/write, clear start
		case I2C_STAT_REP_START_SENT:
			I2C[i2c].DAT = (i2c_state[i2c].slaveAddress << 1) | ((i2c_state[i2c].toWrite > 0) ? 0 : 1);	//SLA+R/W
			I2C[i2c].CONCLR = I2C_CONCLR_STAC;
			break;
		case I2C_STAT_SLAW_ACKED:			//Write addressing succeeded
		case I2C_STAT_DATA_WRITE_ACKED:		//Byte successfully written
			if (i2c_state[i2c].toWrite > 0) {		//more to write
				I2C[i2c].DAT = *(i2c_state[i2c].writeBuffer);
				i2c_state[i2c].writeBuffer++;
				i2c_state[i2c].toWrite--;
			} else if (i2c_state[i2c].toRead > 0) {		//write done, have something to read
				I2C[i2c].CONSET = I2C_CONSET_STA;
			} else {						//nothing to read or write any more -> stop
				userStatus = I2C_STATUS_OK;
				goto stopAndNotifyUser;
			}
			break;
		case I2C_STAT_SLAW_NACKED:			//SLAW failed -> finish, notify
			I2C[i2c].CONCLR = I2C_CONCLR_STAC | I2C_CONCLR_AAC;
			userStatus = I2C_STATUS_ADDRESSING_FAILED;
			goto stopAndNotifyUser;
			break;
		case I2C_STAT_DATA_WRITE_NACKED:	//Data write failed -> finish, notify
			I2C[i2c].CONCLR = I2C_CONCLR_STAC | I2C_CONCLR_AAC;
			userStatus = I2C_STATUS_DATA_FAILED;
			goto stopAndNotifyUser;
			break;
		case I2C_STAT_ARBITRATION_LOST:		//Arbitration lost -> finish, notify
			i2c_state[i2c].toWrite = 0;
			i2c_state[i2c].toRead = 0;
			I2C[i2c].CONCLR = I2C_CONCLR_STAC | I2C_CONCLR_AAC;
			userStatus = I2C_STATUS_ARBITRATION_LOST;
			goto notifyUser;
			break;
		case I2C_STAT_SLAR_ACKED:
			if (i2c_state[i2c].toRead > 1) I2C[i2c].CONSET = I2C_CONSET_AA;
			else I2C[i2c].CONCLR = I2C_CONCLR_AAC;
			break;
		case I2C_STAT_SLAR_NACKED:			//SLAR nacked -> finish, notify
			I2C[i2c].CONCLR = I2C_CONCLR_STAC | I2C_CONCLR_AAC;
			userStatus = I2C_STATUS_ADDRESSING_FAILED;
			goto stopAndNotifyUser;
			break;
		case I2C_STAT_DATA_READ_ACKED:		//byte read and acked (more to come)
			{
				uint8_t val = I2C[i2c].DAT;
				if (i2c_state[i2c].toRead > 0) {
					*(i2c_state[i2c].readBuffer) = val;
					i2c_state[i2c].readBuffer++;
					i2c_state[i2c].toRead--;
				}
			}
			if (i2c_state[i2c].toRead > 1) I2C->CONSET = I2C_CONSET_AA;
			else I2C->CONCLR = I2C_CONCLR_AAC;
			break;
		case I2C_STAT_DATA_READ_NACKED:		//byte read and nacked (done)
			{
				uint8_t val = I2C->DAT;
				if (i2c_state[i2c].toRead > 0) {
					*(i2c_state[i2c].readBuffer) = val;
					i2c_state[i2c].readBuffer++;
					i2c_state[i2c].toRead--;
				}
			}
			userStatus = I2C_STATUS_OK;
			goto stopAndNotifyUser;
			break;
		default: 	//Should not happen
			userStatus = I2C_STATUS_INTERNAL_ERROR;
			goto stopAndNotifyUser;
			break;
	}

	I2C->CONCLR = I2C_CONCLR_SIC; 	//Clear interrupt - continue bus
	return;

stopAndNotifyUser:
	i2c_state[i2c].toWrite = 0;
	i2c_state[i2c].toRead = 0;
	I2C->CONSET = I2C_CONSET_STO;
notifyUser:
	I2C->CONCLR = I2C_CONCLR_SIC;
	if (i2c_state[i2c].completionHandler) i2c_state[i2c].completionHandler(i2c, i2c_state[i2c].refcon, userStatus);
}

void i2c0_handler() {
	i2c_handler(0);
}

void i2c1_handler() {
	i2c_handler(1);
}


void i2c_init(uint8_t i2c, I2C_MODE mode) {

	if (i2c > 1) progfault(ILLEGAL_ARGUMENT);
	if ((i2c == 1) && (mode != I2C_MODE_FAST)) progfault(ILLEGAL_ARGUMENT);

	uint32_t half_clock;
	switch (mode) {
		case I2C_MODE_STANDARD:
			half_clock = MAIN_CLOCK_MHZ * 5;
			break;
		case I2C_MODE_FASTPLUS:
			half_clock = MAIN_CLOCK_MHZ / 2;
			break;
		case I2C_MODE_FAST:
			half_clock = (MAIN_CLOCK_MHZ*5) / 4;
			break;
		default:
			progfault(ILLEGAL_ARGUMENT);
	}

	i2c_state[i2c].running = false;
	i2c_state[i2c].refcon = 0;
	i2c_state[i2c].slaveAddress = 0;
	i2c_state[i2c].toWrite = 0;
	i2c_state[i2c].writeBuffer = NULL;
	i2c_state[i2c].toRead = 0;
	i2c_state[i2c].readBuffer = NULL;
	i2c_state[i2c].completionHandler = NULL;

	//configure pins
	if (i2c == 0) {
		scu_set_i2c0_pinmode(true, mode == I2C_MODE_FASTPLUS, true, false);
	}

	// Turn clock for peripheral
	clock_enable(i2c ? CLK_APB3_I2C1 : CLK_APB1_I2C0,true);

	//set up peripheral
	I2C[i2c].CONCLR = I2C_CONCLR_AAC | I2C_CONCLR_SIC | I2C_CONCLR_STAC | I2C_CONCLR_I2ENC; //clear flags
	I2C[i2c].SCLH = half_clock;
	I2C[i2c].SCLL = half_clock;

	//enable interrupt
	nvic_enable_interrupt((i2c == 0) ? NVIC_I2C0 : NVIC_I2C1);

	// reset peripheral - this seems to mess things up. TODO: Check why *****************
/*
	RGU_RESET_INDEX resetIdx = i2c ? I2C1_RST : I2C0_RST;
	rgu_trigger_reset(resetIdx);
	while (rgu_reset_active(resetIdx)) {}
*/

	//turn on I2C engine
	I2C[i2c].CONSET = I2C_CONSET_I2EN;

	//we're done
	i2c_state[i2c].running = true;
}

void i2c_configure_pin(uint8_t group, uint8_t idx, uint8_t mode) {
	scu_set_pin_mode(group, idx, mode);
	scu_set_pin_pullup(group, idx, true);
	scu_set_pin_pulldown(group, idx, false);
	scu_enable_pin_in_buffer(group, idx, true);
	scu_enable_pin_glitch_filter(group, idx, true);
	scu_set_pin_drive_strength(group, idx, 3);
}

I2C_STATUS i2c_write(uint8_t i2c,
                    uint8_t addr,
                    uint16_t len,
                    uint8_t* buf,
                    I2C_CompletionHandler handler,
                    uint32_t refcon) {
	return i2c_write_read(i2c, addr, len, buf, 0, NULL, handler, refcon);
}

I2C_STATUS i2c_read(uint8_t i2c,
                    uint8_t addr,
                    uint16_t len,
                    uint8_t* buf,
                    I2C_CompletionHandler handler,
                    uint32_t refcon) {
	return i2c_write_read(i2c, addr, 0, NULL, len, buf, handler, refcon);
}

I2C_STATUS i2c_write_read(uint8_t i2c,
                    uint8_t addr,
                    uint16_t writeLen,
                    uint8_t* writeBuf,
                    uint16_t readLen,
                    uint8_t* readBuf,
                    I2C_CompletionHandler handler,
                    uint32_t refcon) {

	if (!i2c_state[i2c].running) return I2C_STATUS_UNINITIALIZED;
	if (i2c_transaction_running(i2c)) return I2C_STATUS_BUSY;
	i2c_state[i2c].refcon = refcon;
	i2c_state[i2c].slaveAddress = addr;
	i2c_state[i2c].toWrite = writeLen;
	i2c_state[i2c].writeBuffer = writeBuf;
	i2c_state[i2c].toRead = readLen;
	i2c_state[i2c].readBuffer = readBuf;
	i2c_state[i2c].completionHandler = handler;
	I2C->CONSET = I2C_CONSET_STA; //send start condition.
	return I2C_STATUS_OK;
}

bool i2c_transaction_running(uint8_t i2c) {
	return ((i2c_state[i2c].toRead > 0) || (i2c_state[i2c].toWrite > 0));
}

void i2c_cancel_transaction(uint8_t i2c) {
	i2c_state[i2c].completionHandler = NULL;
	i2c_state[i2c].toWrite = 0;
	i2c_state[i2c].toRead = 0;
	I2C[i2c].CONSET = I2C_CONSET_STO; //send stop condition (which will hopefully stop everything)
}

// Synchronous API

volatile static I2C_STATUS i2c_sync_status;

void i2c_sync_completion_handler(uint8_t i2c, uint32_t refcon, I2C_STATUS status) {
	i2c_sync_status = status;
}

I2C_STATUS i2c_write_sync(uint8_t i2c,
                          uint8_t addr,
                          uint16_t len,
                          uint8_t* buf) {
	i2c_sync_status = I2C_STATUS_INVALID;
	I2C_STATUS init_status = i2c_write(i2c, addr, len, buf, i2c_sync_completion_handler, 0);
	if (init_status != I2C_STATUS_OK) return init_status;
	while (i2c_sync_status == I2C_STATUS_INVALID) {};
	return i2c_sync_status;
}
