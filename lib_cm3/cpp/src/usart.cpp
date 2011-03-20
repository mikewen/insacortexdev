/*
________________________________________________________________________________________
	UART driver files for stm32f103
	Configuration is located in file stm_uart_config.h
    GPL licensed code (S. Di Mercurio and P. Acco)
________________________________________________________________________________________
USAGE
	??? is the name of the library i.e. stm_uart
	RELPATH is the relative path from your projetc directory to lib_cm3 directory	
	
   	include RELPATH/lib_cm3/???.c file in your makefile
	copy    RELPATH/lib_cm3/???_config_TEMPLATE.h in your project directory	(noted ./)
	rename  ./???_config_TEMPLATE.h as ./???_config.h in your project directory
	edit    ./???_config.h file for your project (lines with //CONF tags)  
	ensure that ./ path is visible in CC compiler options 
	add "#include "RELPATH/lib_cm3/???.h" in app. code 
	add a call to "Init_???();" at initialisation step of your APP
________________________________________________________________________________________
REVS
			
________________________________________________________________________________________
TODO

________________________________________________________________________________________
*/

#include "stm_regs.h"
#include "usart.h"     // import configuration NUM_USART and BAUDRATEx
#include "clock.h"

namespace System
{
namespace Driver
{

#define __USART_SETUP             1              
#define __USART_BAUDRATE         BAUDRATEx		
#define __USART_DATABITS         0x00000000
#define __USART_STOPBITS         0x00000000
#define __USART_PARITY           0x00000000
#define __USART_FLOWCTRL         0x00000000
#define __USART_REMAP            0x00000000

// Je récupère les bouts de STM32_Init utiles à l'USART

// USART conf modifiée pour être configurée par lib_usartx_pol.h
#if ( USART_NUM == 1 )
	#define __USART1
	#define USARTx 				USART1
	#define TX_DMA_CHANNEL 		DMA1_Channel4 
	#define TX_DMA_HANDLER 		DMAChannel4_IRQHandler
	#define DMA1_CHANNELx_IRQ 	DMA1_CHANNEL4_BIT_SHIFT
	#define USARTx_IRQChannel 	USART1_BIT_SHIFT
	#define USARTx_IRQHandler 	USART1_IRQHandler
	#define TX_DMA_NUM 			4
	#define DMAx_ISR_MASK 		(1<<(4*(TX_DMA_NUM-1)+1))
	#define DMAx_RESET_MASK 	(0xF<<(4*(TX_DMA_NUM-1)))
	#define RX_DMA_CHANNEL 		DMA1_Channel5 
	#define SET_APBxENR  		RCC->APB2ENR|=RCC_USART1EN                   // enable clock for USART1
	#define SET_BAUD_RATE   	USART1->BRR=__USART_BRR(__PCLK2, __USART_BAUDRATE) // set baudrate
#endif
#if ( USART_NUM == 2 )
	#define __USART2
	#define USARTx 				USART2
	#define TX_DMA_CHANNEL 		DMA1_Channel7 
	#define TX_DMA_HANDLER 		DMAChannel7_IRQHandler
	#define DMA1_CHANNELx_IRQ 	DMA1_CHANNEL7_BIT_SHIFT
	#define USARTx_IRQChannel 	USART2_BIT_SHIFT
	#define USARTx_IRQHandler 	USART2_IRQHandler
	#define TX_DMA_NUM 			7
	#define DMAx_ISR_MASK 		(1<<(4*(TX_DMA_NUM-1)+1))
	#define DMAx_RESET_MASK 	(0xF<<(4*(TX_DMA_NUM-1)))
	#define RX_DMA_CHANNEL 		DMA1_Channel6 
	#define SET_APBxENR    		RCC->APB1ENR|=RCC_USART2EN                   // enable clock for USART2
	#define SET_BAUD_RATE  		USART2->BRR=__USART_BRR(__PCLK1, __USART_BAUDRATE)
#endif
#if ( USART_NUM == 3 )
	#define __USART3
	#define USARTx 				USART3
	#define TX_DMA_CHANNEL 		DMA1_Channel2 
	#define TX_DMA_HANDLER 		DMAChannel2_IRQHandler
	#define DMA1_CHANNELx_IRQ 	DMA1_CHANNEL2_BIT_SHIFT
	#define USARTx_IRQChannel 	USART3_BIT_SHIFT
	#define USARTx_IRQHandler 	USART3_IRQHandler
	#define TX_DMA_NUM 			2
	#define DMAx_ISR_MASK	 	(1<<(4*(TX_DMA_NUM-1)+1))
	#define DMAx_RESET_MASK 	(0xF<<(4*(TX_DMA_NUM-1)))
	#define RX_DMA_CHANNEL 		DMA1_Channel3 
	#define SET_APBxENR   		RCC->APB1ENR|=RCC_USART3EN                   // enable clock for USART3
	#define SET_BAUD_RATE 		USART3->BRR=__USART_BRR(__PCLK1, __USART_BAUDRATE)
#endif

#ifdef USART_DMA
	#define DMA_BUFFER
#endif

#ifdef USART_IRQ
	#define DMA_BUFFER
	void USARTx_IRQHandler (void);
#endif

#ifdef USART_IRQ
	#define __USART_CR1  		(USART_TXEIE | USART_RXNEIE)
	#define __USART_INTERRUPTS 	(1<<(USART_NUM-1))
#else
	#define __USART_CR1			0x0
	#define __USART_INTERRUPTS	0x0			
#endif

#define __USART_CR2             0x0
#define __USART_CR3             0x0

/*----------------------------------------------------------------------------
 Define  Baudrate setting (BRR) for USARTx 
 *----------------------------------------------------------------------------*/
#define __DIV(__PCLK, __BAUD)       ((__PCLK*25)/(4*__BAUD))
#define __DIVMANT(__PCLK, __BAUD)   (__DIV(__PCLK, __BAUD)/100)
#define __DIVFRAQ(__PCLK, __BAUD)   (((__DIV(__PCLK, __BAUD) - (__DIVMANT(__PCLK, __BAUD) * 100)) * 16 + 50) / 100)
#define __USART_BRR(__PCLK, __BAUD) ((__DIVMANT(__PCLK, __BAUD) << 4)|(__DIVFRAQ(__PCLK, __BAUD) & 0x0F))

#define __AFIO_MAPR               	0x0              //  1

#ifdef DMA_BUFFER
	char rbuff[RBUF_SIZE];
#endif

