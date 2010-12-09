/*
 * Copyright (C) INSA Toulouse
 * Author: Sebastien DI MERCURIO
 *
 * This file is part of brushless DC motor controller.
 *
 * Brushless DC motor controller is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * Brushless DC motor controller is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public
 * License along with Brushless DC motor controller; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __MRF24J40_REGS_H__
#define __MRF24J40_REGS_H__ 

#define MAKE_SHORT_ADR(adr) (adr)
#define MAKE_LONG_ADR(adr)  (0x8000+adr)

#define MRF24J40_RXMCR 		MAKE_SHORT_ADR(0x00)
#define MRF24J40_PANIDL 	MAKE_SHORT_ADR(0x01)
#define MRF24J40_PANIDH 	MAKE_SHORT_ADR(0x02)
#define MRF24J40_SADRL	 	MAKE_SHORT_ADR(0x03)
#define MRF24J40_SADRH	 	MAKE_SHORT_ADR(0x04)
#define MRF24J40_EADR0	 	MAKE_SHORT_ADR(0x05)
#define MRF24J40_EADR1	 	MAKE_SHORT_ADR(0x06)
#define MRF24J40_EADR2	 	MAKE_SHORT_ADR(0x07)
#define MRF24J40_EADR3	 	MAKE_SHORT_ADR(0x08)
#define MRF24J40_EADR4	 	MAKE_SHORT_ADR(0x09)
#define MRF24J40_EADR5	 	MAKE_SHORT_ADR(0x0A)
#define MRF24J40_EADR6	 	MAKE_SHORT_ADR(0x0B)
#define MRF24J40_EADR7	 	MAKE_SHORT_ADR(0x0C)
#define MRF24J40_RXFLUSH 	MAKE_SHORT_ADR(0x0D)

#define MRF24J40_ORDER 		MAKE_SHORT_ADR(0x10)
#define MRF24J40_TXMCR	 	MAKE_SHORT_ADR(0x11)
#define MRF24J40_ACKTMOUT 	MAKE_SHORT_ADR(0x12)
#define MRF24J40_ESLOTG1	MAKE_SHORT_ADR(0x13)
#define MRF24J40_SYMTICKL	MAKE_SHORT_ADR(0x14)
#define MRF24J40_SYMTICKH	MAKE_SHORT_ADR(0x15)
#define MRF24J40_PACON0	 	MAKE_SHORT_ADR(0x16)
#define MRF24J40_PACON1	 	MAKE_SHORT_ADR(0x17)
#define MRF24J40_PACON2	 	MAKE_SHORT_ADR(0x18)
#define MRF24J40_TXBCON0	MAKE_SHORT_ADR(0x1A)
#define MRF24J40_TXNCON	 	MAKE_SHORT_ADR(0x1B)
#define MRF24J40_TXG1CON	MAKE_SHORT_ADR(0x1C)
#define MRF24J40_TXG2CON	MAKE_SHORT_ADR(0x1D)
#define MRF24J40_ESLOTG23 	MAKE_SHORT_ADR(0x1E)
#define MRF24J40_ESLOTG45 	MAKE_SHORT_ADR(0x1F)

#define MRF24J40_ESLOTG67 	MAKE_SHORT_ADR(0x20)
#define MRF24J40_TXPEND	 	MAKE_SHORT_ADR(0x21)
#define MRF24J40_WAKECON 	MAKE_SHORT_ADR(0x22)
#define MRF24J40_FRMOFFSET	MAKE_SHORT_ADR(0x23)
#define MRF24J40_TXSTAT		MAKE_SHORT_ADR(0x24)
#define MRF24J40_TXBCON1	MAKE_SHORT_ADR(0x25)
#define MRF24J40_GATECLK	MAKE_SHORT_ADR(0x26)
#define MRF24J40_TXTIME	 	MAKE_SHORT_ADR(0x27)
#define MRF24J40_HSYMTMRL	MAKE_SHORT_ADR(0x28)
#define MRF24J40_HSYMTMRH	MAKE_SHORT_ADR(0x29)
#define MRF24J40_SOFTRST	MAKE_SHORT_ADR(0x2A)
#define MRF24J40_SECCON0	MAKE_SHORT_ADR(0x2C)
#define MRF24J40_SECCON1	MAKE_SHORT_ADR(0x2D)
#define MRF24J40_TXSTBL 	MAKE_SHORT_ADR(0x2E)

#define MRF24J40_RXSR 		MAKE_SHORT_ADR(0x30)
#define MRF24J40_INTSTAT	MAKE_SHORT_ADR(0x31)
#define MRF24J40_INTCON 	MAKE_SHORT_ADR(0x32)
#define MRF24J40_GPIO		MAKE_SHORT_ADR(0x33)
#define MRF24J40_TRISGPIO	MAKE_SHORT_ADR(0x34)
#define MRF24J40_SLPACK		MAKE_SHORT_ADR(0x35)
#define MRF24J40_RFCTL		MAKE_SHORT_ADR(0x36)
#define MRF24J40_SECCR2	 	MAKE_SHORT_ADR(0x37)
#define MRF24J40_BBREG0		MAKE_SHORT_ADR(0x38)
#define MRF24J40_BBREG1		MAKE_SHORT_ADR(0x39)
#define MRF24J40_BBREG2		MAKE_SHORT_ADR(0x3A)
#define MRF24J40_BBREG3		MAKE_SHORT_ADR(0x3B)
#define MRF24J40_BBREG4		MAKE_SHORT_ADR(0x3C)
#define MRF24J40_BBREG6 	MAKE_SHORT_ADR(0x3E)
#define MRF24J40_CCAEDTH 	MAKE_SHORT_ADR(0x3F)

/* */
#define MRF24J40_RFCON0 	MAKE_LONG_ADR(0x200)
#define MRF24J40_RFCON1 	MAKE_LONG_ADR(0x201)
#define MRF24J40_RFCON2 	MAKE_LONG_ADR(0x202)
#define MRF24J40_RFCON3 	MAKE_LONG_ADR(0x203)
#define MRF24J40_RFCON5 	MAKE_LONG_ADR(0x205)
#define MRF24J40_RFCON6 	MAKE_LONG_ADR(0x206)
#define MRF24J40_RFCON7 	MAKE_LONG_ADR(0x207)
#define MRF24J40_RFCON8 	MAKE_LONG_ADR(0x208)
#define MRF24J40_SLPCAL0 	MAKE_LONG_ADR(0x209)
#define MRF24J40_SLPCAL1 	MAKE_LONG_ADR(0x20A)
#define MRF24J40_SLPCAL2 	MAKE_LONG_ADR(0x20B)
#define MRF24J40_RFSTATE 	MAKE_LONG_ADR(0x20F)

