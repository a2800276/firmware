#ifndef _USART_
#define _USART_

/** USART 0/2/3, UART 1 */

#include "../core/types.h"

typedef struct {
  HW_RW RBR_THR_DLL;   //0x00: Receiver buffer register (read, DLAB=0),
                       //Transmit holding register (write, DLAB=0),
                       //Divisor Latch LSB (rw, DLAB=1)
  HW_RW DLM_IER;       //0x04: Divisor Latch MSB (rw, DLAB=1),
                       //Interrupt enable register (rw, DLAB=0)
  HW_UU IIR_FCR;       //0x08: Interrupt ID register (read),
                       //FIFO control register (write)
  HW_RW LCR;           //0x0c: Line control register
  HW_RW MCR;           //0x10: Modem control register (UART1 only)
  HW_RO LSR;           //0x14: Line status register
  HW_RO MSR;           //0x18: Modem status register (UART1 only)
  HW_RW SCR;           //0x1c: Scratch pad register
  HW_RW ACR;           //0x20: Auto-baud control register
  HW_RW ICR;           //0x24: IrDA control register (UART3 only)
  HW_RW FDR;           //0x28: Fractional divider register
  HW_RW OSR;           //0x2c: Oversampling register (UART0/2/3 only)
  HW_UU reserved3[4];
  HW_RW HDEN;          //0x40: Half-duplex enable register (UART0/2/3 only)
  HW_UU reserved4;
  HW_RW SCICTRL;       //0x48 Smart card interface control register (UART0/2/3 only)
  HW_RW RS485CTRL;     //0x4c: RS485 control register
  HW_RW RS485ADRMATCH; //0x50: RS485 address match
  HW_RW RS485DLY;      //0x54: RS485 direction control delay
  HW_RW SYNCCTRL;      //0x58: Synchronous control register (UART0/2/3 only)
  HW_RW TER;           //0x5c: Transmit enable register
} __attribute__((aligned(4))) USART_STRUCT;

typedef enum {
  USART_IER_RBRIE          = 0x01, //Receiver buffer ready interrupt enable
  USART_IER_THREIE         = 0x02, //Transmitter holding register empty interrupt enable
  USART_IER_RXIE           = 0x04, //RX line interrupt enable (line status/errors)
  USART_IER_ABEOINTEN      = 0x100 //Auto-baud interrupt enable
} USART_IER_VALUES;

typedef enum {
  USART_FCR_FIFOEN         = 0x01, //FIFO enable. 1 for operation. Transition clears FIFOs.
  USART_FCR_RXFIFO_RES     = 0x02, //Write 1 to reset RX FIFO
  USART_FCR_TXFIFO_RES     = 0x04, //Write 1 to reset TX FIFO
  USART_FCR_DMAMODE        = 0x08, //DMA mode
  USART_FCR_RXTRIGLVL_MASK = 0xc0, //RX interrupt trigger level
  USART_FCR_RXTRIGLVL_1    = 0x00, //trigger RX interrupt at 1 byte
  USART_FCR_RXTRIGLVL_4    = 0x40, //trigger RX interrupt at 4 bytes
  USART_FCR_RXTRIGLVL_8    = 0x80, //trigger RX interrupt at 8 bytes
  USART_FCR_RXTRIGLVL_14   = 0xc0 //trigger RX interrupt at 14 bytes
} USART_FCR_VALUES;

typedef enum {
  USART_LCR_WLS_MASK        = 0x03, //Word length select: Character length in bits
  USART_LCR_WLS_5           = 0x00, //5 bits per character
  USART_LCR_WLS_6           = 0x01, //6 bits per character
  USART_LCR_WLS_7           = 0x02, //7 bits per character
  USART_LCR_WLS_8           = 0x03, //8 bits per character
  USART_LCR_SBS             = 0x04, //Stop bit select. 0=1 stop bit, 1=2 stop bits (1.5 for LCR=5)
  USART_LCR_PE              = 0x08, //Parity enable (generation and checking). 0=off, 1=on
  USART_LCR_PS_MASK         = 0x30, //Parity select: Type of parity (if PE is on)
  USART_LCR_PS_ODD          = 0x00, //Odd parity
  USART_LCR_PS_EVEN         = 0x10, //Even parity
  USART_LCR_PS_ONE          = 0x20, //Parity bit always one
  USART_LCR_PS_ZERO         = 0x30, //Parity bit always zero
  USART_LCR_BC              = 0x40, //Break control: 0=normal, 1=break TX (force 0)
  USART_LCR_DLAB            = 0x80  //Divisor Latch access: 0=disabled, 1=enabled (remap hw registers)
} USART_LCR_VALUES;

typedef enum {
  USART_LSR_RDR            = 0x001, //Receiver data ready. 1=Unread data in RDR
  USART_LSR_OE             = 0x002, //Overrun error, cleared on read
  USART_LSR_PE             = 0x004, //Parity error
  USART_LSR_FE             = 0x008, //Framing error
  USART_LSR_BI             = 0x010, //Break interrupt
  USART_LSR_THRE           = 0x020, //Transmitter holding register empty
  USART_LSR_TEMT           = 0x040, //Transmitter empty
  USART_LSR_RXFE           = 0x080, //Error in RX FIFO
  USART_LSR_TXERR          = 0x100  //NACK in smart card T=0 mode
} USART_LSR_VALUES;