		/*----------------------------------------------------------------------------
		  Usart initialisation
		 *----------------------------------------------------------------------------*/
		void setup_usart()			//ATTENTION CONFIG  PIN RX AS FLOTING INPUT AND TX AS ALTERNATE FUNCTION
		{
		#ifdef DMA_BUFFER
		 	void buffer_init(void);
		
			buffer_init ();
		#endif
		
			SET_APBxENR ;
		 	SET_BAUD_RATE ;
		    USARTx->CR1  = __USART_DATABITS;                       // set Data bits
		    USARTx->CR2  = __USART_STOPBITS;                       // set Stop bits
		    USARTx->CR1 |= __USART_PARITY;                         // set Parity
		    USARTx->CR3  = __USART_FLOWCTRL;                       // Set Flow Control
		
		    USARTx->CR1 |= (USART_RE | USART_TE);         		   // RX, TX enable
		
			#ifdef USART_IRQ
		      	USARTx->CR1 |= __USART_CR1;
		      	USARTx->CR2 |= __USART_CR2;
		      	USARTx->CR3 |= __USART_CR3;
		      	NVIC->ISER[1] |= (1 << (USARTx_IRQChannel & 0x1F));   // enable interrupt
			#endif
		
		    USARTx->CR1 |= USART_UE;                            // USART enable
		 
			#ifdef USART_DMA
			RCC->AHBENR |= RCC_DMA1EN;					// DMA1 CLOCK ENABLE
			TX_DMA_CHANNEL->CPAR =(u32)&(USARTx->DR); 	// ADRESSE  USART_DR
		 	TX_DMA_CHANNEL->CNDTR=1;	
		  	TX_DMA_CHANNEL->CCR|=(DMA_PL_IS_MEDIUM|DMA_MSIZE_IS_8BITS|DMA_PSIZE_IS_8BITS);	// PRIORITY LEVEL MEDIUM
		  	TX_DMA_CHANNEL->CCR|=DMA_DIR_IS_FROM_MEMORY;   	// READ FROM MEMORY
		  	TX_DMA_CHANNEL->CCR|=DMA_MINC;   			// POINTER INCREMENTALE MEMORY 
		 	
			RX_DMA_CHANNEL->CPAR =(u32)&(USARTx->DR); // ADRESSE  USART_DR
		  	RX_DMA_CHANNEL->CMAR =(u32)rbuff;   		// ADRESSE DATA_RECU
		  	RX_DMA_CHANNEL->CNDTR=RBUF_SIZE;
		  	RX_DMA_CHANNEL->CCR|=(DMA_PL_IS_MEDIUM|DMA_MSIZE_IS_8BITS|DMA_PSIZE_IS_8BITS);	// PRIORITY LEVEL MEDIUM
		  	RX_DMA_CHANNEL->CCR&=(~DMA_DIR_IS_FROM_MEMORY);	// READ FROM PERIPHERAL
		  	RX_DMA_CHANNEL->CCR|=DMA_MINC;   			// POINTER INCREMENTALE MEMORY 
		  	RX_DMA_CHANNEL->CCR|=DMA_CIRC;				// CIRCULAR MODE
		  
		  	//NVIC->ISER[0] = 0x00003000;
		  	NVIC->ISER[0] = 1<<DMA1_CHANNELx_IRQ;
		
		  	USARTx->CR3 |= (USART_DMAT|USART_DMAR);		// DMAT AND DMAR ENABLE
			//USARTx->CR3 |=0x00000700;					// CTS ENABLE ,RTS ENABLE ,CTSIE
		
		   	buffer_init();
		
			TX_DMA_CHANNEL->CCR|=DMA_TCIE; // tc enable interrupt
		   	RX_DMA_CHANNEL->CCR|=DMA_EN;  //ENABLE CHANNEL	 3	  and tc enable    
		  	#endif
		
		//_________________________________________________________________________
		// PORT configuration
		
		#ifdef __USART1                                                    
		
		    AFIO->MAPR  &= ~(1 << 2);                        	// clear USART1 remap
		
		    if ((__USART1_REMAP & 0x04) == 0x00)              	// USART1 no remap
			{
		      	RCC->APB2ENR |= RCC_IOPAEN;                   	// enable clock for GPIOA
		      	GPIOA->CRH   &= ~(0xFFUL  << 4);              	// Clear PA9, PA10
		      	GPIOA->CRH   |=  (0x0BUL  << 4);              	// USART1 Tx (PA9)  alternate output push-pull
		      	GPIOA->CRH   |=  (0x04UL  << 8);              	// USART1 Rx (PA10) input floating
		    }
		    else                                              	// USART1    remap
			{
		      	RCC->APB2ENR |= RCC_AFIOEN;                   	// enable clock for Alternate Function
		      	AFIO->MAPR   |= __USART1_REMAP;               	// set   USART1 remap
		      	RCC->APB2ENR |= RCC_IOPBEN;                   	// enable clock for GPIOB
		      	GPIOB->CRL   &= ~(0xFFUL  << 24);             	// Clear PB6, PB7
		      	GPIOB->CRL   |=  (0x0BUL  << 24);             	// USART1 Tx (PB6)  alternate output push-pull
		      	GPIOB->CRL   |=  (0x04UL  << 28);             	// USART1 Rx (PB7) input floating
		    }
		#endif /* __USART1 */
		
		#ifdef __USART2                                                    
		
		    AFIO->MAPR  &= ~(1 << 3);                         	// clear USART2 remap
		
		    if ((__USART2_REMAP & 0x08) == 0x00)         		// USART2 no remap
			{
		      	RCC->APB2ENR |=  RCC_IOPAEN;                  	// enable clock for GPIOA
		      	GPIOA->CRL   &= ~(0xFFUL  << 8);            	// Clear PA2, PA3
		      	GPIOA->CRL   |=  (0x0BUL  << 8);             	// USART2 Tx (PA2)  alternate output push-pull
		      	GPIOA->CRL   |=  (0x04UL  << 12);           	// USART2 Rx (PA3)  input floating
		      
			  	if (__USART_FLOWCTRL & 0x0300)            		// HW flow control enabled
		        {
					GPIOA->CRL   &= ~(0xFFUL  << 0);       		// Clear PA0, PA1
		        	GPIOA->CRL   |=  (0x04UL  << 0);         	// USART2 CTS (PA0) input floating
		        	GPIOA->CRL   |=  (0x0BUL  << 4);          	// USART2 RTS (PA1) alternate output push-pull
		      	}
		    }                                                
		    else                                        		// USART2    remap
		    {
				RCC->APB2ENR |= RCC_AFIOEN;                   	// enable clock for Alternate Function
		      	AFIO->MAPR   |= __USART2_REMAP;         		// set   USART2 remap
		      	RCC->APB2ENR |=  RCC_IOPDEN;                  	// enable clock for GPIOD
		      	GPIOD->CRL   &= ~(0xFFUL  << 20);           	// Clear PD5, PD6
		      	GPIOD->CRL   |=  (0x0BUL  << 20);           	// USART2 Tx (PD5)  alternate output push-pull
		      	GPIOD->CRL   |=  (0x04UL  << 24);           	// USART2 Rx (PD6)  input floating
		      
			  	if (__USART_FLOWCTRL & 0x0300)             		// HW flow control enabled
		        {
					GPIOD->CRL   &= ~(0xFFUL  << 12);        	// Clear PD3, PD4
		        	GPIOD->CRL   |=  (0x04UL  << 12);         	// USART2 CTS (PD3) input floating
		        	GPIOD->CRL   |=  (0x0BUL  << 16);         	// USART2 RTS (PD4) alternate output push-pull
		      	}
		    }
		#endif /* __USART2 */
		
		#ifdef __USART3                                                    
		
		    AFIO->MAPR  &= ~(3 << 4);                     		// clear USART3 remap
		
		    if ((__USART3_REMAP & 0x30) == 0x00)              	// USART3 no remap
			{
		      	RCC->APB2ENR |=  RCC_IOPBEN;                  	// enable clock for GPIOB
		      	GPIOB->CRH   &= ~(0xFFUL  <<  8);  				// Clear PB10, PB11
		      	GPIOB->CRH   |=  (0x0BUL  <<  8);     			// USART3 Tx (PB10) alternate output push-pull
		      	GPIOB->CRH   |=  (0x04UL  << 12);     			// USART3 Rx (PB11) input floating
		      
			  	if (__USART_FLOWCTRL & 0x0300)             		// HW flow control enabled
		        {
					GPIOB->CRH   &= ~(0xFFUL  << 20);        	// Clear PB13, PB14
		        	GPIOB->CRH   |=  (0x04UL  << 20);      		// USART3 CTS (PB13) input floating
		        	GPIOB->CRH   |=  (0x0BUL  << 24);        	// USART3 RTS (PB14) alternate output push-pull
		      	}
		    }
		    else if ((__USART3_REMAP & 0x30) == 0x10)       	// USART3 partial remap
			{
		      	RCC->APB2ENR |= RCC_AFIOEN;                   	// enable clock for Alternate Function
		      	AFIO->MAPR   |= __USART3_REMAP;           		// set   USART3 remap
		      	RCC->APB2ENR |=  RCC_IOPCEN;                  	// enable clock for GPIOC
		      	GPIOC->CRH   &= ~(0xFFUL  <<  8);           	// Clear PC10, PC11
		      	GPIOC->CRH   |=  (0x0BUL  <<  8);           	// USART3 Tx (PC10) alternate output push-pull
		      	GPIOC->CRH   |=  (0x04UL  << 12);            	// USART3 Rx (PC11) input floating
		
		      	if (__USART_FLOWCTRL & 0x0300)             		// HW flow control enabled
				{
		        	RCC->APB2ENR |=  RCC_IOPBEN;         		// enable clock for GPIOB
		        	GPIOB->CRH   &= ~(0xFFUL  << 20);       	// Clear PB13, PB14
		        	GPIOB->CRH   |=  (0x04UL  << 20);      		// USART3 CTS (PB13) input floating
		       	 	GPIOB->CRH   |=  (0x0BUL  << 24);         	// USART3 RTS (PB14) alternate output push-pull
		      	}
		    }
		    else                                      			// USART3 full remap
			{
		      	RCC->APB2ENR |= RCC_AFIOEN;                   	// enable clock for Alternate Function
		      	AFIO->MAPR   |= __USART3_REMAP;          		// set   USART3 remap
		      	RCC->APB2ENR |=  RCC_IOPDEN;                  	// enable clock for GPIOD
		      	GPIOD->CRH   &= ~(0xFFUL  <<  0);           	// Clear PD8, PD9
		      	GPIOD->CRH   |=  (0x0BUL  <<  0);           	// USART3 Tx (PD8) alternate output push-pull
		      	GPIOD->CRH   |=  (0x04UL  <<  4);            	// USART3 Rx (PD9) input floating
		      
			  	if (__USART_FLOWCTRL & 0x0300)              	// HW flow control enabled
				{
		        	GPIOD->CRH   &= ~(0xFFUL  << 12);         	// Clear PD11, PD12
		        	GPIOD->CRH   |=  (0x04UL  << 12);       	// USART3 CTS (PD11) input floating
		        	GPIOD->CRH   |=  (0x0BUL  << 16);        	// USART3 RTS (PD12) alternate output push-pull
		      	}
		    } 
		#endif /* __USART3 */
		}
		
