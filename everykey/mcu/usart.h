#ifndef _USART_
#define _USART_

/** USART 0/2/3 */

#include "../core/types.h"

typedef struct {
  HW_RW RBR_THR_DLL;   //Receiver buffer register (read, DLAB=0),
                       //Transmit holding register (write, DLAB=0),
                       //Divisor Latch LSB (rw, DLAB=1)
  HW_RW DLM_IER;       //Divisor Latch MSB (rw, DLAB=1),
                       //Interrupt enable register (rw, DLAB=0)
  HW_UU IIR_FCR;       //Interrupt ID register (read),
                       //FIFO control register (write)
  HW_RW LCR;           //Line control register
  HW_UU reserved1;
  HW_RO LSR;           //Line status register
  HW_UU reserved2;
  HW_RW SCR;           //Scratch pad register
  HW_RW ACR;           //Auto-baud control register
  HW_RW ICR;           //IrDA control register (UART3 only)
  HW_RW FDR;           //Fractional divider register
  HW_RW OSR;           //Oversampling register
  HW_UU reserved3[4];
  HW_RW HDEN;          //Half-duplex enable register
  HW_UU reserved4;
  HW_RW SCICTRL;       //Smart card interface control register
  HW_RW RS485CTRL;     //RS485 control register
  HW_RW RS485ADRMATCH; //RS485 address match
  HW_RW RS485DLY;      //RS485 direction control delay
  HW_RW SYNCCTRL;      //Synchronous control register
  HW_RW TER;           //Transmit enable register
} __attribute__((aligned(4))) USART_STRUCT;

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
#define USART2_HW ((USART_STRUCT*)(0x400C1000))
#define USART3_HW ((USART_STRUCT*)(0x400C2000))

#ifdef __cplusplus
extern "C" {
#endif

//API

/** inits the given USART as a synchronous master. No start or stop bits.
@param idx USART hardware index (0, 2 or 3)
@param pbc bits per character
@param baud serial speed in bps. Will try to get near that. Assumes MAIN_CLOCK_MHZ.
@param oversamling (uart clocks per bit clock) in 1/8 steps. Allowed: 8-128. (=Oversampling 1-16). Default 128.
@discussion Sync master mode is not only useful for USART communication but may be used
as a buffered, fine-grained output. Reducing Oversampling may be useful for finer timing fidelity.  */
void usart_init_sync_master(uint8_t idx, uint8_t bpc, uint32_t baud, uint8_t oversampling);

/** turns off the given USART
@param idx USART hardware index (0, 2 or 3) */
void usart_shutdown(uint8_t idx);

/** configures a pin to use with a USART. Should be called prior to using the pin
@param group pin group, e.g. 2 of P2_3
@param idx pin idx, e.g. 3 of P2_3
@param mode USART mode of pin (see UM10430, SCU)
@param input set to true if this is an input (enable input buffer)
@param fast set to true for high frequencies (fast slope, disable glitch filter) */
void usart_configure_pin(uint8_t group, uint8_t idx, uint8_t mode, bool input, bool fast);

/** sends a byte buffer synchronously (does not refer to UART/USART, blocks while sending).
@param idx USART hardware index (0, 2 or 3)
@param buf buffer containing characters (LSB first)
@param len number of characters to send. */
void usart_write_sync(uint8_t idx, uint8_t* buf, uint16_t len);


#ifdef __cplusplus
}
#endif

#endif
