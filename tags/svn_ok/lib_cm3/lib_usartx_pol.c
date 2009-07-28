#include <stm32f10x_type.h>                        // STM32F10x Library Definitions
#include <stm32f10x_nvic.h>                        // STM32F10x Library Definitions
#include <stm32f10x_usart.h>                        // STM32F10x Library Definitions

#include "STM32_Reg.h"          // missing bit definitions
#include "lib_usartx_pol.h"     // import configuration NUM_USART and BAUDRATEx
#include <stdio.h>				// import FILE type


//#pragma import(__use_no_semihosting_swi)  // ACCO !!! pas pigé l'histoire TODO

// Je récupère les bouts de STM32_Init utiles à l'USART
// 
// USART conf modifiée pour être configurée par lib_usartx_pol.h
#define __USART_SETUP  1
                   
#if    (NUM_USART == 1)
	#define _USART1
	#define USARTx USART1
#elif   (NUM_USART == 2)
	#define _USART2                
	#define USARTx USART2
#elif   (NUM_USART == 3)
	#define _USART3                
	#define USARTx USART3
#endif
#define __USART_USED	(1<<(NUM_USART-1))


#define __USART_SETUP             1              
#define __USART_DETAILS           0x00			
#define __USART_INTERRUPTS        0x00			
#define __USART1_BAUDRATE         BAUDRATEx		
#define __USART1_DATABITS         0x00000000
#define __USART1_STOPBITS         0x00000000
#define __USART1_PARITY           0x00000000
#define __USART1_FLOWCTRL         0x00000000
#define __USART1_REMAP            0x00000000
#define __USART1_CR1              0x00000000
#define __USART1_CR2              0x00000000
#define __USART1_CR3              0x00000000
#define __USART2_BAUDRATE         BAUDRATEx	          
#define __USART2_DATABITS         0x00000000
#define __USART2_STOPBITS         0x00000000
#define __USART2_PARITY           0x00000000
#define __USART2_FLOWCTRL         0x00000000
#define __USART2_REMAP            0x00000000
#define __USART2_CR1              0x00000000
#define __USART2_CR2              0x00000000
#define __USART2_CR3              0x00000000
#define __USART3_BAUDRATE         BAUDRATEx	                
#define __USART3_DATABITS         0x00000000
#define __USART3_STOPBITS         0x00000000
#define __USART3_PARITY           0x00000000
#define __USART3_FLOWCTRL         0x00000000
#define __USART3_REMAP            0x00000000
#define __USART3_CR1              0x00000000
#define __USART3_CR2              0x00000000
#define __USART3_CR3              0x00000000



/*----------------------------------------------------------------------------
 Define SYSCLK
 *----------------------------------------------------------------------------*/
#define __HSI 8000000UL
//#define __HSE 8000000UL
#define __PLLMULL (((__RCC_CFGR_VAL & CFGR_PLLMULL_MASK) >> 18) + 2)

#if   ((__RCC_CFGR_VAL & CFGR_SW_MASK) == 0x00) 
  #define __SYSCLK   __HSI                        // HSI is used as system clock
#elif ((__RCC_CFGR_VAL & CFGR_SW_MASK) == 0x01)
  #define __SYSCLK   __HSE                        // HSE is used as system clock
#elif ((__RCC_CFGR_VAL & CFGR_SW_MASK) == 0x02)
  #if (__RCC_CFGR_VAL & CFGR_PLLSRC_MASK)         // HSE is PLL clock source
    #if (__RCC_CFGR_VAL & CFGR_PLLXTPRE_MASK)     // HSE/2 is used
      #define __SYSCLK  ((__HSE >> 1) * __PLLMULL)// SYSCLK = HSE/2 * pllmull
    #else                                         // HSE is used
      #define __SYSCLK  ((__HSE >> 0) * __PLLMULL)// SYSCLK = HSE   * pllmul
    #endif  
  #else                                           // HSI/2 is PLL clock source
    #define __SYSCLK  ((__HSI >> 1) * __PLLMULL)  // SYSCLK = HSI/2 * pllmul
  #endif
#else
   #error "ask for help"
#endif

/*----------------------------------------------------------------------------
 Define  HCLK
 *----------------------------------------------------------------------------*/
#define __HCLKPRESC  ((__RCC_CFGR_VAL & CFGR_HPRE_MASK) >> 4)
#if (__HCLKPRESC & 0x08)
  #define __HCLK        (__SYSCLK >> ((__HCLKPRESC & 0x07)+1))
#else
  #define __HCLK        (__SYSCLK)
#endif

/*----------------------------------------------------------------------------
 Define  PCLK1
 *----------------------------------------------------------------------------*/
