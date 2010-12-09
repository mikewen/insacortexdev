#ifndef __RS606_H__
#define __RS606_H__

#include <stm32f10x_lib.h>

typedef enum {RS606_OFF, RS606_TX, RS606_RX, RS606_TX_LOOPBACK} RS606_MODE;

void RS606SetMode(RS606_MODE mode);

#endif /* __RS606_H__ */				
