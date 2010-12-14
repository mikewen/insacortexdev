#ifndef _USB_CDC_H_
#define _USB_CDC_H_

/* Api de la classe CDC */
void USB_CDC_Init(void);
int USB_CDC_Status(void);
int USB_CDC_SendData(u8 *buffer, int len);
int USB_CDC_ReadData(u8 *buffer, int len);
int USB_CDC_RXBufferLength(void);

#endif /* _USB_CDC_H_ */