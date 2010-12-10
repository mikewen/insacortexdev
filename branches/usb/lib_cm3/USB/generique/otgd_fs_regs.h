/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : otgd_fs_regs.h
* Author             : STMicroelectronics
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : USB OTG IP hardware registers.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifndef __USB_OTG_REGS_H__
#define __USB_OTG_REGS_H__

#ifdef STM32F10X_CL

#include "stm32f10x.h"
#include "usb_type.h"

#define USB_OTG_FS_BASE_ADDR             0x50000000

#define USB_OTG_CORE_GLOBAL_REGS_OFFSET      0x0000
#define USB_OTG_DEV_GLOBAL_REG_OFFSET        0x0800
#define USB_OTG_DEV_IN_EP_REG_OFFSET         0x0900
#define USB_OTG_EP_REG_OFFSET                0x0020
#define USB_OTG_DEV_OUT_EP_REG_OFFSET        0x0B00
#define USB_OTG_PCGCCTL_OFFSET               0x0E00
#define USB_OTG_DATA_FIFO_OFFSET             0x1000
#define USB_OTG_DATA_FIFO_SIZE               0x1000

#define NUM_TX_FIFOS                         4
/*******************************************************************************
 * USB_OTG Core registers .
 * The USB_OTG_USB_OTG_FS_REGS structure defines the size
 * and relative field offsets for the Core Global registers.
 ******************************************************************************/
typedef struct _USB_OTG_GREGS  //000h
{

  volatile u32 GOTGCTL;      /* USB_OTG Control and Status reg     000h*/
  volatile u32 GOTGINT;      /* USB_OTG Interrupt Register         004h*/
  volatile u32 GAHBCFG;      /* Core AHB Configuration Register    008h*/
  volatile u32 GUSBCFG;      /* Core USB Configuration Register    00Ch*/
  volatile u32 GRSTCTL;      /* Core Reset Register                010h*/
  volatile u32 GINTSTS;      /* Core Interrupt Register            014h*/
  volatile u32 GINTMSK;      /* Core Interrupt Mask Register       018h*/
  volatile u32 GRXSTSR;      /* Receive Sts Q Read Register        01Ch*/
  volatile u32 GRXSTSP;      /* Receive Sts Q Read & POP Register  020h*/
  volatile u32 GRXFSIZ;      /* Receive FIFO Size Register         024h*/
  volatile u32 DIEPTXF0;      /* EP0 Tx FIFO Size Register          028h*/
  volatile u32 HNPTXSTS;     /* Non Periodic Tx FIFO/Queue Sts reg 02Ch*/
  u32 Reserved30[2];     /* Reserved                           030h*/
  volatile u32 GCCFG;        /* General Core configuration reg     038h*/
  volatile u32 CID;          /* User ID Register                   03Ch*/
  u32  reserved[48];          /* Reserved                 040h-0FFh*/
  volatile u32 HPTXFSIZ;     /* Host Periodic Tx FIFO Size Reg     100h*/
  volatile u32 DIEPTXFx[NUM_TX_FIFOS - 1];        /* dev Transmit FIFO */

}
USB_OTG_GREGS;

/*******************************************************************************
 * dev Registers
 * dev Global Registers : Offsets 800h-BFFh
 * The following structures define the size and relative field offsets
 * for the dev Mode Registers.
 * These registers are visible only in dev mode and must not be
 * accessed in Host mode, as the results are unknown
 ******************************************************************************/
