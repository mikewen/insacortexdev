#include <stm32f10x_lib.h>
#include <stdio.h>
#include "missing_defs.h"

#include "uart.h"
#include <stdarg.h>
#include "time.h"

int Delay_Uart;

FILE UART_1;
FILE UART_2;
FILE UART_3;

FILE UART_1_NB;
FILE UART_2_NB;
FILE UART_3_NB;

void uart_init(void)
{
	UART_1._file=UART_1_ID;
	UART_2._file=UART_2_ID;
	UART_3._file=UART_3_ID;

	UART_1_NB._file=UART_1_ID_NB;
	UART_2_NB._file=UART_2_ID_NB;
	UART_3_NB._file=UART_3_ID_NB;
}

void putchar_UART1(u8 c)
{
	while (!(USART1->SR & USART_FLAG_TXE));

	USART1->DR = c;
}

u8 getchar_UART1 (void)
{	 
	while (!(USART1->SR & USART_FLAG_RXNE));

	return 	(u8) USART1->DR;
}

void putchar_UART2(u8 c)
{
	while (!(USART2->SR & USART_FLAG_TXE));

	USART2->DR = c;
}

u8 getchar_UART2 (void)
{	
	TIMEResetCounter();

	while (
			!(USART2->SR & USART_FLAG_RXNE) &&
			(TIMEGetCounter()<Delay_Uart)
		  )
	{
	}

   	if (TIMEGetCounter()>=Delay_Uart)
	{
		/* Probleme de liaison serie => Reset */
		USART2->CR1  = USART2->CR1 & ~(USART_Mode_Rx);
		USART2->CR1  = USART2->CR1 | USART_Mode_Rx;
	}

	return 	(u8) USART2->DR;
}

void putchar_UART3(u8 c)
{
	while (!(USART3->SR & USART_FLAG_TC));

	USART3->DR = c;

	while (!(USART3->SR & USART_FLAG_TC));
}


u8 getchar_UART3 (void)
{	 
	while (!(USART3->SR & USART_FLAG_RXNE));

	return 	(u8) USART3->DR;
}

signed char getchar_UART1_NB (void)
{
signed char data;
	 
	if ((USART1->SR & USART_FLAG_RXNE)!=0)
	{
		data = (u8) USART1->DR;
	}
	else data = -1;

	return data;
}

signed char getchar_UART2_NB (void)
{
signed char data;
	 
	if ((USART2->SR & USART_FLAG_RXNE)!=0)
	{
		data = (u8) USART2->DR;
	}
	else data = -1;

	return data;
}

signed char getchar_UART3_NB (void)
{
signed char data;
	 
	if ((USART3->SR & USART_FLAG_RXNE)!=0)
	{
		data = (u8) USART3->DR;
	}
	else data = -1;

	return data;
}