#define __PCLK1PRESC  ((__RCC_CFGR_VAL & CFGR_PRE1_MASK) >> 8)
#if (__PCLK1PRESC & 0x04)
  #define __PCLK1       (__HCLK >> ((__PCLK1PRESC & 0x03)+1))
#else
  #define __PCLK1       (__HCLK)
#endif

/*----------------------------------------------------------------------------
 Define  PCLK2
 *----------------------------------------------------------------------------*/
#define __PCLK2PRESC  ((__RCC_CFGR_VAL & CFGR_PRE2_MASK) >> 11)
#if (__PCLK2PRESC & 0x04)
  #define __PCLK2       (__HCLK >> ((__PCLK2PRESC & 0x03)+1))
#else
  #define __PCLK2       (__HCLK)
#endif

/*----------------------------------------------------------------------------
 Define  Baudrate setting (BRR) for USART1 
 *----------------------------------------------------------------------------*/
#define __DIV(__PCLK, __BAUD)       ((__PCLK*25)/(4*__BAUD))
#define __DIVMANT(__PCLK, __BAUD)   (__DIV(__PCLK, __BAUD)/100)
#define __DIVFRAQ(__PCLK, __BAUD)   (((__DIV(__PCLK, __BAUD) - (__DIVMANT(__PCLK, __BAUD) * 100)) * 16 + 50) / 100)
#define __USART_BRR(__PCLK, __BAUD) ((__DIVMANT(__PCLK, __BAUD) << 4)|(__DIVFRAQ(__PCLK, __BAUD) & 0x0F))

#define __AFIO_MAPR               0x00000000              //  1

//#include <stm32f10x_usart.h>                        // STM32F10x Library Definitions