		/*****************************************************************************
		 *  Start polling part of code
		 *****************************************************************************/
		#ifdef USART_POL
		//_______________________________________________________________________
		//  Redéfinition de fputc et fgetc pour stdio.h (si __MINILIB__ n'est pas defini
		// Sinon, definition de usart_write et usart_read
		//________________________________________________________________________
		
		/*----------------------------------------------------------------------------
		  fputc
		 *----------------------------------------------------------------------------*/
		#if !defined __MINILIB__
			int fputc(int ch, FILE *f) 
		#else
			char minilib_write(int file, char ch) 
		#endif
		{
		  	while (!(USARTx->SR & USART_TXE));
		
		  	USARTx->DR = (ch & 0x0FF);
		
		  	return (ch);
		}
		
		/*----------------------------------------------------------------------------
		  fgetc
		 *----------------------------------------------------------------------------*/
		#if !defined __MINILIB__
			int fgetc(FILE *f)
		#else
			char minilib_read(void)
		#endif
		{
			while (!(USARTx->SR & USART_RXNE));
		
		  	return ((int)(USARTx->DR & 0x0FF));
		}
		#endif /* USART_POL */
		/*****************************************************************************
		 *  End Polling part of code
		 *****************************************************************************/
		
		/*****************************************************************************
		 *  Start DMA part of code
		 *****************************************************************************/
		#ifdef USART_DMA
		#define END_RBUF  (&(rbuff[RBUF_SIZE-1]))
		
