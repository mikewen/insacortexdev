#ifndef __ADC_H__
#define __ADC_H__

#include <stm32f10x_lib.h>

void ADCInit (void);
u16 ADCConvert (u8 channel);

#endif /* __ADC_H__ */
