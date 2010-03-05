#ifndef __TIME_H__
#define __TIME_H__

#include <stm32f10x_lib.h>

typedef void (*TIME_CALLBACK) (void);

void TIMEInit (TIME_CALLBACK callback);
u32 TIMEGetCounter(void);
void TIMEResetCounter(void);

#endif /* __TIME_H__ */