		char *in_ptr = rbuff;
		char tbuff[TBUF_SIZE];
		#define END_TBUF  (&(tbuff[TBUF_SIZE-1]))
		
		char *send_ptr=tbuff;
		char *pdma;
		char tbuffer_empty=1;
		short int dma_size;
		
		/*----------------------------------------------------------------------------
		  buffer initialisation
		 *----------------------------------------------------------------------------*/
		void buffer_init(void)
		{ 
			int i;
			for (i=0;i<RBUF_SIZE;i++) rbuff[i] = 0;
		};
		
		/*----------------------------------------------------------------------------
		  fputc
		 *----------------------------------------------------------------------------*/
		#if !defined __MINILIB__
			int fputc(int ch, FILE *f) 
		#else
			char minilib_write(int file, char ch)
		#endif
		{
		int i;
		 
			if (!tbuffer_empty)
		 	{
				while (send_ptr == (char *) (TX_DMA_CHANNEL->CMAR)) 
				{
					i = i+1;
				}
		 
		 		*send_ptr=ch;
		   
		   		if(send_ptr>=END_TBUF)
		      	{
			  		send_ptr=tbuff;
			  	}
			  	else send_ptr++;
		   	 }
			 else
			 {
			 	*send_ptr=ch;
		   
		 		// send single char to DMA
				TX_DMA_CHANNEL->CCR &= (~DMA_EN);    //disABLE CHANNEL		 
				TX_DMA_CHANNEL->CNDTR=1;
				dma_size=TX_DMA_CHANNEL->CNDTR;  //save buff size because CNDTR is cleared on IRQ
				TX_DMA_CHANNEL->CMAR=(unsigned int) send_ptr;
			    tbuffer_empty=0;
		
		   		if(send_ptr>=END_TBUF)
		      	{
			  		send_ptr=tbuff;
			  	}
			  	else send_ptr++;
		
				TX_DMA_CHANNEL->CCR |= DMA_EN;
			}
		   
			return ch;
		}
		
