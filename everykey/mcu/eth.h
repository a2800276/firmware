#ifndef _ETH_
#define _ETH_

#include "../core/types.h"
/** LPC18xx ethernet peripheral driver. This module does not cover pin assignment
or extra pins such as power control, reset and/or external interrupts. These
should be implemented in board-specific functions. Note that this implementation
assumes a clock rate of 150-250 MHz. */

typedef struct {
  HW_RW MAC_CONFIG;
  HW_RW MAC_FRAME_FILTER;
  HW_RW MAC_HASHTABLE_HIGH;
  HW_RW MAC_HASHTABLE_LOW;
  HW_RW MAC_MII_ADDR;
  HW_RW MAC_MII_DATA;
  HW_RW MAC_FLOW_CTRL;
  HW_RW MAC_VLAN_TAG;
  HW_UU unused1;
  HW_RO MAC_DEBUG;
  HW_RW MAC_RWAKE_FRFLT;
  HW_RW MAC_PMT_CTRL_STAT;
  HW_UU unused2[2];
  HW_RO MAC_INTR;
  HW_RW MAC_INTR_MASK;
  HW_RW MAC_ADDR0_HIGH;
  HW_RW MAC_ADDR0_LOW;
  HW_UU unused3[430];
  HW_RW MAC_TIMESTP_CTRL;
  HW_RW SUBSECOND_INCR;
  HW_RO SECONDS;
  HW_RO NANOSECONDS;
  HW_RW SECONDSUPDATE;
  HW_RW NANOSECONDSUPDATE;
  HW_RW ADDEND;
  HW_RW TARGETSECONDS;
  HW_RW TARGETNANOSECONDS;
  HW_RW HIGHWORD;
  HW_RO TIMESTAMPSTAT;
  HW_UU unused4[565];
  HW_RW DMA_BUS_MODE;
  HW_RW DMA_TRANS_POLL_DEMAND;
  HW_RW DMA_REC_POLL_DEMAND;
  HW_RW DMA_REC_DES_ADDR;
  HW_RW DMA_TRANS_DES_ADDR;
  HW_RW DMA_STAT;
  HW_RW DMA_OP_MODE;
  HW_RW DMA_INT_EN;
  HW_RO DMA_MFRM_BUFOF;
  HW_RW DMA_REC_INT_WDT;
  HW_UU unused5[8];
  HW_RO DMA_CURHOST_TRANS_DES;
  HW_RO DMA_CURHOST_REC_DES;
  HW_RO DMA_CURHOST_TRANS_BUF;
  HW_RO DMA_CURHOST_REC_BUF;
} __attribute__((aligned(4))) ETH_STRUCT;

#define ETH_HW ((ETH_STRUCT*)(0x40010000))

typedef enum {
  ETH_MAC_CONFIG_RE = 0x04,        //Receiver enable. 1=on
  ETH_MAC_CONFIG_TE = 0x08,        //Transmitter enable. 1=on
  ETH_MAC_CONFIG_DF = 0x10,        //Deferral check (half duplex only)
  ETH_MAC_CONFIG_BL_10 = 0x00,     //Backoff limit (half duplex only)
  ETH_MAC_CONFIG_BL_8 = 0x20,      //Backoff limit (half duplex only)
  ETH_MAC_CONFIG_BL_4 = 0x40,      //Backoff limit (half duplex only)
  ETH_MAC_CONFIG_BL_1 = 0x60,      //Backoff limit (half duplex only)
  ETH_MAC_CONFIG_ACS = 0x80,       //Auto Pac/CRC stripping. 1=strip
  ETH_MAC_CONFIG_LUD = 0x100,      //Link up/down in SMII. 1=link up
  ETH_MAC_CONFIG_DR = 0x200,       //Disable retry (half duplex only). 1=only one transmission, 0=retry.
  ETH_MAC_CONFIG_DM = 0x800,       //Duplex mode. 0=half, 1=full
  ETH_MAC_CONFIG_LM = 0x1000,      //Loopback mode. 1=loopback
  ETH_MAC_CONFIG_DO = 0x2000,      //Disable receive own (half duplex only)
  ETH_MAC_CONFIG_FES = 0x4000,     //Speed. 0=10Mbps, 1=100Mbps
  ETH_MAC_CONFIG_PS = 0x8000,      //Port select. 1=MII/100Mbp. Only 1 allowed.
  ETH_MAC_CONFIG_DCRS = 0x10000,   //Disable carrier sense during transmission (half duplex only)
  ETH_MAC_CONFIG_IFG_96 = 0x00000, //Inter-frame gap 96 bit times
  ETH_MAC_CONFIG_IFG_88 = 0x20000, //Inter-frame gap 88 bit times
  ETH_MAC_CONFIG_IFG_80 = 0x40000, //Inter-frame gap 80 bit times
  ETH_MAC_CONFIG_IFG_72 = 0x60000, //Inter-frame gap 72 bit times
  ETH_MAC_CONFIG_IFG_64 = 0x80000, //Inter-frame gap 64 bit times. Min for half duplex.
  ETH_MAC_CONFIG_IFG_56 = 0xa0000, //Inter-frame gap 56 bit times
  ETH_MAC_CONFIG_IFG_48 = 0xc0000, //Inter-frame gap 48 bit times
  ETH_MAC_CONFIG_IFG_40 = 0xe0000, //Inter-frame gap 40 bit times
  ETH_MAC_CONFIG_JE = 0x100000,    //Jumbo Frame enable. 1=9018 bytes max.
  ETH_MAC_CONFIG_JD = 0x400000,    //Jabber disable 1=send up to 16384 byte frames
  ETH_MAC_CONFIG_WD = 0x800000     //Watchdog Disable. 1=receive up to 16384 byte frames
} ETH_MAC_CONFIG_BITS;