// fonction modifiée à partir de USART_config() de STM32_Init.c
void usartx_setup (void) {
#ifdef _USART1                                                    
//  if (__USART_USED & 0x01) {                                // USART1 used

    AFIO->MAPR   &= ~(1 << 2);                              // clear USART1 remap
    if      ((__USART1_REMAP & 0x04) == 0x00) {             // USART1 no remap
      RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;                   // enable clock for GPIOA
      GPIOA->CRH   &= ~(0xFFUL  << 4);                      // Clear PA9, PA10
      GPIOA->CRH   |=  (0x0BUL  << 4);                      // USART1 Tx (PA9)  alternate output push-pull
      GPIOA->CRH   |=  (0x04UL  << 8);                      // USART1 Rx (PA10) input floating
    }
    else {                                                  // USART1    remap
      RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                   // enable clock for Alternate Function
      AFIO->MAPR   |= __USART1_REMAP;                       // set   USART1 remap
      RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;                   // enable clock for GPIOB
      GPIOB->CRL   &= ~(0xFFUL  << 24);                     // Clear PB6, PB7
      GPIOB->CRL   |=  (0x0BUL  << 24);                     // USART1 Tx (PB6)  alternate output push-pull
      GPIOB->CRL   |=  (0x04UL  << 28);                     // USART1 Rx (PB7) input floating
    }

    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;                   // enable clock for USART1
        
    USART1->BRR  = __USART_BRR(__PCLK2, __USART1_BAUDRATE); // set baudrate
    USART1->CR1  = __USART1_DATABITS;                       // set Data bits
    USART1->CR2  = __USART1_STOPBITS;                       // set Stop bits
    USART1->CR1 |= __USART1_PARITY;                         // set Parity
    USART1->CR3  = __USART1_FLOWCTRL;                       // Set Flow Control

    USART1->CR1 |= (USART_CR1_RE | USART_CR1_TE);           // RX, TX enable

    if (__USART_INTERRUPTS & 0x01) {                        // interrupts used
      USART1->CR1 |= __USART1_CR1;
      USART1->CR2 |= __USART1_CR2;
      USART1->CR3 |= __USART1_CR3;
      NVIC->ISER[1] |= (1 << (USART1_IRQChannel & 0x1F));   // enable interrupt
    }

    USART1->CR1 |= USART_CR1_UE;                            // USART enable
//  } // end USART1 used
#endif
#ifdef _USART2                                                    

//  if (__USART_USED & 0x02) {                                // USART2 used

    AFIO->MAPR   &= ~(1 << 3);                              // clear USART2 remap
    if      ((__USART2_REMAP & 0x08) == 0x00) {             // USART2 no remap
      RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN;                  // enable clock for GPIOA
      GPIOA->CRL   &= ~(0xFFUL  << 8);                      // Clear PA2, PA3
      GPIOA->CRL   |=  (0x0BUL  << 8);                      // USART2 Tx (PA2)  alternate output push-pull
      GPIOA->CRL   |=  (0x04UL  << 12);                     // USART2 Rx (PA3)  input floating
      if (__USART2_FLOWCTRL & 0x0300) {                     // HW flow control enabled
        GPIOA->CRL   &= ~(0xFFUL  << 0);                    // Clear PA0, PA1
        GPIOA->CRL   |=  (0x04UL  << 0);                    // USART2 CTS (PA0) input floating
        GPIOA->CRL   |=  (0x0BUL  << 4);                    // USART2 RTS (PA1) alternate output push-pull
      }
    }                                                
    else {                                                  // USART2    remap
      RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                   // enable clock for Alternate Function
      AFIO->MAPR   |= __USART2_REMAP;                       // set   USART2 remap
      RCC->APB2ENR |=  RCC_APB2ENR_IOPDEN;                  // enable clock for GPIOD
      GPIOD->CRL   &= ~(0xFFUL  << 20);                     // Clear PD5, PD6
      GPIOD->CRL   |=  (0x0BUL  << 20);                     // USART2 Tx (PD5)  alternate output push-pull
      GPIOD->CRL   |=  (0x04UL  << 24);                     // USART2 Rx (PD6)  input floating
      if (__USART2_FLOWCTRL & 0x0300) {                     // HW flow control enabled
        GPIOD->CRL   &= ~(0xFFUL  << 12);                   // Clear PD3, PD4
        GPIOD->CRL   |=  (0x04UL  << 12);                   // USART2 CTS (PD3) input floating
        GPIOD->CRL   |=  (0x0BUL  << 16);                   // USART2 RTS (PD4) alternate output push-pull
      }
    }

    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;                   // enable clock for USART2

    USART2->BRR  = __USART_BRR(__PCLK1, __USART2_BAUDRATE); // set baudrate
    USART2->CR1  = __USART2_DATABITS;                       // set Data bits
    USART2->CR2  = __USART2_STOPBITS;                       // set Stop bits
    USART2->CR1 |= __USART2_PARITY;                         // set Parity
    USART2->CR3  = __USART2_FLOWCTRL;                       // Set Flow Control

    USART2->CR1 |= (USART_CR1_RE | USART_CR1_TE);           // RX, TX enable

    if (__USART_INTERRUPTS & 0x02) {                        // interrupts used
      USART2->CR1 |= __USART2_CR1;
      USART2->CR2 |= __USART2_CR2;
      USART2->CR3 |= __USART2_CR3;
      NVIC->ISER[1] |= (1 << (USART2_IRQChannel & 0x1F));   // enable interrupt
    }

    USART2->CR1 |= USART_CR1_UE;                            // USART enable

// } // end USART2 used
#endif
#ifdef _USART3                                                    
//  if (__USART_USED & 0x04) {                                // USART3 used


    AFIO->MAPR   &= ~(3 << 4);                              // clear USART3 remap
    if      ((__USART3_REMAP & 0x30) == 0x00) {             // USART3 no remap
      RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;                  // enable clock for GPIOB
      GPIOB->CRH   &= ~(0xFFUL  <<  8);                     // Clear PB10, PB11
      GPIOB->CRH   |=  (0x0BUL  <<  8);                     // USART3 Tx (PB10) alternate output push-pull
      GPIOB->CRH   |=  (0x04UL  << 12);                     // USART3 Rx (PB11) input floating
      if (__USART3_FLOWCTRL & 0x0300) {                     // HW flow control enabled
        GPIOB->CRH   &= ~(0xFFUL  << 20);                   // Clear PB13, PB14
        GPIOB->CRH   |=  (0x04UL  << 20);                   // USART3 CTS (PB13) input floating
        GPIOB->CRH   |=  (0x0BUL  << 24);                   // USART3 RTS (PB14) alternate output push-pull
      }
    }
    else if ((__USART3_REMAP & 0x30) == 0x10) {             // USART3 partial remap
      RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                   // enable clock for Alternate Function
      AFIO->MAPR   |= __USART3_REMAP;                       // set   USART3 remap
      RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN;                  // enable clock for GPIOC
      GPIOC->CRH   &= ~(0xFFUL  <<  8);                     // Clear PC10, PC11
      GPIOC->CRH   |=  (0x0BUL  <<  8);                     // USART3 Tx (PC10) alternate output push-pull
      GPIOC->CRH   |=  (0x04UL  << 12);                     // USART3 Rx (PC11) input floating
      if (__USART3_FLOWCTRL & 0x0300) {                     // HW flow control enabled
        RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;                // enable clock for GPIOB
        GPIOB->CRH   &= ~(0xFFUL  << 20);                   // Clear PB13, PB14
        GPIOB->CRH   |=  (0x04UL  << 20);                   // USART3 CTS (PB13) input floating
        GPIOB->CRH   |=  (0x0BUL  << 24);                   // USART3 RTS (PB14) alternate output push-pull
      }
    }
    else {                                                  // USART3 full remap
      RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                   // enable clock for Alternate Function
      AFIO->MAPR   |= __USART3_REMAP;                       // set   USART3 remap
      RCC->APB2ENR |=  RCC_APB2ENR_IOPDEN;                  // enable clock for GPIOD
      GPIOD->CRH   &= ~(0xFFUL  <<  0);                     // Clear PD8, PD9
      GPIOD->CRH   |=  (0x0BUL  <<  0);                     // USART3 Tx (PD8) alternate output push-pull
      GPIOD->CRH   |=  (0x04UL  <<  4);                     // USART3 Rx (PD9) input floating
      if (__USART3_FLOWCTRL & 0x0300) {                     // HW flow control enabled
        GPIOD->CRH   &= ~(0xFFUL  << 12);                   // Clear PD11, PD12
        GPIOD->CRH   |=  (0x04UL  << 12);                   // USART3 CTS (PD11) input floating
        GPIOD->CRH   |=  (0x0BUL  << 16);                   // USART3 RTS (PD12) alternate output push-pull
      }
    } 

    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;                   // enable clock for USART3

    USART3->BRR  = __USART_BRR(__PCLK1, __USART3_BAUDRATE); // set baudrate
    USART3->CR1  = __USART3_DATABITS;                       // set Data bits
    USART3->CR2  = __USART3_STOPBITS;                       // set Stop bits
    USART3->CR1 |= __USART3_PARITY;                         // set Parity
    USART3->CR3  = __USART3_FLOWCTRL;                       // Set Flow Control

    USART3->CR1 |= (USART_CR1_RE | USART_CR1_TE);           // RX, TX enable

    if (__USART_INTERRUPTS & 0x04) {                        // interrupts used
      USART3->CR1 |= __USART3_CR1;
      USART3->CR2 |= __USART3_CR2;
      USART3->CR3 |= __USART3_CR3;
      NVIC->ISER[1] |= (1 << (USART3_IRQChannel & 0x1F));   // enable interrupt
    }

    USART3->CR1 |= USART_CR1_UE;                            // USART enable
//  } // end USART3 used
#endif
#ifdef _USART1                                                    
//if (__AFIO_MAPR & (1 << 2)) {                             // USART1 remap used 

    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                     // enable clock for Alternate Function
    AFIO->MAPR &= ~(1 << 2);                                // clear used bit
    AFIO->MAPR |= ((1 << 2) & __AFIO_MAPR);                 // set used bits
//  } // end USART1 remap used
#endif
#ifdef _USART2                                                    
//  if (__AFIO_MAPR & (1 << 3)) {                             // USART2 remap used 

    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                     // enable clock for Alternate Function
    AFIO->MAPR &= ~(1 << 3);                                // clear used bit
    AFIO->MAPR |= ((1 << 3) & __AFIO_MAPR);                 // set used bits
//  } // end USART2 remap used
#endif
#ifdef _USART3                                                    
//  if (__AFIO_MAPR & (3 << 4)) {                             // USART3 remap used 

    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;                     // enable clock for Alternate Function
    AFIO->MAPR &= ~(3 << 4);                                // clear used bit
    AFIO->MAPR |= ((3 << 4) & __AFIO_MAPR);                 // set used bits

//  } // end USART3 remap used
#endif

} // end of stm32_UsartSetup