		/*----------------------------------------------------------------------------
		  fgetc
		 *----------------------------------------------------------------------------*/
		#if !defined __MINILIB__
			int fgetc(FILE *f)
		#else
			char minilib_read(void)
		#endif
		{
		int ch;
		  
		  	while(*in_ptr == 0);
		  
		  	ch = *in_ptr;
		  
		  	*(in_ptr++) = 0;
		  	if (in_ptr > END_RBUF) in_ptr=rbuff;
		  	
		  	return (ch);
		} 
		
		/*----------------------------------------------------------------------------
		  uart_buffer_full
		 *----------------------------------------------------------------------------*/
		int uart_buffer_full(void)
		{
			return *in_ptr; 
		}
		 
		/*----------------------------------------------------------------------------
		  DMA IRQ handler
		 *----------------------------------------------------------------------------*/
		void TX_DMA_HANDLER (void)
		{	
		char * pDMA ;
				 
			if ( (DMA1->ISR & DMAx_ISR_MASK) && !tbuffer_empty ) 	  //end of conversion interrupt
		    {
				pDMA = (char *) (TX_DMA_CHANNEL->CMAR + dma_size);
		   	
				if ( pDMA>END_TBUF )
		  		{
					pDMA=tbuff;
				}
			
				if ( pDMA == send_ptr) 
				{
					tbuffer_empty = 1;
				}
				else
				{
					if (send_ptr>pDMA)
					{
					    // DMA up to send_ptr
						TX_DMA_CHANNEL->CCR &= (~DMA_EN);    //disABLE CHANNEL	 
						TX_DMA_CHANNEL->CNDTR=(send_ptr-pDMA);
						dma_size=TX_DMA_CHANNEL->CNDTR;  //save buff size because CNDTR is cleared on IRQ
						TX_DMA_CHANNEL->CMAR= (unsigned int) pDMA;
						TX_DMA_CHANNEL->CCR |= DMA_EN;  
					}
					else
					{	// DMA up to end of buffer
						TX_DMA_CHANNEL->CCR &= (~DMA_EN);    //disABLE CHANNEL	 
						TX_DMA_CHANNEL->CNDTR=(END_TBUF-pDMA+1);
						dma_size=TX_DMA_CHANNEL->CNDTR;  //save buff size because CNDTR is cleared on IRQ
						TX_DMA_CHANNEL->CMAR=(unsigned int) pDMA;
						TX_DMA_CHANNEL->CCR |= DMA_EN;  
					}
				}	   		
			}
		
			DMA1->IFCR |= (DMAx_RESET_MASK); //reset interrupt flag
		}
		#endif /* USART_DMA */
		/*****************************************************************************
		 *  End DMA part of code
		 *****************************************************************************/
		
