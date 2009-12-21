/*----------------------------------------------------------------------------
 * Name:    STM32_Init.h
 * Purpose: STM32 peripherals initialisation definitions
 * Version: V1.00
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2005-2008 Keil Software. All rights reserved.
 *----------------------------------------------------------------------------
 * History:
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/

/* Define to prevent recursive inclusion ------------------------------------ */
#ifndef __STM32_INIT_H
#define __STM32_INIT_H

extern  void stm32_NvicSetup (void) ;
extern  void init_timer2(void);
extern  void init_portA(void);
extern  void Setup_Pll_As_Clock_System(void);
extern  void init_portB(void);
extern  void init_timer4(void);
extern  void init_timer1(void);
extern  void init_timer3(void);
extern  void  Setup_Adc(void);
extern  void Adc_On(void);
extern  u32  Get_Courant_Moteur(void);
extern  void Start_Conversion(void);
extern  void  Setup_Systick_Timer(void);
extern void init_portC(void);
#endif