typedef struct _USB_OTG_DEV // 800h
{

  volatile u32 DCFG;           /* dev Configuration Register   800h*/
  volatile u32 DCTL;           /* dev Control Register         804h*/
  volatile u32 DSTS;           /* dev Status Register (RO)     808h*/
  u32 reservedC;           /* Reserved                     80Ch*/
  volatile u32 DIEPMSK;        /* dev IN Endpoint Mask         810h*/
  volatile u32 DOEPMSK;        /* dev OUT Endpoint Mask        814h*/
  volatile u32 DAINT;          /* dev All Endpoints Itr Reg    818h*/
  volatile u32 DAINTMSK;       /* dev All Endpoints Itr Mask   81Ch*/
  u32  Reserved20[2];      /* Reserved                820h-824h*/
  volatile u32 DVBUSDIS;       /* dev VBUS discharge Register  828h*/
  volatile u32 DVBUSPULSE;     /* dev VBUS Pulse Register      82Ch*/
  volatile u32 Reserved30;     /* Reserved                     830h*/
  volatile u32 DIEPEMPMSK;     /*  IN EP FIFO empty int mask   834h*/

}
USB_OTG_DEV;
/*******************************************************************************
 * dev Logical IN Endpoint-Specific Registers: Offsets 900h-AFCh
 * There will be one set of endpoint registers per logical endpointimplemented.
 * These registers are visible only in dev mode and must not be
 * accessed in Host mode, as the results are unknown
*******************************************************************************/
typedef struct _USB_OTG_DINEPS
{
  volatile u32 DIEPCTLx;     /* dev IN EP Ctl Reg 900h + (ep_num * 20h) + 00h*/
  u32 reserved04;        /* Reserved          900h + (ep_num * 20h) + 04h*/
  volatile u32 DIEPINTx;     /* dev IN EP Itr Reg 900h + (ep_num * 20h) + 08h*/
  u32 reserved0C;        /* Reserved          900h + (ep_num * 20h) + 0Ch*/
  volatile u32 DIEPTSIZx;    /* dev IN EP Tx Size 900h + (ep_num * 20h) + 10h*/
  u32 reserved14;        /* Reserved          900h + (ep_num * 20h) + 14h*/
  volatile u32 DTXFSTSx;     /* IN EP TxFIFO Sts  900h + (ep_num * 20h) + 18h*/
  u32 reserved18;        /* Reserved          900h + (ep_num * 20h) + 1Ch*/

}
USB_OTG_DINEPS;

/*******************************************************************************
 * dev Logical OUT Endpoint-Specific Registers Offsets: B00h-CFCh
 * There will be one set of endpoint registers per logical endpoint
 * implemented.
 * These registers are visible only in dev mode and must not be
 * accessed in Host mode, as the results are unknown
******************************************************************************/
typedef struct _USB_OTG_DOUTEPS
{
  volatile u32 DOEPCTLx;        /* OUT EP Ctl Reg B00h + (ep_num * 20h) + 00h*/
  u32 reserved04;           /* Reserved       B00h + (ep_num * 20h) + 04h*/
  volatile u32 DOEPINTx;        /* OUT EP Itr Reg B00h + (ep_num * 20h) + 08h*/
  u32 reserved0C;           /* Reserved       B00h + (ep_num * 20h) + 0Ch*/
  volatile u32 DOEPTSIZx;       /* OUT EP Tx Size B00h + (ep_num * 20h) + 10h*/
  volatile u32 Reserved14[3];   /* Reserved       B00h + (ep_num * 20h) + 14h*/
}
USB_OTG_DOUTEPS;



/*******************************************************************************
 * otg Core registers .
 * The USB_OTG_USB_OTG_FS_REGS structure defines the size
 * and relative field offsets for the Core Global registers.
 ******************************************************************************/
typedef struct USB_OTG_USB_OTG_FS_REGS  //000h
{
  USB_OTG_GREGS         *GREGS;
  USB_OTG_DEV           *DEV;
  USB_OTG_DINEPS        *DINEPS[NUM_TX_FIFOS];
  USB_OTG_DOUTEPS       *DOUTEPS[NUM_TX_FIFOS];
  volatile u32         *FIFO[NUM_TX_FIFOS];
  volatile u32         *PCGCCTL;

}
USB_OTG_CORE_REGS , *pUSB_OTG_CORE_REGS;
/******************************************************************************/
typedef union _USB_OTG_GAHBCFG_TypeDef
{
  u32 d32;
  struct
  {
u32 gintmsk :
    1;
u32 reserved1 :
    6;
u32 txfemplvl :
    1;
u32 reserved8_31 :
    24;
  }
  b;
} 
USB_OTG_GAHBCFG_TypeDef;
/******************************************************************************/

typedef union _USB_OTG_GUSBCFG_TypeDef
{
  u32 d32;
  struct
  {
u32 toutcal :
    3;
u32 Reserved3_5 :
    3;
u32 physel : /* MUST be always 1 because the phy is embedded*/
    1;
u32 Reserved7 :
    1;
u32 srpcap :
    1;
u32 hnpcap :
    1;
u32 usbtrdtim :
    4;
u32 reserved15_30 :
    15;
u32 force_host :
    1;
u32 force_dev :
    1;
u32 corrupt_tx :
    1;
  }
  b;
} USB_OTG_GUSBCFG_TypeDef;
/******************************************************************************/

