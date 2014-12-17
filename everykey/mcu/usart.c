
#include "usart.h"
#include "../board.h"
#include "../utils/progfault.h"
#include "clocks.h"
#include "rgu.h"
#include "nvic.h"

typedef struct {
  UART_receive_callback receive_callback;
  UART_sent_callback sent_callback;
  UART_error_callback error_callback;
} UART_CONFIG;

UART_CONFIG uartconf[4];

const USART_STRUCT* USART_LOOKUP[4] = { USART0_HW, UART1_HW, USART2_HW, USART3_HW };


void uart_interrupt_handler(uint8_t idx, USART_STRUCT* hw) {
  nvic_clear_interrupt_pending(NVIC_USART0 + idx);
  uint32_t iir = hw->IIR_FCR;
  uint8_t intid = (iir >> 1) & 0x07;
  uint32_t lsr = hw->LSR;
  
  //errors present
  if ((intid == 3) || (lsr & 0x1e)) {
    if (uartconf[idx].error_callback) {
      uartconf[idx].error_callback(idx, lsr & 0x1e);
    }
  }

  if ((intid == 2) || (intid == 6) || (lsr & USART_LSR_RDR)) {
     if (uartconf[idx].receive_callback) {
        uartconf[idx].receive_callback(idx);
     }
  }
  if ((intid == 1) || (lsr & USART_LSR_THRE)) {
    if (uartconf[idx].sent_callback) {
       uartconf[idx].sent_callback(idx);
    }
  }
}

void usart0_handler(void) {
  uart_interrupt_handler(0, USART0_HW);
}

void uart1_handler(void) {
  uart_interrupt_handler(1, UART1_HW);
}

void usart2_handler(void) {
  uart_interrupt_handler(2, USART2_HW);
}

void usart3_handler(void) {
  uart_interrupt_handler(3, USART3_HW);
}


void usart_find_divisor(uint32_t clock, uint32_t baud, uint8_t oversampling, uint16_t* outLatch, uint8_t* outDivMul, uint8_t* outDivAdd) {
    /* Approach: Test all fractional values, find lower and upper latch for each,
     calculate clock from ideal baud rate, remember best result. Disable fractional if divAdd = 0
     oversampling has unit 1/8

     baud = clock / ((oversampling/8) * latch * (1 + divAdd/divMul))
     baud = clock / ((oversampling/8) * latch * (divMul + divAdd)/divMul)
     baud = 8 * divMul * clock / (oversampling * latch * (divMul + divAdd))
     clock = baud * (oversampling * latch * (divMul + divAdd)) / (8 * divMul)
     8 * clock = baud * oversampling * latch * (divMul + divAdd) / divMul

     baud = clock / ((oversampling/8) * latch * (1 + divAdd/divMul))
     latch = 8 * clock / (oversampling * baud * (1 + divAdd/divMul))
     latch = 8 * clock / (oversampling * baud * ((divMul + divAdd)/divMul))
     latch = 8 * divMul * clock / (oversampling * baud * (divMul + divAdd))

     latch * (1 + divAdd/divMul)) = clock / baud / (oversampling/8)
     latch = clock / baud / (oversampling/8) / (1 + divAdd/divMul)
     latch = 8 * clock / baud / oversampling / (1 + divAdd/divMul)
     latch = 8 * clock / baud / oversampling / ((divMul + divAdd) / divMul)
     latch = 8 * divMul * clock / baud / oversampling / (divMul + divAdd)
     latch = 8 * divMul * clock / (baud * oversampling * (divMul + divAdd))
     */

    /* Pre-scaling of clock and baud to avoid overflows:
     To find the latch, 8 * divMul * clock has to fit into 32 bits.
     Worst case: 8: 4 bits, divMul:4 bits --> Clock: 32-8 bits = 24 bits max.

     We shift both baud and clock right until this value fits.

     This will hopefully also prevent the clock calculation from overflowing...
    */

    while (clock > 0x00ffffff) {
        clock >>= 1;
        baud >>= 1;
    }

    uint16_t bestLatch = 0;
    uint8_t bestDivMul = 0;
    uint8_t bestDivAdd = 0;
    uint32_t bestError = 0xffffffff;

    uint8_t divMul, divAdd;
    for (divMul = 1; divMul <= 15; divMul++) {
        for (divAdd = 0; divAdd < divMul; divAdd++) {
            uint16_t lowerLatch = 8 * divMul * clock / (baud * oversampling * (divMul + divAdd));
            uint16_t upperLatch = lowerLatch + 1;
            if ((lowerLatch >= 1) && ((divAdd==0) || (lowerLatch >= 3))) { //latch min 1, 3 for fractional divider
                uint32_t testClock = baud * oversampling * lowerLatch * (divMul + divAdd) / divMul;
                uint32_t testError = (testClock > 8*clock) ? (testClock - 8*clock) : (8*clock - testClock);
                if (testError < bestError) {
                    bestLatch = lowerLatch;
                    bestDivMul = divMul;
                    bestDivAdd = divAdd;
                    bestError = testError;
                }
            }
            if ((upperLatch >= 1) && ((divAdd==0) || (upperLatch >= 3))) { //latch min 1, 3 for fractional divider
                uint32_t testClock = baud * oversampling * upperLatch * (divMul + divAdd) / divMul;
                uint32_t testError = (testClock > 8*clock) ? (testClock - 8*clock) : (8*clock - testClock);
                if (testError < bestError) {
                    bestLatch = upperLatch;
                    bestDivMul = divMul;
                    bestDivAdd = divAdd;
                    bestError = testError;
                }
            }
        }
    }
    if (bestLatch == 0) {

        progfault(ILLEGAL_ARGUMENT);  //Could not be solved - must be ridiculous baud value
    }
    if (outLatch) {
        *outLatch = bestLatch;
    }
    if (outDivMul) {
        *outDivMul = bestDivMul;
    }
    if (outDivAdd) {
        *outDivAdd = bestDivAdd;
    }
}

