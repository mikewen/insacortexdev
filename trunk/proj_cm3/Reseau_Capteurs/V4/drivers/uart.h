#ifndef __UART_H__
#define __UART_H__

#include <stm32f10x_lib.h>

#include <stdio.h>
#include "missing_defs.h"

extern FILE UART_1;
extern FILE UART_2;
extern FILE UART_3;

#define UART_1_ID 1
#define UART_2_ID 2
#define UART_3_ID 3 


#define EMPTY 0
#define NOT_EMPTY 1
#define TIMEOUT 2

void uart_init(void);

int SendChar (int uart, int c);
int GetKey (int uart);
u8 UART_Buffer_State (FILE *f);
#endif /* __UART_H__ */