typedef union _USB_OTG_GRSTCTL_TypeDef
{
  u32 d32;
  struct
  {
u32 csftrst :
    1;
u32 hsftrst :
    1;
u32 hstfrm :
    1;
u32 reserved3 :
    1;
u32 rxfflsh :
    1;
u32 txfflsh :
    1;
u32 txfnum :
    5;
u32 reserved11_30 :
    20;
u32 ahbidle :
    1;
  }
  b;
} USB_OTG_GRSTCTL_TypeDef;

/******************************************************************************/
typedef union _USB_OTG_GINTMSK_TypeDef
{
  u32 d32;
  struct
  {
u32 reserved0 :
    1;
u32 modemismatch :
    1;
u32 otgintr :
    1;
u32 sofintr :
    1;
u32 rxstsqlvl :
    1;
u32 reserved5 :
    1;
u32 ginnakeff :
    1;
u32 goutnakeff :
    1;
u32 reserved8_9 :
    2;
u32 erlysuspend :
    1;
u32 usbsuspend :
    1;
u32 usbreset :
    1;
u32 enumdone :
    1;
u32 isooutdrop :
    1;
u32 eopframe :
    1;
u32 reserved16 :
    1;
u32 epmismatch :
    1;
u32 inepintr :
    1;
u32 outepintr :
    1;
u32 incomplisoin :
    1;
u32 incomplisoout :
    1;
u32 reserved22_23 :
    2;
u32 portintr :
    1;
u32 hcintr :
    1;
u32 ptxfempty :
    1;
u32 reserved27 :
    1;
u32 conidstschng :
    1;
u32 disconnect :
    1;
u32 sessreqintr :
    1;
u32 wkupintr :
    1;
  }
  b;
} USB_OTG_GINTMSK_TypeDef;

/******************************************************************************/


typedef union _USB_OTG_GINTSTS_TypeDef
{
  u32 d32;
  struct
  {
u32 curmode :
    1;
u32 modemismatch :
    1;
u32 otgintr :
    1;
u32 sofintr :
    1;
u32 rxstsqlvl :
    1;
u32 reserved5 :
    1;
u32 ginnakeff :
    1;
u32 goutnakeff :
    1;
u32 reserved8_9 :
    2;
u32 erlysuspend :
    1;
u32 usbsuspend :
    1;
u32 usbreset :
    1;
u32 enumdone :
    1;
u32 isooutdrop :
    1;
u32 eopframe :
    1;
u32 Reserved16_17 :
    2;
u32 inepint:
    1;
u32 outepintr :
    1;
u32 incomplisoin :
    1;
u32 incomplisoout :
    1;
u32 reserved22_23 :
    2;
u32 portintr :
    1;
u32 hcintr :
    1;
u32 ptxfempty :
    1;
u32 reserved27 :
    1;
u32 conidstschng :
    1;
u32 disconnect :
    1;
u32 sessreqintr :
    1;
u32 wkupintr :
    1;
  }
  b;
} USB_OTG_GINTSTS_TypeDef;

/******************************************************************************/
typedef union _USB_OTG_GRXSTSP_TypeDef
{

  u32 d32;
  struct
  {
u32 epnum :
    4;
u32 bcnt :
    11;
u32 dpid :
    2;
u32 pktsts :
    4;
u32 frmnum :
    4;
u32 reserved :
    7;
  }
  b;
} USB_OTG_GRXSTSP_TypeDef;

/******************************************************************************/

typedef union _USB_OTG_FIFOSIZ_TypeDef
{
  u32 d32;
  struct
  {
u32 startaddr :
    16;
u32 depth :
    16;
  }
  b;
} USB_OTG_FIFOSIZ_TypeDef;

/******************************************************************************/

typedef union _USB_OTG_DTXFSTS_TypeDef
{
  u32 d32;
  struct
  {
u32 txfspcavail :
    16;
u32 reserved :
    16;
  }
  b;
} USB_OTG_DTXFSTS_TypeDef;

/******************************************************************************/

typedef union _USB_OTG_GCCFG_TypeDef
{

  u32 d32;
  struct
  {

u32 reserved0 :
    16;
u32 pwdn :
    1;
u32 reserved17 :
    1;
u32 vbussensingA :
    1;
u32 vbussensingB :
    1;
u32 SOFouten :
    1;
u32 reserved21 :
    11;
  }
  b;
} USB_OTG_GCCFG_TypeDef;


/******************************************************************************/
typedef union _USB_OTG_DCFG_TypeDef
{

  u32 d32;
  struct
  {
u32 devspd :
    2;
u32 nzstsouthshk :
    1;
u32 reserved3 :
    1;
u32 devaddr :
    7;
u32 perfrint :
    2;
u32 reserved13_31 :
    19;
  }
  b;
} USB_OTG_DCFG_TypeDef;