typedef enum {
  ETH_MAC_FRAME_FILTER_PR = 0x01,             //Promiscuous mode
  ETH_MAC_FRAME_FILTER_HUC = 0x02,            //Hash unicast
  ETH_MAC_FRAME_FILTER_HMC = 0x04,            //Hash multicast
  ETH_MAC_FRAME_FILTER_DAIF = 0x08,           //DA inverse filtering
  ETH_MAC_FRAME_FILTER_PM = 0x10,             //Pass all multicast
  ETH_MAC_FRAME_FILTER_DBF = 0x20,            //Disable broadcast filter. 0=pass all, 1=filter
  ETH_MAC_FRAME_FILTER_PCF_FILTER_ALL = 0x00, //Filter all control frames
  ETH_MAC_FRAME_FILTER_PCF_NOPAUSE = 0x40,    //Filter all but pause
  ETH_MAC_FRAME_FILTER_PCF_ALL = 0x80,        //Pass all, no address filtering
  ETH_MAC_FRAME_FILTER_PCF_ADDRESS = 0xc0,    //Pass all that match address filter
  ETH_MAC_FRAME_FILTER_HPF = 0x400,           //Hash or perfect filter
  ETH_MAC_FRAME_FILTER_RA = 0x80000000        //Receive all
} ETH_MAC_FRAME_FILTER_BITS;

typedef enum {
  ETH_MAC_MII_ADDR_GB = 0x01,           //MII busy
  ETH_MAC_MII_ADDR_W = 0x02,            //Write
  ETH_MAC_MII_ADDR_CR_42 = 0x00,        //MDC clock = CLK_Mx_ETHERNET / 42 (60-100MHz)
  ETH_MAC_MII_ADDR_CR_62 = 0x04,        //MDC clock = CLK_Mx_ETHERNET / 62 (100-150MHz)
  ETH_MAC_MII_ADDR_CR_16 = 0x08,        //MDC clock = CLK_Mx_ETHERNET / 16 (20-35MHz)
  ETH_MAC_MII_ADDR_CR_26 = 0x0c,        //MDC clock = CLK_Mx_ETHERNET / 26 (35-60MHz)
  ETH_MAC_MII_ADDR_CR_102 = 0x10,       //MDC clock = CLK_Mx_ETHERNET / 102 (150-250MHz)
  ETH_MAC_MII_ADDR_CR_124 = 0x14,       //MDC clock = CLK_Mx_ETHERNET / 124 (250-300MHz)
  ETH_MAC_MII_ADDR_CR_OUT = 0x20,       //MDC out of recommended spec (1-2.5MHz)
  ETH_MAC_MII_ADDR_CR_MASK = 0x3c,      //MDC mask
  ETH_MAC_MII_ADDR_GR_MASK = 0x7c0,     //MII register in PHY device mask (10..6)
  ETH_MAC_MII_ADDR_GR_SHIFT = 6,        //MII register in PHY device shift (10..6)
  ETH_MAC_MII_ADDR_PA_MASK = 0xf800,    //PHY address mask (15..11)
  ETH_MAC_MII_ADDR_PA_SHIFT = 11,       //PHY address shift (15..11)
} ETH_MAC_MII_ADDR_BITS;