		/*****************************************************************************
		 *  Start IRQ part of code
		 *****************************************************************************/
		#ifdef USART_IRQ
		/*----------------------------------------------------------------------------
		 *----------------------------------------------------------------------------*/
		#if TBUF_SIZE < 2
			#error TBUF_SIZE is too small.  It must be larger than 1.
		#elif ((TBUF_SIZE & (TBUF_SIZE-1)) != 0)
			#error TBUF_SIZE must be a power of 2.
		#endif
		
		#if RBUF_SIZE < 2
			#error RBUF_SIZE is too small.  It must be larger than 1.
		#elif ((RBUF_SIZE & (RBUF_SIZE-1)) != 0)
			#error RBUF_SIZE must be a power of 2.
		#endif
		
		/*----------------------------------------------------------------------------
		 *----------------------------------------------------------------------------*/
		struct buf_st 
		{
		  	unsigned int in;                                // Next In Index
		  	unsigned int out;                               // Next Out Index
		  	char buf [RBUF_SIZE];                           // Buffer
		};
		
		static struct buf_st rbuf = { 0, 0, };
		#define SIO_RBUFLEN ((unsigned short)(rbuf.in - rbuf.out))
		
		static struct buf_st tbuf = { 0, 0, };
		#define SIO_TBUFLEN ((unsigned short)(tbuf.in - tbuf.out))
		