void usart_init_sync_master(uint8_t idx, uint8_t bpc, uint32_t baud, uint8_t oversampling,
                            uint8_t trigLvl, UART_receive_callback receive_cb,
                            UART_sent_callback sent_cb, UART_error_callback error_cb) {
  if (bpc < 5 || bpc > 8) {
    progfault(ILLEGAL_ARGUMENT);
  }
  if ((trigLvl != USART_FCR_RXTRIGLVL_1) && (trigLvl != USART_FCR_RXTRIGLVL_4) &&
      (trigLvl != USART_FCR_RXTRIGLVL_8) && (trigLvl != USART_FCR_RXTRIGLVL_14)) {
    progfault(ILLEGAL_ARGUMENT);
  }

  USART_STRUCT* hw = NULL;
  RGU_RESET_INDEX rst = 0;
  switch (idx) {
    case 0:
      clock_set_source(BASE_UART0_CLK, true, CLKSRC_PLL1);  //base clock = main clock
      clock_enable_opts(CLK_Mx_USART0, true, true, true); //Turn on register interface clock
      clock_enable_opts(CLK_APB0_USART0, true, true, true);  //derived clock for IO
      hw = USART0_HW;
      rst = UART0_RST;
    break;
    case 2:
      clock_set_source(BASE_UART2_CLK, true, CLKSRC_PLL1);  //base clock = main clock
      clock_enable_opts(CLK_Mx_USART2, true, true, true); //Turn on register interface clock
      clock_enable_opts(CLK_APB2_USART2, true, true, true);  //derived clock for IO
      hw = USART2_HW;
      rst = UART2_RST;
    break;
    case 3:
      clock_set_source(BASE_UART3_CLK, true, CLKSRC_PLL1);  //base clock = main clock
      clock_enable_opts(CLK_Mx_USART3, true, true, true); //Turn on register interface clock
      clock_enable_opts(CLK_APB2_USART3, true, true, true);  //derived clock for IO
      hw = USART3_HW;
      rst = UART3_RST;
      break;
    default:
      progfault(ILLEGAL_ARGUMENT);
  }

  uartconf[idx].receive_callback = receive_cb;
  uartconf[idx].sent_callback = sent_cb;
  uartconf[idx].error_callback = error_cb;
 
  //Calculate clock dividers
  uint8_t divAddVal;
  uint8_t divMulVal;
  uint16_t divLatchVal;
  usart_find_divisor(1000000*MAIN_CLOCK_MHZ, baud, oversampling, &divLatchVal, &divMulVal, &divAddVal);
/*
  divAddVal = 1;
  divMulVal = 2;
  divLatchVal = 3;
*/
  //Set DLAB
  hw->LCR = USART_LCR_DLAB; //Enable access to to divisor latch
  hw->DLM_IER = (divLatchVal >> 8) & 0xff;
  hw->RBR_THR_DLL = divLatchVal & 0xff;
  hw->LCR = 0; //Finished with divisor latch

  //Config UART
  hw->IIR_FCR = USART_FCR_FIFOEN | USART_FCR_RXFIFO_RES | USART_FCR_TXFIFO_RES | trigLvl;

  //enable receive, transmit and status interrupts
  hw->DLM_IER = USART_IER_RBRIE | USART_IER_THREIE | USART_IER_RXIE;

  uint16_t wls = bpc - 5;
  hw->LCR = wls;
  hw->ACR = 0;             //Default: no auto baud rate
  if (idx == 3) {
    hw->ICR = 0;           //Disable IrDA (USART3 only)
  }
  hw->FDR = (divAddVal & 0x0f) | ((divMulVal & 0x0f) << 3);
  uint8_t osFull = ((oversampling >> 3) - 1) & 0x0f;
  uint8_t osFrac = oversampling & 0x07;
  hw->OSR = (osFull << 4) | (osFrac << 1);

  hw->SYNCCTRL = USART_SYNCCTRL_SYNC | USART_SYNCCTRL_CSRC | USART_SYNCCTRL_SSSDIS;
  nvic_enable_interrupt(NVIC_USART0 + idx); //Enable interrupts
}