typedef enum {
  ETH_DMA_BUS_MODE_SWR = 0x1,            //trigger SW reset
  ETH_DMA_BUS_MODE_DA = 0x02,            //dma arbitration - 0: round robin (prio bits 15/16), 1: rx over tx
  ETH_DMA_BUS_MODE_DSL_MASK = 0x7c,      //mask for descriptor skip length (2..6)
  ETH_DMA_BUS_MODE_DSL_SHIFT = 2,        //shift for descriptor skip length (2..6)
  ETH_DMA_BUS_MODE_ATDS = 0x80,          //alternate descriptor size - 0: 16 bytes, 1: 32 bytes
  ETH_DMA_BUS_MODE_PBL_MASK = 0x3f00,    //programmable burst length mask (8..13)
  ETH_DMA_BUS_MODE_PBL_SHIFT = 8,        //programmable burst length shift (8..13)
  ETH_DMA_BUS_MODE_PR_1_1 = 0x0000,      //rx-to-tx priority ratio 1:1 (if DA = 0)
  ETH_DMA_BUS_MODE_PR_2_1 = 0x4000,      //rx-to-tx priority ratio 2:1 (if DA = 0)
  ETH_DMA_BUS_MODE_PR_3_1 = 0x8000,      //rx-to-tx priority ratio 3:1 (if DA = 0)
  ETH_DMA_BUS_MODE_PR_4_1 = 0xc000,      //rx-to-tx priority ratio 4:1 (if DA = 0)
  ETH_DMA_BUS_MODE_FB = 0x10000,         //fixed burst
  ETH_DMA_BUS_MODE_RPBL_MASK = 0x7e0000, //RxDMA PBL mask (17..22), only if USP = 1
  ETH_DMA_BUS_MODE_RPBL_SHIFT = 17,      //RxDMA PBL shift (17..22), only if USP = 1
  ETH_DMA_BUS_MODE_USP = 0x800000,       //use separate PBL for Rx. 0: use PBL for Rx, 1: use RPBL for Rx
  ETH_DMA_BUS_MODE_PBL8X = 0x1000000,    //multiply PBL/RPBL by 8
  ETH_DMA_BUS_MODE_AAL = 0x2000000,      //address aligned beats
  ETH_DMA_BUS_MODE_MB = 0x4000000,       //mixed bursts
  ETH_DMA_BUS_MODE_TXPR = 0x8000000      //Tx priority
} ETH_DMA_BUS_MODE_BITS;

