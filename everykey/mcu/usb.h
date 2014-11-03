#ifndef _USB_H_
#define _USB_H_

#include "../core/types.h"

typedef struct {
  HW_UU reserved1[64];
  HW_RO CAPLENGTH;                      //100
  HW_RO HCSPARAMS;                      //104
  HW_RO HCCPARAMS;                      //108
  HW_UU reserved2[5];                   //10c-11c
  HW_RO DCIVERSION;                     //120
  HW_RO DCCPARAMS;                      //124
  HW_UU reserved3[6];                   //128-13c
  HW_RW USBCMD_D_H;                     //140
  HW_RW USBSTS_D_H;                     //144
  HW_RW USBINTR_D_H;                    //148
  HW_RW FRINDEX_D_H;                    //14c
  HW_UU reserved4;                      //150
  HW_RW DEVICEADDR_PERIODICLISTBASE;    //154
  HW_RW ENDPOINTLISTADDR_ASYNCLISTADDR; //158
  HW_RW TTCTRL;                         //15c
  HW_RW BURSTSIZE;                      //160
  HW_RW TXFILLTUNING;                   //164
  HW_UU reserved5[2];                   //168-16c
  HW_RW ULPIVIEWPORT;                   //170 - USB1 only. Reserved on USB0
  HW_RW BINTERAL;                       //174
  HW_RW ENDPTNAK;                       //178
  HW_RW ENDPTNAKEN;                     //17c
  HW_UU reserved6;                      //180
  HW_RW POERSC1_D_H;                    //184
  HW_UU reserved7[11];                  //188-1a0
  HW_RW OTGSC;                          //1a4 - USB0 only. Reserved on USB1
  HW_RW USBMODE_D_H;                    //1a8
  HW_RW ENTPTSETUPSTAT;                 //1ac
  HW_RW ENTPTPRIME;                     //1b0
  HW_RW ENDPTFLUSH;                     //1b4
  HW_RO ENTPTSTAT;                      //1b8
  HW_RW ENDPTCOMPLETE;                  //1bc
  HW_RW ENDPTCTRL[6];                   //1c0-1d4. 6 on USB0. 4 on USB1.
  HW_UU padding[906];                   //1d8-ffc. Padding from USB0 to USB1
} USB_STRUCT;

#define USB_HW ((USB_STRUCT*)(0x40006000))

#endif
