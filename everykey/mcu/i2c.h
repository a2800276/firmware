/* I2C functions */

#ifndef _IIC_
#define _IIC_

#include "../core/types.h"

/** I2C status response codes */
typedef enum {
	I2C_STATUS_OK = 0,
	I2C_STATUS_UNINITIALIZED,
	I2C_STATUS_BUSY,
	I2C_STATUS_ADDRESSING_FAILED,
	I2C_STATUS_DATA_FAILED,
	I2C_STATUS_ARBITRATION_LOST,
	I2C_STATUS_TIMEOUT,
	I2C_STATUS_INTERNAL_ERROR,	//Something weird happened
	I2C_STATUS_INVALID			//This code is never returned, can be used for internal signalling
} I2C_STATUS;

/** User-supplied callback when a I2C transaction finished.
 @param refcon values passed in when the transaction was started
 @param status result of the transaction */
typedef void (*I2C_CompletionHandler)(uint8_t i2c, uint32_t refcon, I2C_STATUS status);

typedef struct {
	HW_RW CONSET;	//Control set register
	HW_RO STAT;		//Status register
	HW_RW DAT;		//Data register
	HW_RW ADR0;		//I2C slave register 0
	HW_RW SCLH;		//SCH duty cycle register high half word
	HW_RW SCLL;		//SCL duty cycle register low half word
	HW_WO CONCLR;	//I2C control clear register
	HW_RW MMCTRL;	//Monitor mode control register
	HW_RW ADR1;		//I2C slave address register 1
	HW_RW ADR2;		//I2C slave address register 2
	HW_RW ADR3;		//I2C slave address register 3
	HW_RO DATA_BUF;	//Data buffer register
	HW_RW MASK0;	//I2C Slave address mask register 0
	HW_RW MASK1;	//I2C Slave address mask register 1
	HW_RW MASK2;	//I2C Slave address mask register 2
	HW_RW MASK3;	//I2C Slave address mask register 3
	HW_UU PADDING[64496];	//padding from I2C0 to I2C1
} __attribute__((aligned(4))) I2C_STRUCT;

/** peripheral base address. Padding allows to use I2C[0] or I2C[1]. */
#define I2C ((I2C_STRUCT*)(0x400A1000))

/** values for the CONSET register */
typedef enum {
	I2C_CONSET_AA = 0x04,        //Assert acknopwledge
	I2C_CONSET_SI = 0x08,        //I2C interrupt flag
	I2C_CONSET_STO = 0x10,       //I2C Start flag (master mode)
	I2C_CONSET_STA = 0x20,       //I2C Stop flag (recover from error in slave mode)
	I2C_CONSET_I2EN = 0x40       //I2C enable flag
} I2C_CONSET;

/** values for CONCLR register */
typedef enum {
	I2C_CONCLR_AAC = 0x04,       //Clear assert ack
	I2C_CONCLR_SIC = 0x08,       //Clear interrupt flag
	I2C_CONCLR_STAC = 0x20,      //Clear START flag
	I2C_CONCLR_I2ENC = 0x40,     //Clear I2C enable flag (turn off I2C)
} I2C_CONCLR;

/** State at interrupt time */
typedef enum {
	I2C_STAT_START_SENT         = 0x08, //Start code sent -> send SLAR or SLAW
	I2C_STAT_REP_START_SENT     = 0x10, //Repeated start code sent -> send SLAR or SLAW
	I2C_STAT_SLAW_ACKED	        = 0x18, //Slave address + Write ok -> send data
	I2C_STAT_SLAW_NACKED        = 0x20, //Slave address + Write not ok -> bail (no slave?)
	I2C_STAT_DATA_WRITE_ACKED   = 0x28, //Data write ok -> next, STOP or REPSTART
	I2C_STAT_DATA_WRITE_NACKED  = 0x30, //Data write not ok -> fail
	I2C_STAT_ARBITRATION_LOST   = 0x38, //Arbitration lost (multimaster / nak) -> fail (?)
	I2C_STAT_SLAR_ACKED         = 0x40, //Slave address + Read ok -> read data
	I2C_STAT_SLAR_NACKED        = 0x48, //Slave address + Read not ok -> fail (no slave?)
	I2C_STAT_DATA_READ_ACKED    = 0x50, //Read ok -> next byte (turn off ack for last byte)
	I2C_STAT_DATA_READ_NACKED   = 0x58  //Last read ok -> STOP
} I2C_STAT_CODE;

