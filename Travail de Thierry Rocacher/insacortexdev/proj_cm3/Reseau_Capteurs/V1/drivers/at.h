#ifndef __AT_H__
#define __AT_H__

#include <stm32f10x_lib.h>

typedef enum {AT_SUCCESS=1, AT_ERROR=0} AT_STATUS;

void ATInit(u8 channel);
void ATEnterMode(void);
void ATExitMode(void);

AT_STATUS ATSetChannel(u8 channel);
AT_STATUS ATSetPanID(u16 panId);
AT_STATUS ATSetDestinationLow(u32 destLow);
AT_STATUS ATSetDestinationHigh(u32 destHigh);
AT_STATUS ATSetShortSourceAddr(u16 addr);

#endif /* __AT_H__ */				
