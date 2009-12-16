#include <stm32f10x_lib.h>
#include "uart.h"
#include <stdarg.h>
#include "time.h"

int Delay_Uart;

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
	//TIMEResetCounter();

	/*while (
			!(USART2->SR & USART_FLAG_RXNE) &&
			(TIMEGetCounter()<Delay_Uart)
		  )	  */
	while (
			!(USART2->SR & USART_FLAG_RXNE)
		  )
	{
	}

   	/*if (TIMEGetCounter()>=Delay_Uart)
	{*/
		// Probleme de liaison serie => Reset */
	/*	USART2->CR1  = USART2->CR1 & ~(USART_Mode_Rx);
		USART2->CR1  = USART2->CR1 | USART_Mode_Rx;
	} */

	return 	(u8) USART2->DR;
}

void putchar_UART3(u8 c)
{
	while (!(USART1->SR & USART_FLAG_TXE));

	USART3->DR = c;
}


u8 getchar_UART3 (void)
{	 
	while (!(USART3->SR & USART_FLAG_RXNE));

	return 	(u8) USART3->DR;
}

void putchar(u8 channel, u8 c)
{
	switch (channel)
	{
	case UART_1:
		putchar_UART1(c);
		break;
	case UART_2:
		putchar_UART2(c);
		break;
	case UART_3:
		putchar_UART3(c);
		break;
	}
}


u8 getchar (u8 channel, int delay)
{
u8 temp;

	Delay_Uart = delay;
	temp =0;

	switch (channel)
	{
	case UART_1:
		temp= getchar_UART1();
		break;
	case UART_2:
		temp= getchar_UART2();
		break;
	case UART_3:
		temp= getchar_UART3();
		break;
	}

	return temp;
}

bool getchar_NB(u8 channel, u8 *data)
{
bool status;

	status =  FALSE;

	switch (channel)
	{
	case UART_1:
		if ((USART1->SR & USART_FLAG_RXNE)!=0)
		{
			status = TRUE;
			*data = (u8) USART1->DR;
		}
		break;
	case UART_2:
		if ((USART2->SR & USART_FLAG_RXNE)!=0)
		{
			status = TRUE;
			*data = (u8) USART2->DR;
		}
		break;
	case UART_3:
		if ((USART3->SR & USART_FLAG_RXNE)!=0)
		{
			status = TRUE;
			*data = (u8) USART3->DR;
		}
		break;
	}

	return status;	
}

void printint(u8 channel, u16 i)
{
u8 digit;
	
	/*digit = i % 10;
	digit = digit + '0';
	i = i / 10;
	putchar(channel, (u8)i);*/

	digit = i/10000;
	putchar(channel, (u8)(digit+ '0'));

	i=i - (digit*10000);

	digit = i/1000;
	putchar(channel, (u8)(digit+ '0'));

	i=i - (digit*1000);

	digit = i/100;
	putchar(channel, (u8)(digit+ '0'));

	i=i - (digit*100);

	digit = i/10;
	putchar(channel, (u8)(digit+ '0'));

	i=i - (digit*10);

	digit = i;
	putchar(channel, (u8)(digit+ '0'));
}

void printstring(u8 channel, char *s)
{
int i;

	i=0;
	while (s[i]!= 0x0)
	{
		putchar(channel, (u8)s[i]);
		i++;
	}
}

void printhex(u8 channel, u16 val)
{
	u16 i;

	i = val;

	i = ((val & 0xF000)>>12);
	if (i<10)
	{
		if (i!=0) putchar(channel, i+'0');
	}
	else
	{
		putchar(channel, i+'A'-10);
	}

	i = ((val & 0x0F00)>>8);
	if (i<10)
	{
		if (i!=0) putchar(channel, i+'0');
	}
	else
	{
		putchar(channel, i+'A'-10);
	}

	i = ((val & 0x00F0)>>4);
	if (i<10)
	{
		putchar(channel, i+'0');
	}
	else
	{
		putchar(channel, i+'A'-10);
	}

	i = (val & 0x000F);
	if (i<10)
	{
		putchar(channel, i+'0');
	}
	else
	{
		putchar(channel, i+'A'-10);
	}
}

int	printf(u8 channel, const char *fmt, ...)
{
char *p;
va_list argp;
int i;
char *s;

	va_start(argp, fmt);

	for(p = (char*)fmt; *p != '\0'; p++)
	{
		if(*p != '%')
		{
			putchar(channel, (u8)*p);
			continue;
		}

		switch(*++p)
		{
		case 'c':
			i = va_arg(argp, int);
			putchar(channel, (u8)i);
			break;

		case 'd':
			i = va_arg(argp, int);
			printint(channel, i);
			break;

		case 's':
			s = va_arg(argp, char *);
			printstring(channel, s);
			break;

		case 'x':
			i = va_arg(argp, int);
			printhex(channel, i);
		break;

		case '%':
			putchar(channel, '%');
			break;
		}
	}

	va_end(argp);

	return 0;
}


