/******************************************************************************
* File Name          : adc.h
* Author             : P Acco/ S DI Mercurio
* Version            : V1.0.0
* Date               : 01/09/2010
* Description        : Header file for adc driver
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

#ifndef __ADC_H__
#define __ADC_H__

#define ADC_ARRAY_SIZE 100
extern short int ADC_Array[ADC_ARRAY_SIZE];

void Init_ADC (void);
int Lire_ADC(void);

#endif /*__ADC_H__ */