//_______________________________________________________________________
//  Redéfinition de fputc et fgetc pour stdio.h
//________________________________________________________________________


/*----------------------------------------------------------------------------
  external functions
 *----------------------------------------------------------------------------*/

struct __FILE {
  int handle;                 // Add whatever you need here 
};
FILE __stdout;
FILE __stdin;


/*----------------------------------------------------------------------------
  fputc
 *----------------------------------------------------------------------------*/
int fputc(int ch, FILE *f) {
  while (!(USARTx->SR & USART_FLAG_TXE));
  USARTx->DR = (ch & 0x1FF);

  return (ch);

}

/*----------------------------------------------------------------------------
  fgetc
 *----------------------------------------------------------------------------*/
int fgetc(FILE *f) {
  while (!(USARTx->SR & USART_FLAG_RXNE));

  return ((int)(USARTx->DR & 0x1FF));
}

/*----------------------------------------------------------------------------
  _ttywrch
 *----------------------------------------------------------------------------*
void _ttywrch(int ch) {
 SendChar (ch);
}*/

/*----------------------------------------------------------------------------
  ferror
 *----------------------------------------------------------------------------*
int ferror(FILE *f) {
                              // Your implementation of ferror
  return EOF;
} */

/*----------------------------------------------------------------------------
  _sys_exit
 *----------------------------------------------------------------------------*
void _sys_exit(int return_code) {
label:  goto label;           // endless loop
}*/




