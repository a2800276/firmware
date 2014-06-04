#ifndef _LAN8720A_
#define _LAN8720A_

#include "../core/types.h"

typedef enum {
  LAN8720A_REG_BCR = 0,             //Basic control register
  LAN8720A_REG_BSR = 1,             //Basic status register
  LAN8720A_REG_PHYID1 = 2,          //PHY Identifier 1
  LAN8720A_REG_PHYID2 = 3,          //PHY Identifier 2
  LAN8720A_REG_ANAR = 4,            //Auto-negotiation advertisement register
  LAN8720A_REG_ANLPAR = 5,          //Auto-negotiation link partner ability register
  LAN8720A_REG_ANER = 6,            //Auto-negotiation expansion register
  LAN8720A_REG_MCSR = 17,           //Mode control / status register
  LAN8720A_REG_SM = 18,             //Special modes
  LAN8720A_REG_SECR = 26,           //Symbol error counter register
  LAN8720A_REG_CSIR = 27,           //Control / status indication register
  LAN8720A_REG_ISR = 29,            //Interrupt source register
  LAN8720A_REG_IMR = 30,            //Interrupt mask register
  LAN8720A_REG_PHYSCSR = 31         //PHY special control / status register
} LAN8720A_REGISTER;

typedef enum {
  LAN8720A_BCR_SR = 0x8000,         //Soft reset
  LAN8720A_BCR_LOOPBACK = 0x4000,   //Loopback
  LAN8720A_BCR_SSEL = 0x2000,       //Speed select
  LAN8720A_BCR_ANE = 0x1000,        //Auto-negotiation enable (overrides SSEL and DM)
  LAN8720A_BCR_PD = 0x800,          //Power-down. 0=normal, 1=power down
  LAN8720A_BCR_ISOL = 0x400,        //Isolate. 0=normal, 1=isolate RMII from PHY
  LAN8720A_BCR_RAN = 0x200,         //Restart auto-negotiation
  LAN8720A_BCR_DM = 0x100           //Duplex mode. 0=half, 1=full
} LAN8720A_BCR_BITS;

typedef enum {
  LAN8720A_BSR_100T4 = 0x8000,      //100 Base T4
  LAN8720A_BSR_100TXFULL = 0x4000,  //100 Base TX full duplex
  LAN8720A_BSR_100TXHALF = 0x2000,  //100 Base TX half duplex
  LAN8720A_BSR_10TFULL = 0x1000,    //10 Base T full duplex
  LAN8720A_BSR_10THALF = 0x800,     //10 Base T half duplex
  LAN8720A_BSR_100T2FULL = 0x400,   //100 Base 2 full duplex
  LAN8720A_BSR_100T2HALF = 0x200,   //100 Base 2 half duplex
  LAN8720A_BSR_ES = 0x100,          //Extended status. 1=ext status in reg 15
  LAN8720A_BSR_ANC = 0x20,          //Auto-negotiation complete
  LAN8720A_BSR_RF = 0x10,           //Remote fault
  LAN8720A_BSR_ANA = 0x08,          //Auto-negotiate ability
  LAN8720A_BSR_LS = 0x04,           //Link status
  LAN8720A_BSR_JD = 0x02,           //Jabber detect
  LAN8720A_BSR_EC = 0x01            //Extended capabilities. 1=extended capabilities supported
} LAN8720A_BSR_BITS;

#ifdef __cplusplus
extern "C" {
#endif

/** initializes the PHY. Assumes that the MAC paripheral is initialized. */
void lan_init();

#ifdef __cplusplus
}
#endif

#endif