typedef enum {
  ETH_DMA_STAT_TI = 0x01,                //Transmit interrupt
  ETH_DMA_STAT_TPS = 0x02,               //Transmit process stopped
  ETH_DMA_STAT_TU = 0x04,                //Transmit buffer unavailable
  ETH_DMA_STAT_TJT = 0x08,               //Transmit jabber timeout
  ETH_DMA_STAT_OVF = 0x10,               //Receive overflow
  ETH_DMA_STAT_UNF = 0x20,               //Transmit underflow
  ETH_DMA_STAT_RI = 0x40,                //Receive interrupt
  ETH_DMA_STAT_RU = 0x80,                //Receive buffer unavailable
  ETH_DMA_STAT_RPS = 0x100,              //Receive process stopped
  ETH_DMA_STAT_RWT = 0x200,              //Receive watchdog timeout
  ETH_DMA_STAT_ETI = 0x400,              //Early transmit interrupt
  ETH_DMA_STAT_FBI = 0x2000,             //Fatal bus error interrupt
  ETH_DMA_STAT_ERI = 0x4000,             //Early receive interrupt
  ETH_DMA_STAT_AIE = 0x8000,             //Abnormal interrupt summary
  ETH_DMA_STAT_NIE = 0x10000,            //Normal interrupt summary
  ETH_DMA_STAT_RS_MASK = 0xe0000,        //Receive process state mask
  ETH_DMA_STAT_RS_STOP = 0x00000,        //Receive: Stopped
  ETH_DMA_STAT_RS_RFTD = 0x20000,        //Receive: Running: Fetching receive transfer descriptor
  ETH_DMA_STAT_RS_WFRP = 0x60000,        //Receive: Running: Waiting for receive packet
  ETH_DMA_STAT_RS_SUSP = 0x80000,        //Receive: Suspended: Receive descriptor unavailable
  ETH_DMA_STAT_RS_CRD = 0xa0000,         //Receive: Running: Closing receive descriptor
  ETH_DMA_STAT_RS_TSW = 0xc0000,         //Receive: TIME_STAMP write state
  ETH_DMA_STAT_RS_TRNS = 0xe0000,        //Receive: Running: Transferring from receive buffer to host memory
  ETH_DMA_STAT_TS_MASK = 0x700000,       //Transmit process state mask
  ETH_DMA_STAT_TS_STOP = 0x000000,       //Transmit: Stopped
  ETH_DMA_STAT_TS_FTTD = 0x100000,       //Transmit: Running: Fetching transmit transfer descriptor
  ETH_DMA_STAT_TS_WFS = 0x200000,        //Transmit: Running: Waiting for status
  ETH_DMA_STAT_TS_TRNS = 0x300000,       //Transmit: Running: Transferring from host memory to transmit buffer
  ETH_DMA_STAT_TS_TSW = 0x400000,        //Transmit: Running: TIME_STAMP write state
  ETH_DMA_STAT_TS_SUSP = 0x600000,       //Transmit: Suspended: Transmit descriptor unavailable or transmit buffer underflow
  ETH_DMA_STAT_TS_CTD = 0x700000,        //Transmit: Running: Closing transmit descriptor
  ETH_DMA_STAT_EB1 = 0x800000,           //Error bit 1 (for FBI=1). 1=error in TxDMA, 0=error in RxDMA
  ETH_DMA_STAT_EB2 = 0x1000000,          //Error bit 2 (for FBI=1). 1=error during read, 0=error during write
  ETH_DMA_STAT_EB3 = 0x2000000,          //Error bit 3 (for FBI=1). 1=error during desc access, 0=error during data access
  ETH_DMA_STAT_ALL = 0x1e7ff             //All clearable and maskable interrupts (0-16, except for 11 and 12)
} ETH_DMA_STAT_BITS;

typedef enum {
  ETH_DMA_OP_MODE_SR = 0x02,             //Start/stop receive. 1=start
  ETH_DMA_OP_MODE_OSF = 0x04,            //Operate on second frame
  ETH_DMA_OP_MODE_RTH_64 = 0x00,         //Receive threshold 64
  ETH_DMA_OP_MODE_RTH_32 = 0x10,         //Receive threshold 32
  ETH_DMA_OP_MODE_RTH_96 = 0x20,         //Receive threshold 96
  ETH_DMA_OP_MODE_RTH_128 = 0x30,        //Receive threshold 128
  ETH_DMA_OP_MODE_FUF = 0x40,            //Forward undersized frames
  ETH_DMA_OP_MODE_FEF = 0x80,            //Forward error frames
  ETH_DMA_OP_MODE_ST = 0x2000,           //Start/stop transmission command
  ETH_DMA_OP_MODE_TTH_64 = 0x0000,       //Transmit threshold 64
  ETH_DMA_OP_MODE_TTH_128 = 0x4000,      //Transmit threshold 128
  ETH_DMA_OP_MODE_TTH_192 = 0x8000,      //Transmit threshold 192
  ETH_DMA_OP_MODE_TTH_256 = 0xc000,      //Transmit threshold 256
  ETH_DMA_OP_MODE_TTH_40 = 0x10000,      //Transmit threshold 40
  ETH_DMA_OP_MODE_TTH_32 = 0x14000,      //Transmit threshold 32
  ETH_DMA_OP_MODE_TTH_24 = 0x18000,      //Transmit threshold 24
  ETH_DMA_OP_MODE_TTH_16 = 0x1c000,      //Transmit threshold 16
  ETH_DMA_OP_MODE_FTF = 0x100000,        //Flush transmit FIFO
  ETH_DMA_OP_MODE_DFF = 0x1000000        //Disable flushing of received frames
} ETH_DMA_OP_MODE_BITS;


#ifdef __cplusplus
extern "C" {
#endif

/** starts up ethernet */
void eth_init();

/** shuts down ethernet */
void eth_shutdown();

void eth_mii_write(uint8_t reg, uint16_t val);

uint16_t eth_mii_read(uint8_t reg);

#ifdef __cplusplus
}
#endif


#endif