void usart_init_async(uint8_t idx, uint8_t bpc, uint32_t baud, UART_PARITY parity,
                      bool longstop, uint8_t oversampling, uint8_t trigLvl,
                      UART_receive_callback receive_cb, UART_sent_callback sent_cb,
                      UART_error_callback error_cb) {
  if (bpc < 5 || bpc > 8) {
    progfault(ILLEGAL_ARGUMENT);
  }
  if ((trigLvl != USART_FCR_RXTRIGLVL_1) && (trigLvl != USART_FCR_RXTRIGLVL_4) &&
      (trigLvl != USART_FCR_RXTRIGLVL_8) && (trigLvl != USART_FCR_RXTRIGLVL_14)) {
    progfault(ILLEGAL_ARGUMENT);
  }

  USART_STRUCT* hw = NULL;
  RGU_RESET_INDEX rst = 0;
  switch (idx) {
    case 0:
      clock_set_source(BASE_UART0_CLK, true, CLKSRC_PLL1);  //base clock = main clock
      clock_enable_opts(CLK_Mx_USART0, true, true, true); //Turn on register interface clock
      clock_enable_opts(CLK_APB0_USART0, true, true, true);  //derived clock for IO
      hw = USART0_HW;
      rst = UART0_RST;
      break;
    case 1:
      clock_set_source(BASE_UART1_CLK, true, CLKSRC_PLL1);  //base clock = main clock
      clock_enable_opts(CLK_Mx_UART1, true, true, true); //Turn on register interface clock
      clock_enable_opts(CLK_APB0_USART1, true, true, true);  //derived clock for IO
      hw = UART1_HW;
      rst = UART1_RST;
      break;
    case 2:
      clock_set_source(BASE_UART2_CLK, true, CLKSRC_PLL1);  //base clock = main clock
      clock_enable_opts(CLK_Mx_USART2, true, true, true); //Turn on register interface clock
      clock_enable_opts(CLK_APB2_USART2, true, true, true);  //derived clock for IO
      hw = USART2_HW;
      rst = UART2_RST;
      break;
    case 3:
      clock_set_source(BASE_UART3_CLK, true, CLKSRC_PLL1);  //base clock = main clock
      clock_enable_opts(CLK_Mx_USART3, true, true, true); //Turn on register interface clock
      clock_enable_opts(CLK_APB2_USART3, true, true, true);  //derived clock for IO
      hw = USART3_HW;
      rst = UART3_RST;
      break;
    default:
      progfault(ILLEGAL_ARGUMENT);
  }

  uartconf[idx].receive_callback = receive_cb;
  uartconf[idx].sent_callback = sent_cb;
  uartconf[idx].error_callback = error_cb;

  //Calculate clock dividers
  uint8_t divAddVal;
  uint8_t divMulVal;
  uint16_t divLatchVal;
  usart_find_divisor(1000000*MAIN_CLOCK_MHZ, baud, oversampling, &divLatchVal, &divMulVal, &divAddVal);

  //Set DLAB
  hw->LCR = USART_LCR_DLAB; //Enable access to to divisor latch
  hw->DLM_IER = (divLatchVal >> 8) & 0xff;
  hw->RBR_THR_DLL = divLatchVal & 0xff;
  hw->LCR = 0; //Finished with divisor latch

  //Config UART
  hw->IIR_FCR = USART_FCR_FIFOEN | USART_FCR_RXFIFO_RES | USART_FCR_TXFIFO_RES | trigLvl;

  //enable receive, transmit and status interrupts
  hw->DLM_IER = USART_IER_RBRIE | USART_IER_THREIE | USART_IER_RXIE;

  uint16_t wls = bpc - 5;
  hw->LCR = wls | parity;
  hw->ACR = 0;             //Default: no auto baud rate
  if (idx == 3) {
    hw->ICR = 0;           //Disable IrDA (USART3 only)
  }
  hw->FDR = (divAddVal & 0x0f) | ((divMulVal & 0x0f) << 3);
  uint8_t osFull = ((oversampling >> 3) - 1) & 0x0f;
  uint8_t osFrac = oversampling & 0x07;
  hw->OSR = (osFull << 4) | (osFrac << 1);

  if (idx != 1) { //If this is a USART, disable synchronous mode
    hw->SYNCCTRL = 0;
  }
  
  nvic_enable_interrupt(NVIC_USART0 + idx); //Enable interrupts
}


