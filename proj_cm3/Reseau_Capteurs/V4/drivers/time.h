#ifndef __TIME_H__
#define __TIME_H__

#include <stm32f10x_lib.h>

typedef void (*TIME_CALLBACK) (void);

extern int TIMEEnabled;

void TIMEInit (TIME_CALLBACK callback);
u32 TIMEGetCounter(void);
void TIMEResetCounter(void);
void TIMEWait1ms(void);
void TIMEWaitxms(int ms);

#endif /* __TIME_H__ */
