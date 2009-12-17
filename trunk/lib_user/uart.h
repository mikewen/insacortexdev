#ifndef __UART_H__
#define __UART_H__

//#include <stm32f10x_lib.h>
//#include "../../machines/cortex-m3/tpl_os_std_types.h"
#define bool u8

#define UART_1 1
#define UART_2 2
#define UART_3 3
#define XBEE UART_3

void putchar(u8 channel, u8 c);
u8 getchar (u8 channel, int delay);
bool getchar_NB(u8 channel, u8 *data);
int	printf(u8 channel, const char *fmt, ...);

#endif /* __UART_H__ */
