#include <stm32f10x_lib.h>
#include <stdio.h>
#include "missing_defs.h"

#include "uart.h"
#include <stdarg.h>
#include "time.h"

#include "stm_system.h"

int Delay_Uart;

FILE UART_1;
FILE UART_2;
FILE UART_3;

FILE UART_1_NB;
FILE UART_2_NB;
FILE UART_3_NB;

#define BUF_SIZE 512

struct buf_st 
{
  	unsigned int in;                                // Next In Index
  	unsigned int out;                               // Next Out Index
	unsigned int len;
  	char buf [BUF_SIZE];                           // Buffer
};

/* Variables pour l'uart1 */
struct buf_st rbuf_uart1 = { 0, 0, 0, };
struct buf_st tbuf_uart1 = { 0, 0, 0, };
int tx_restart_uart1 = 1;               // NZ if TX restart is required

/* Variables pour l'uart2 */
struct buf_st rbuf_uart2 = { 0, 0, 0, };
struct buf_st tbuf_uart2 = { 0, 0, 0, };
int tx_restart_uart2 = 1;               // NZ if TX restart is required

/* Variables pour l'uart3 */
struct buf_st rbuf_uart3 = { 0, 0, 0, };
struct buf_st tbuf_uart3 = { 0, 0, 0, };
int tx_restart_uart3 = 1;               // NZ if TX restart is required

void uart_init(void)
{
int i;

	UART_1._file=UART_1_ID;
	UART_2._file=UART_2_ID;
	UART_3._file=UART_3_ID;

	UART_1_NB._file=UART_1_ID_NB;
	UART_2_NB._file=UART_2_ID_NB;
	UART_3_NB._file=UART_3_ID_NB;

	for (i=0; i<BUF_SIZE; i++)
	{
		rbuf_uart1.buf[i]=0;
		tbuf_uart1.buf[i]=0;
		rbuf_uart2.buf[i]=0;
		tbuf_uart2.buf[i]=0;
		rbuf_uart3.buf[i]=0;
		tbuf_uart3.buf[i]=0;
	}

	rbuf_uart1.in = 0;
	rbuf_uart1.out = 0;
	rbuf_uart1.len = 0;
	rbuf_uart2.in = 0;
	rbuf_uart2.out = 0;
	rbuf_uart2.len = 0;
	rbuf_uart3.in = 0;
	rbuf_uart3.out = 0;
	rbuf_uart3.len = 0;

	NVIC_SET_PRIO_PERIPH(USART1,10);
	NVIC_SET_PRIO_PERIPH(USART2,10);
	NVIC_SET_PRIO_PERIPH(USART3,10);
	NVIC_SET_PRIO_SYSTEM(SYSTICK,14);
}

/*----------------------------------------------------------------------------
  USART1_IRQHandler
  Handles USART1 global interrupt request.
 *----------------------------------------------------------------------------*/
void USART1_IRQHandler (void) 
{
volatile unsigned int IIR;
struct buf_st *p;

    IIR = USART1->SR;
    if (IIR & USART_FLAG_RXNE)                   // read interrupt
	{
    	USART1->SR &= ~USART_FLAG_RXNE;	          // clear interrupt
      	p = &rbuf_uart1;

      	if (p->len <BUF_SIZE) 
		{
        	p->buf [p->in] = (unsigned char)(USART1->DR & 0x0FF);
        	p->in++;
			p->len++;
			if (p->in> (BUF_SIZE-1)) p->in = 0;
      	}
	}

    if (IIR & USART_FLAG_TXE) 
	{
      	USART1->SR &= ~USART_FLAG_TXE;	          // clear interrupt
		p = &tbuf_uart1;

      	if (p->len !=0) 
		{
        	USART1->DR = (unsigned char)(p->buf [p->out] & 0x0FF);
        	p->out++;
			p->len--;
			if (p->out> (BUF_SIZE-1)) p->out = 0;
        	tx_restart_uart1 = 0;
      	}
      	else 
		{
        	tx_restart_uart1 = 1;
			USART1->CR1 &= ~USART_FLAG_TXE;		      // disable TX interrupt if nothing to send
      	}
    }
}

/*----------------------------------------------------------------------------
  USART2_IRQHandler
  Handles USART2 global interrupt request.
 *----------------------------------------------------------------------------*/
void USART2_IRQHandler (void) 
{
volatile unsigned int IIR;
struct buf_st *p;

    IIR = USART2->SR;
    if (IIR & USART_FLAG_RXNE)                   // read interrupt
	{
    	USART2->SR &= ~USART_FLAG_RXNE;	          // clear interrupt
      	p = &rbuf_uart2;

      	if (p->len <BUF_SIZE) 
		{
        	p->buf [p->in] = (unsigned char)(USART2->DR & 0x0FF);
        	p->in++;
			p->len++;
			if (p->in> (BUF_SIZE-1)) p->in = 0;
      	}
	}

    if (IIR & USART_FLAG_TXE) 
	{
      	USART2->SR &= ~USART_FLAG_TXE;	          // clear interrupt
		p = &tbuf_uart2;

      	if (p->len !=0) 
		{
        	USART2->DR = (unsigned char)(p->buf [p->out] & 0x0FF);
        	p->out++;
			p->len--;
			if (p->out> (BUF_SIZE-1)) p->out = 0;
        	tx_restart_uart2 = 0;
      	}
      	else 
		{
        	tx_restart_uart2 = 1;
			USART2->CR1 &= ~USART_FLAG_TXE;		      // disable TX interrupt if nothing to send
      	}
    }
}