/** structure for housekeeping of transactions. Make sure that all 0 = off */
typedef struct {
	bool running;                    //true if initialized, false otherwise
	volatile uint32_t refcon;        //user-supplied refcon for this transaction
	volatile uint8_t slaveAddress;   //slave address to talk to
	volatile uint16_t toWrite;       //remaining number of bytes to write
	volatile uint8_t* writeBuffer;   //pointer to next byte to write
	volatile uint16_t toRead;        //remaining number of bytes to read
	volatile uint8_t* readBuffer;    //pointer to next byte to read
	volatile I2C_CompletionHandler completionHandler;  //callback when transaction is completed
} I2C_State;

/** Codes for bus speed */
typedef enum {
	I2C_MODE_STANDARD = 0,	//100 kbps
	I2C_MODE_FAST = 1,		//400 kbps (fast mode)
	I2C_MODE_FASTPLUS = 2	//1 mbps (fast mode plus)
} I2C_MODE;

#ifdef __cplusplus
extern "C" {
#endif

/** Turns on I2C as master in a specific speed mode. Does all setup.
This function assumes that the respective peripheral clock is running at full main speed.
Note that I2C1 can only be used in fast mode.
I2C0 pins are dedicated, so they can be initialized automatically.
I2C1 IO pins are NOT initialized automatically, needs to be done manually (see i2c_configure_pin)
 @param i2c I2C engine (0 or 1)
 @param mode operation speed
 @param state pointer to an uninitialized I2C_State structure in RAM. Must not be null. */
void i2c_init(uint8_t i2c, I2C_MODE mode);

/** configures a pin to use with I2C1. Should be called prior to use for SDA and SCL pin
@param group pin group, e.g. 2 of P2_3
@param idx pin idx, e.g. 3 of P2_3
@param mode I2C mode of pin (see UM10430, SCU), e.g. 1 for P2_3 */
void i2c_configure_pin(uint8_t group, uint8_t idx, uint8_t mode);

//TODO: Function to turn peripheral off

/** Writes data to I2C.
 @param i2c I2C engine (0 or 1)
 @param addr slave address
 @param len number of bytes to write
 @param buf data to write. Must be valid during transaction
 @param handler function to call after completion of transaction
 @param refcon user value that will be passed on to completion handler
 @return status of transmission initiation (does not indicate success of transaction) */
I2C_STATUS i2c_write(uint8_t i2c,
                     uint8_t addr,
                     uint16_t len,
                     uint8_t* buf,
                     I2C_CompletionHandler handler,
                     uint32_t refcon);

/** Reads data from I2C.
 @param i2c I2C engine (0 or 1)
 @param addr slave address
 @param len number of bytes to read
 @param buf buffer to store read data. Must be valid during transaction
 @param handler function to call after completion of transaction
 @param refcon user value that will be passed on to completion handler
 @return status of transmission initiation (does not indicate success of transaction) */
I2C_STATUS i2c_read(uint8_t i2c,
                    uint8_t addr,
                    uint16_t len,
                    uint8_t* buf,
                    I2C_CompletionHandler handler,
                    uint32_t refcon);

/** First writes to, then reads data from I2C (combined transaction)
 @param i2c I2C engine (0 or 1)
 @param addr slave address
 @param writeLen number of bytes to write
 @param writeBuf data to write. Must be valid during transaction
 @param readLen number of bytes to read
 @param readBuf buffer to store read data. Must be valid during transaction
 @param handler function to call after completion of transaction
 @param refcon user value that will be passed on to completion handler
 @return status of transmission initiation (does not indicate success of transaction) */
I2C_STATUS i2c_write_read(uint8_t i2c,
                          uint8_t addr,
                          uint16_t writeLen,
                          uint8_t* writeBuf,
                          uint16_t readLen,
                          uint8_t* readBuf,
                          I2C_CompletionHandler handler,
                          uint32_t refcon);

/** returns whether there is a I2C transaction running or not
 @param i2c I2C engine (0 or 1)
 @return true if there is a pending transaction, false otherwise */
bool i2c_transaction_running(uint8_t i2c);


/** tries to cancel a pending transaction
 @param i2c I2C engine (0 or 1) */
void i2c_cancel_transaction(uint8_t i2c);


// Convenience synchronous API - blocks until transfer is done. Usually easier to use but may be inefficient

/** tries to write to I2C, blocks until transaction failure or completion. May only be used in main thread
or in an interrupt with lower priority than the I2C interrupts.
 @param i2c I2C engine (0 or 1)
 @param addr slave address
 @param len number of bytes to write
 @param buf data to write. Must be valid during transaction
 @return result of transaction */
I2C_STATUS i2c_write_sync(uint8_t i2c,
                          uint8_t addr,
                          uint16_t len,
                          uint8_t* buf);

#ifdef __cplusplus
}
#endif

#endif