typedef enum {
  USART_SYNCCTRL_SYNC       = 0x01, //Enable synchronous mode
  USART_SYNCCTRL_CSRC       = 0x02, //Clock source: 0=slave (clock in), 1=master (clock out)
  USART_SYNCCTRL_FES        = 0x04, //Falling edge sampling: 0=RXD sample on rising, 1=on falling clk edge
  USART_SYNCCTRL_TSBYPASS   = 0x08, //1=bypass tx clock sync (higher rates, less robust)
  USART_SYNCCTRL_CSCEN      = 0x10, //Continuous TX clock. clock out: 0=only on data, 1=always
  USART_SYNCCTRL_SSSDIS     = 0x20, //Start/stop bit disable. 0=send/receive, 1=no start/stop bits
  USART_SYNCCTRL_CCCLR      = 0x40  //Continuous clock clear. 1=clear CSCEN after char receive
} USART_SYNCCTRL_VALUES;


//Unfortunately, base addresses are not evenly spaced
#define USART0_HW ((USART_STRUCT*)(0x40081000))
#define UART1_HW  ((USART_STRUCT*)(0x40082000))
#define USART2_HW ((USART_STRUCT*)(0x400C1000))
#define USART3_HW ((USART_STRUCT*)(0x400C2000))

#ifdef __cplusplus
extern "C" {
#endif

//API
typedef enum {
  UART_PARITY_NONE = 0x00,
  UART_PARITY_ODD = 0x08,
  UART_PARITY_EVEN = 0x18,
  UART_PARITY_ONE = 0x28,
  UART_PARITY_ZERO = 0x38
} UART_PARITY;

typedef void (*UART_receive_callback)(uint8_t uart);

//error is an or-ed bitfield of  USART_LSR_OE, USART_LSR_PE, USART_LSR_FE and USART_LSR_BI
typedef void (*UART_error_callback)(uint8_t uart, uint8_t error_bitfield);

typedef void (*UART_sent_callback)(uint8_t uart);

/** FIXME: divider calculation is erroneous in some cases (see .c file) */

/** inits the given USART as a synchronous master. No start or stop bits.
@param idx USART hardware index (0, 2 or 3)
@param bpc bits per character
@param baud serial speed in bps. Will try to get near that. Assumes MAIN_CLOCK_MHZ.
@param oversamling (uart clocks per bit clock) in 1/8 steps. Allowed: 8-128. (=Oversampling 1-16). Default 128.
@param trigLvl trigger level for rx. One of USART_FCR_RXTRIGLVL_xx values
@param receive_cb callback on receive data. Should read_avail. Called from interrupt, be quick.
@param sent_cb callback on sent data. Should refill buffer. Called from interrupt, be quick.
@param error_cb callback on error. Should handle somehow. Called from interrupt, be quick.
@discussion Sync master mode is not only useful for USART communication but may be used
as a buffered, fine-grained output. Reducing Oversampling may be useful for finer timing fidelity. 
Pins must be configured manually. */
void usart_init_sync_master(uint8_t idx, uint8_t bpc, uint32_t baud, uint8_t oversampling,
                            uint8_t trigLvl, UART_receive_callback receive_cb,
                            UART_sent_callback sent_cb, UART_error_callback error_cb);

/** inits the given USART 
@param idx USART hardware index (0-3)
@param bpc bits per character
@param baud serial speed in bps. Will try to get near that. Assumes MAIN_CLOCK_MHZ.
@param parity parity
@param longstop if true, use 2 stop bits (1.5 for 5bpc), false for 1 stop bit
@param oversamling (uart clocks per bit clock) in 1/8 steps. Allowed: 8-128. (=Oversampling 1-16). Default 128.
@param trigLvl trigger level for rx. One of USART_FCR_RXTRIGLVL_xx values
@param receive_cb callback on receive data. Should read_avail. Called from interrupt, be quick.
@param sent_cb callback on sent data. Should refill buffer. Called from interrupt, be quick.
@param error_cb callback on error. Should handle somehow. Called from interrupt, be quick.
@discussion Oversampling may be useful for finer timing fidelity.
Pins must be configured manually. */
void usart_init_async(uint8_t idx, uint8_t bpc, uint32_t baud, UART_PARITY parity, bool longstop,
                      uint8_t oversampling, uint8_t trigLvl,
                      UART_receive_callback receive_cb, UART_sent_callback sent_cb,
                      UART_error_callback error_cb);

/** turns off the given USART
@param idx USART hardware index (0-3) */
void usart_shutdown(uint8_t idx);

/** configures a pin to use with a USART. Should be called prior to using the pin
@param group pin group, e.g. 2 of P2_3
@param idx pin idx, e.g. 3 of P2_3
@param mode USART mode of pin (see UM10430, SCU)
@param input set to true if this is an input (enable input buffer)
@param fast set to true for high frequencies (fast slope, disable glitch filter) */
void usart_configure_pin(uint8_t group, uint8_t idx, uint8_t mode, bool input, bool fast);

/** sends a byte buffer synchronously (does not refer to UART/USART, blocks while sending).
THIS METHOD IS HIGHLY INEFFICIENT AND SHOULD NOT BE USED - USE sent_cb / write_avail INSTEAD.
@param idx USART hardware index (0-3)
@param buf buffer containing characters (LSB first)
@param len number of characters to send. */
void usart_write_sync(uint8_t idx, uint8_t* buf, uint16_t len);

/** reads available bytes from an UART. Does not block.
@param idx UART/USART hardware index (0-3)
@param buf buffer to hold results. If NULL, results will be discarded
@param maxlen maximum number of bytes to read
@return number of bytes actually read */
uint16_t usart_read_avail(uint8_t idx, uint8_t* buf, uint16_t maxlen);

/** writes available bytes to an UART. Does not block.
@param idx UART/USART hardware index (0-3)
@param buf buffer containing data to write. If NULL, zero bytes will be written
@param maxlen maximum number of bytes to write
@return number of bytes actually written */
uint16_t usart_write_avail(uint8_t idx, uint8_t* buf, uint16_t maxlen);

#ifdef __cplusplus
}
#endif

#endif