		static unsigned int tx_restart = 1;               // NZ if TX restart is required
		
		/*----------------------------------------------------------------------------
		  USART1_IRQHandler
		  Handles USART1 global interrupt request.
		 *----------------------------------------------------------------------------*/
		void USARTx_IRQHandler (void) 
		{
		volatile unsigned int IIR;
		struct buf_st *p;
		
		    IIR = USARTx->SR;
		    if (IIR & USART_RXNE)                   // read interrupt
			{
		    	USARTx->SR &= ~USART_RXNE;	          // clear interrupt
		      	p = &rbuf;
		
		      	if (((p->in - p->out) & ~(RBUF_SIZE-1)) == 0) 
				{
		        	p->buf [p->in & (RBUF_SIZE-1)] = (USARTx->DR & 0x1FF);
		        	p->in++;
		      	}
			}
		
		    if (IIR & USART_TXE) 
			{
		      	USARTx->SR &= ~USART_TXE;	          // clear interrupt
				p = &tbuf;
		
		      	if (p->in != p->out) 
				{
		        	USARTx->DR = (p->buf [p->out & (TBUF_SIZE-1)] & 0x1FF);
		        	p->out++;
		        	tx_restart = 0;
		      	}
		      	else 
				{
		        	tx_restart = 1;
					USARTx->CR1 &= ~USART_TXE;		      // disable TX interrupt if nothing to send
		      	}
		    }
		}
		
		/*------------------------------------------------------------------------------
		  buffer_Init
		  initialize the buffers
		 *------------------------------------------------------------------------------*/
		void buffer_init (void) 
		{
		  	tbuf.in = 0;                                    // Clear com buffer indexes
		  	tbuf.out = 0;
		  	tx_restart = 1;
		
		  	rbuf.in = 0;
		  	rbuf.out = 0;
		}
		
		/*------------------------------------------------------------------------------
		  SenChar
		  transmit a character
		 *------------------------------------------------------------------------------*/
		int SendChar (int c) 
		{
		struct buf_st *p = &tbuf;
		                                                  // If the buffer is full, return an error value
		  	if (SIO_TBUFLEN >= TBUF_SIZE) return (-1);
		                                                  
		  	p->buf [p->in & (TBUF_SIZE - 1)] = c;           // Add data to the transmit buffer.
		  	p->in++;
		
		  	if (tx_restart)                                // If transmit interrupt is disabled, enable it
			{
		    	tx_restart = 0;
				USARTx->CR1 |= USART_TXE;		          // enable TX interrupt
		  	}
		
		  	return (0);
		}
		
		/*------------------------------------------------------------------------------
		  GetKey
		  receive a character
		 *------------------------------------------------------------------------------*/
		int GetKey (void) 
		{
		struct buf_st *p = &rbuf;
		
			if (SIO_RBUFLEN == 0) return (-1);
		
		  	return (p->buf [(p->out++) & (RBUF_SIZE - 1)]);
		}
		
		//________________________________________________________________________________
		//________________________________________________________________________________
		// extrait de boards/keil/stm32/USART_irg/Retarget.c
		//________________________________________________________________________________
		//________________________________________________________________________________
		
		/*----------------------------------------------------------------------------
		  fputc
		 *----------------------------------------------------------------------------*/
		#if !defined __MINILIB__
			int fputc(int ch, FILE *f)
		#else
			char minilib_write(int file, char ch)
		#endif
		{
		  	return (SendChar(ch));
		}
		
		/*----------------------------------------------------------------------------
		  fgetc
		 *----------------------------------------------------------------------------*/
		#if !defined __MINILIB__
			int fgetc(FILE *f) 
		#else
			char minilib_read(int file)
		#endif
		{
		int ch;
		
			do 
			{
		    	ch = GetKey ();
		  	} while (ch == -1);
		  	
		  	return ((char)ch);
		}

	}
}
#endif /* USART_IRQ */

/*****************************************************************************
 *  End IRQ part of code
 *****************************************************************************/
