/******************************************************************************
* File Name          : adc.h
* Author             : P Acco/ S DI Mercurio
* Version            : V1.0.0
* Date               : 01/09/2010
* Description        : Header file for pwm driver
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifndef __PWM_H__
#define __PWM_H__

typedef void(*eventptr)(void);

void Init_PWM (void);
void Regle_PWM(int val);

#endif /*__PWM_H__ */