/*----------------------------------------------------------------------------
  USART3_IRQHandler
  Handles USART3 global interrupt request.
 *----------------------------------------------------------------------------*/
void USART3_IRQHandler (void) 
{
volatile unsigned int IIR;
struct buf_st *p;

    IIR = USART3->SR;
    if (IIR & USART_FLAG_RXNE)                   // read interrupt
	{
    	USART3->SR &= ~USART_FLAG_RXNE;	          // clear interrupt
      	p = &rbuf_uart3;

      	if (p->len <BUF_SIZE) 
		{
        	p->buf [p->in] = (unsigned char)(USART3->DR & 0x0FF);
        	p->in++;
			p->len++;
			if (p->in> (BUF_SIZE-1)) p->in = 0;
      	}
	}

    if (IIR & USART_FLAG_TXE) 
	{
      	USART3->SR &= ~USART_FLAG_TXE;	          // clear interrupt
		p = &tbuf_uart3;

      	if (p->len !=0) 
		{
        	USART3->DR = (unsigned char)(p->buf [p->out] & 0x0FF);
        	p->out++;
			p->len--;
			if (p->out> (BUF_SIZE-1)) p->out = 0;
        	tx_restart_uart3 = 0;
      	}
      	else 
		{
        	tx_restart_uart3 = 1;
			USART3->CR1 &= ~USART_FLAG_TXE;		      // disable TX interrupt if nothing to send
      	}
    }
}

/*------------------------------------------------------------------------------
  SendChar
  transmit a character
 *------------------------------------------------------------------------------*/
int SendChar (int uart, int c) 
{
struct buf_st *p;

	switch (uart)
	{
		case UART_1_ID:
		case UART_1_ID_NB:
			p = &tbuf_uart1;
			break;
		case UART_2_ID:
		case UART_2_ID_NB:
			p = &tbuf_uart2;
			break;
		case UART_3_ID:
		case UART_3_ID_NB:
			p = &tbuf_uart3;
			break;
	}
                                                  // If the buffer is full, return an error value
  	if (p->len >= BUF_SIZE) return (-1);
                                                  
  	p->buf [p->in ] = c;           // Add data to the transmit buffer.
  	p->in++;
	p->len++;
	if (p->in >(BUF_SIZE-1)) p->in=0;

	switch (uart)
	{
		case UART_1_ID:
		case UART_1_ID_NB:
			if (tx_restart_uart1)
			{
				tx_restart_uart1 = 0; 
				USART1->CR1 |= USART_FLAG_TXE;		          // enable TX interrupt
			}
			break;
		case UART_2_ID:
		case UART_2_ID_NB: 
			if (tx_restart_uart2)
			{
				tx_restart_uart2 = 0; 
				USART2->CR1 |= USART_FLAG_TXE;		          // enable TX interrupt
			}
			break;
		case UART_3_ID:
		case UART_3_ID_NB: 
			if (tx_restart_uart3)
			{
				tx_restart_uart3 = 0; 
				USART3->CR1 |= USART_FLAG_TXE;		          // enable TX interrupt
			}
			break;
	}

  	return (0);
}

/*------------------------------------------------------------------------------
  GetKey
  receive a character
 *------------------------------------------------------------------------------*/
int GetKey (int uart) 
{
struct buf_st *p;
char data;

	switch (uart)
	{
		case UART_1_ID:
		case UART_1_ID_NB:
			p = &rbuf_uart1;
			break;
		case UART_2_ID:
		case UART_2_ID_NB:
			p = &rbuf_uart2;
			break;
		case UART_3_ID:
		case UART_3_ID_NB:
			p = &rbuf_uart3;
			break;
	}

	if (p->len == 0) return (-1);
	else p->len--;

	data = p->buf [p->out];
	p->out++;
	if (p->out > (BUF_SIZE-1)) p->out=0;

  	return (data);
}

void putchar_UART1(u8 c)
{
	SendChar(1, c);
}

u8 getchar_UART1 (void)
{	 
int ch;

	do 
	{
    	ch = GetKey (1);
  	} while (ch == -1);
  	
  	return ((u8)ch);
}

void putchar_UART2(u8 c)
{
	SendChar(2, c);
}

u8 getchar_UART2 (void)
{	
int ch;

	do 
	{
    	ch = GetKey (2);
  	} while (ch == -1);
  	
  	return ((u8)ch);
}

void putchar_UART3(u8 c)
{
	SendChar(3, c);
}

u8 getchar_UART3 (void)
{	 
int ch;

	do 
	{
    	ch = GetKey (3);
  	} while (ch == -1);
  	
  	return ((u8)ch);
}

signed char getchar_UART1_NB (void)
{
int ch;

	ch = GetKey (1);
  	
  	return ((signed char)ch);
}

signed char getchar_UART2_NB (void)
{
int ch;

	ch = GetKey (2);
  	
  	return ((signed char)ch);
}

signed char getchar_UART3_NB (void)
{
int ch;

	ch = GetKey (3);
  	
  	return ((signed char)ch);
}

u8 UART_Buffer_State (FILE *f)
{
u8 status;
	
	status = EMPTY;
	
	switch (f->_file)
	{
		case UART_1_ID:
		case UART_1_ID_NB:
			if (rbuf_uart1.len !=0) status =NOT_EMPTY;
			break;
		case UART_2_ID:
		case UART_2_ID_NB:
			if (rbuf_uart2.len !=0) status =NOT_EMPTY;
			break;
		case UART_3_ID:
		case UART_3_ID_NB:
			if (rbuf_uart3.len !=0) status =NOT_EMPTY;
			break;
	} 

	return status;
}