void usart_shutdown(uint8_t idx) {
  switch (idx) {
    case 0:
      clock_enable(CLK_Mx_USART0, false);	//Turn off register interface clock
      clock_enable(CLK_APB0_USART0, false);  //derived clock for IO
      clock_set_source(BASE_UART0_CLK, false, CLKSRC_PLL1);  //base clock off
      break;
    case 1:
      clock_enable(CLK_Mx_UART1, false); //Turn off register interface clock
      clock_enable(CLK_APB0_USART1, false);  //derived clock for IO
      clock_set_source(BASE_UART1_CLK, false, CLKSRC_PLL1);  //base clock off
      break;
    case 2:
      clock_enable(CLK_Mx_USART2, false); //Turn off register interface clock
      clock_enable(CLK_APB2_USART2, false);  //derived clock for IO
      clock_set_source(BASE_UART2_CLK, false, CLKSRC_PLL1);  //base clock off
      break;
    case 3:
      clock_enable(CLK_Mx_USART3, false);	//Turn off register interface clock
      clock_enable(CLK_APB2_USART3, false);  //derived clock for IO
      clock_set_source(BASE_UART3_CLK, false, CLKSRC_PLL1);  //base clock off
      break;
  default:
    progfault(ILLEGAL_ARGUMENT);
  }
}

void usart_configure_pin(uint8_t group, uint8_t idx, uint8_t mode, bool input, bool fast) {
  scu_set_pin_mode(group, idx, mode);
  scu_set_pin_pullup(group, idx, !input); //enable pullups for input to avoid erroneous input for unconnected ports
  scu_set_pin_pulldown(group, idx, false);
  scu_enable_pin_in_buffer(group, idx, input);
  scu_enable_pin_glitch_filter(group, idx, !fast);
  scu_set_pin_slew_rate(group, idx, fast);
}

#include "../utils/simpleio.h"
#define LED2_PIN P7_5_AS_GPIO_3_13

void usart_write_sync(uint8_t idx, uint8_t* buf, uint16_t len) {
  USART_STRUCT* hw = NULL;
  switch (idx) {
    case 0:
      hw = USART0_HW;
    break;
    case 1:
      hw = UART1_HW;
    break;
    case 2:
      hw = USART2_HW;
    break;
    case 3:
      hw = USART3_HW;
      break;
    default:
      progfault(ILLEGAL_ARGUMENT);
  }

  //TODO: Need to check timeout? ******
  uint16_t sent = 0;
  for (sent = 0; sent < len; sent++) {
    while (!(hw->LSR & USART_LSR_THRE)) {}
    hw->RBR_THR_DLL = buf[sent];
  }
}

uint16_t usart_read_avail(uint8_t idx, uint8_t* buf, uint16_t maxlen) {
  //Note: no idx range test (should be added if not too inefficient) ******
  USART_STRUCT* hw = (USART_STRUCT*)USART_LOOKUP[idx];
  int i=0;
  while (((hw->LSR) & USART_LSR_RDR) && (i < maxlen)) {
    if (buf) {
      buf[i++] = hw->RBR_THR_DLL;
    } else {
      hw->RBR_THR_DLL;
    }
  }
  return i;
}

uint16_t usart_write_avail(uint8_t idx, uint8_t* buf, uint16_t maxlen) {
  //Note: no idx range test (should be added if not too inefficient) ******
  USART_STRUCT* hw = (USART_STRUCT*)USART_LOOKUP[idx];
  int i=0;
  while (((hw->LSR) & USART_LSR_THRE) && (i < maxlen)) {
    if (buf) {
      hw->RBR_THR_DLL = buf[i++];
    } else {
      hw->RBR_THR_DLL = 0;
    }
  }
  return i;
}

