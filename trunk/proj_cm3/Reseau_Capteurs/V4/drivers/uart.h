#ifndef __UART_H__
#define __UART_H__

#include <stm32f10x_lib.h>

#include <stdio.h>
#include "missing_defs.h"

extern FILE UART_1;
extern FILE UART_2;
extern FILE UART_3;
extern FILE UART_1_NB;
extern FILE UART_2_NB;
extern FILE UART_3_NB;

#define UART_1_ID 1
#define UART_2_ID 2
#define UART_3_ID 3 

#define UART_1_ID_NB 101
#define UART_2_ID_NB 102
#define UART_3_ID_NB 103 

#define EMPTY 0
#define NOT_EMPTY 1
#define TIMEOUT 2

void uart_init(void);

void putchar_UART1(u8 c);
u8 getchar_UART1 (void);
void putchar_UART2(u8 c);
u8 getchar_UART2 (void);
void putchar_UART3(u8 c);
u8 getchar_UART3 (void);
signed char getchar_UART1_NB (void);
signed char getchar_UART2_NB (void);
signed char getchar_UART3_NB (void);

u8 UART_Buffer_State (FILE *f);
#endif /* __UART_H__ */
