#ifndef _USB_CDC_H_
#define _USB_CDC_H_

#include "hw_config.h"

/* Api de la classe CDC */
void USB_CDC_Init(void);
int USB_CDC_Status(void);
int USB_CDC_SendData(u8 *buffer, int len);
int USB_CDC_ReadData(u8 *buffer, int len);
int USB_CDC_RXBufferLength(void);

/* Autres fonctions ------------------------------------------------------- */
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);
bool USART_Config(void);
void Handle_USBAsynchXfer (void);
void Get_SerialNum(void);

#endif /* _USB_CDC_H_ */