#define MRF24J40_RSSI	 	MAKE_LONG_ADR(0x210)
#define MRF24J40_SLPCON0 	MAKE_LONG_ADR(0x211)

#define MRF24J40_SLPCON1 	MAKE_LONG_ADR(0x220)
#define MRF24J40_WAKETIMEL 	MAKE_LONG_ADR(0x222)
#define MRF24J40_WAKETIMEH 	MAKE_LONG_ADR(0x223)
#define MRF24J40_REMCNTL 	MAKE_LONG_ADR(0x224)
#define MRF24J40_REMCNTH 	MAKE_LONG_ADR(0x225)
#define MRF24J40_MAINCNT0 	MAKE_LONG_ADR(0x226)
#define MRF24J40_MAINCNT1 	MAKE_LONG_ADR(0x227)
#define MRF24J40_MAINCNT2 	MAKE_LONG_ADR(0x228)
#define MRF24J40_MAINCNT3 	MAKE_LONG_ADR(0x229)
#define MRF24J40_TESTMODE 	MAKE_LONG_ADR(0x22F)

#define MRF24J40_ASSOEADR0 	MAKE_LONG_ADR(0x230)
#define MRF24J40_ASSOEADR1 	MAKE_LONG_ADR(0x231)
#define MRF24J40_ASSOEADR2	MAKE_LONG_ADR(0x232)
#define MRF24J40_ASSOEADR3 	MAKE_LONG_ADR(0x233)
#define MRF24J40_ASSOEADR4 	MAKE_LONG_ADR(0x234)
#define MRF24J40_ASSOEADR5 	MAKE_LONG_ADR(0x235)
#define MRF24J40_ASSOEADR6 	MAKE_LONG_ADR(0x236)
#define MRF24J40_ASSOEADR7 	MAKE_LONG_ADR(0x237)
#define MRF24J40_ASSOSADR0 	MAKE_LONG_ADR(0x238)
#define MRF24J40_ASSOSADR1 	MAKE_LONG_ADR(0x239)

#define MRF24J40_UPNONCE0 	MAKE_LONG_ADR(0x240)
#define MRF24J40_UPNONCE1 	MAKE_LONG_ADR(0x241)
#define MRF24J40_UPNONCE2 	MAKE_LONG_ADR(0x242)
#define MRF24J40_UPNONCE3 	MAKE_LONG_ADR(0x243)
#define MRF24J40_UPNONCE4 	MAKE_LONG_ADR(0x244)
#define MRF24J40_UPNONCE5 	MAKE_LONG_ADR(0x245)
#define MRF24J40_UPNONCE6 	MAKE_LONG_ADR(0x246)
#define MRF24J40_UPNONCE7 	MAKE_LONG_ADR(0x247)
#define MRF24J40_UPNONCE8 	MAKE_LONG_ADR(0x248)
#define MRF24J40_UPNONCE9 	MAKE_LONG_ADR(0x249)
#define MRF24J40_UPNONCE10 	MAKE_LONG_ADR(0x24A)
#define MRF24J40_UPNONCE11 	MAKE_LONG_ADR(0x24B)
#define MRF24J40_UPNONCE12 	MAKE_LONG_ADR(0x24C)

#endif /* __MRF24J40_REGS_H__ */