/******************************************************************************/

typedef union _USB_OTG_DCTL_TypeDef
{

  u32 d32;
  struct
  {
u32 rmtwkupsig :
    1;
u32 sftdiscon :
    1;
u32 gnpinnaksts :
    1;
u32 goutnaksts :
    1;
u32 tstctl :
    3;
u32 sgnpinnak :
    1;
u32 cgnpinnak :
    1;
u32 sgoutnak :
    1;
u32 cgoutnak :
    1;
u32 pwronprgdone :
    1;  
u32 reserved :
    20;
  }
  b;
} USB_OTG_DCTL_TypeDef;

/******************************************************************************/
typedef union _USB_OTG_DSTS_TypeDef
{

  u32 d32;
  struct
  {
u32 suspsts :
    1;
u32 enumspd :
    2;
u32 errticerr :
    1;
u32 reserved4_7:
    4;
u32 soffn :
    14;
u32 reserved22_31 :
    10;
  }
  b;
} USB_OTG_DSTS_TypeDef;

/******************************************************************************/

typedef union _USB_OTG_DIEPINTx_TypeDef
{

  u32 d32;
  struct
  {
u32 xfercompl :
    1;
u32 epdis :
    1;
u32 Reserved2 :
    1;
u32 timeout :
    1;
u32 intktxfemp :
    1;
u32 reserved5 :
    1;
u32 inepnakeff :
    1;
u32 txfempty :
    1;
u32 reserved08_31 :
    24;
  }
  b;
} USB_OTG_DIEPINTx_TypeDef;


typedef union _USB_OTG_DIEPINTx_TypeDef  USB_OTG_DIEPMSKx_TypeDef;
/******************************************************************************/

typedef union _USB_OTG_DOEPINTx_TypeDef
{

  u32 d32;
  struct
  {
u32 xfercompl :
    1;
u32 epdis :
    1;
u32 reserved2 :
    1;
u32 setup :   /* for EP0 only */
    1;
u32 outtokenepdis :
    1;
u32 reserved5 :
    1;   
u32 b2bsetup :
    1;      
u32 reserved07_31 :
    25;    
  }
  b;
} USB_OTG_DOEPINTx_TypeDef;


typedef union _USB_OTG_DOEPINTx_TypeDef  USB_OTG_DOEPMSKx_TypeDef;

/******************************************************************************/
typedef union _USB_OTG_DAINT_TypeDef
{

  u32 d32;
  struct
  {
u32 in :
    16;
u32 out :
    16;
  }
  ep;
} USB_OTG_DAINT_TypeDef;

/******************************************************************************/
typedef union _USB_OTG_DEPCTLx_TypeDef
{

  u32 d32;
  struct
  {
u32 mps :
    11;
u32 Reserved11_14 :
    4;
u32 usbactep :
    1;
u32 dpid :
    1;
u32 naksts :
    1;
u32 eptype :
    2;
u32 Reserved20 :
    1;
u32 stall :
    1;
u32 txfnum :
    4;
u32 cnak :
    1;
u32 snak :
    1;
u32 setd0pid :
    1;
u32 setoddfrm :
    1;
u32 epdis :
    1;
u32 epena :
    1;
  }
  b;
} USB_OTG_DEPCTLx_TypeDef;

/******************************************************************************/

typedef union _OTG_FS_DEPTSIZx_TypeDef
{

  u32 d32;
  struct
  {
u32 xfersize :
    19;
u32 pktcnt :
    10;
u32 mcount :
    2;
u32 reserved :
    1;
  }
  b;
} 
OTG_FS_DEPTSIZx_TypeDef;

/******************************************************************************/

typedef union _USB_OTG_DOEPTSIZ0_TypeDef
{

  u32 d32;
  struct
  {
u32 xfersize :
    7;
u32 reserved7_18 :
    12;
u32 pktcnt :
    1;
u32 reserved20_28 :
    9;
u32 supcnt :
    2;
    u32 reserved31;
  }
  b;
} USB_OTG_DOEPTSIZ0_TypeDef;

/******************************************************************************/

typedef union _OTG_FS_PCGCCTL_TypeDef
{
  u32 d32;
  struct
  {
u32 stoppclk :
    1;
u32 gatehclk :
    1;
u32 reserved3 :
    30;
  }
  b;
} OTG_FS_PCGCCTL_TypeDef;

#endif /* STM32F10X_CL */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

#endif
