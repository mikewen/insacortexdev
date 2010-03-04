#ifndef __UART_H__
#define __UART_H__

#include <stm32f10x_lib.h>

#define UART_1 1
#define UART_2 2
#define UART_3 3



void putchar(u8 channel, u8 c);
u8 getchar (u8 channel, int delay);
bool getchar_NB(u8 channel, u8 *data);
int	printf(u8 channel, const char *fmt, ...);

#endif /* __UART_H__ */
