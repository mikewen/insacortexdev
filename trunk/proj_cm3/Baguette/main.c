#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                           // STM32 Initialization
#include "lcd.h"
#include "led.h"
#include <stdio.h>

extern unsigned char font8data[];

// Main
volatile u32 tmp;
volatile u8 sens;
volatile u8 index_tab;
volatile u8 index_font;

char tableau[]={'I','N','S','A'};
char text[50];

int ADC_min, ADC_max;

volatile u8 tick;

#define MAX_max 2700
#define MAX_min	2400
#define MIN_max	300
#define MIN_min	150

void adc_Init (void) {

//  GPIOA->CRL &= ~0x0000000F;                    // set PIN1 as analog input (see stm32_Init.c)

  //RCC->AHBENR |= (1<<0);                          // enable periperal clock for DMA

  //DMA1_Channel1->CMAR  = (u32)&ADC_ConvertedValue;// set channel1 memory address
  //DMA1_Channel1->CPAR  = (u32)&(ADC1->DR);        // set channel1 peripheral address
  //DMA1_Channel1->CNDTR = 1;                       // transmit 1 word
  //DMA1_Channel1->CCR   = 0x00002520;              // configure DMA channel
  //DMA1_Channel1->CCR  |= (1 << 0);                // DMA Channel 1 enable


  RCC->APB2ENR |= (1<<9);                         // enable periperal clock for ADC1

  ADC1->SQR1  = 0x00000000;                       // only one conversion
  ADC1->SMPR2 = 0x00000028;                       // set sample time channel1 (55,5 cycles)
  //ADC1->SQR3  = 0x00000001;                       // set channel1 as 1st conversion

  ADC1->CR1   = 0x00000100;                       // use independant mode, SCAN mode
  ADC1->CR2   = 0x000E0003;                       // use data align right,continuous conversion
                                                  // EXTSEL = SWSTART 
                                                  // enable ADC, no external Trigger
  //ADC1->CR2  |=  0x00500000;					  // start SW conversion
}

u32 adc_Convert(u8 Channel)
{
u32 tmp;

	do
	{
		ADC1->SQR3  = Channel;                       // set Channel as 1st conversion
		ADC1->CR2  |=  0x00500000;					  // start SW conversion

		while ((ADC1->SR & 0x00000002)!= 0x00000002)
		{
		}

		tmp = ADC1->DR;
	} while ((tmp<=5) || (tmp >=0xFF0));

	if ((ADC_min > tmp) && ((tmp>>4) >0)) ADC_min = tmp;
	if (ADC_max < tmp) ADC_max = tmp;

	return ADC1->DR;
}

/*------------------------------------------------------------------------------
  Systick Interrupt Handler
  SysTick interrupt happens every 10 ms
 *------------------------------------------------------------------------------*/
void SysTick_Handler (void) 
{
u8 caractere;
u8 pattern;

	tick = 1;

  	switch (sens)
	{
	case 0:
		/* rien a faire */
		//GPIOB->ODR = 0x0;
		led_SetValue(0xFF);
		break;
	case 1:
		/* Sens direct */
		caractere = tableau[index_tab];
		pattern = font8data[(caractere*8) + index_font];
		//GPIOB->ODR = (pattern<<8);
		led_SetValue(~pattern);

		index_font++;
		if (index_font>=8)
		{
			index_font = 0;

			index_tab ++;
			if (index_tab>=4)
			{
				sens = 0;
			}

		}
		break;
	case 2:
		/* Sens indirect */
		caractere = tableau[index_tab];
		pattern = font8data[(caractere*8) + index_font];
		//GPIOB->ODR = (pattern<<8);
		led_SetValue(~pattern);

		if (index_font==0)
		{
			index_font = 7;

			if (index_tab==0)
			{
				sens = 0;
			}
			else
			{
				index_tab --;
			}
		}
		else
		{
			index_font--;
		}
		break;
	}
  
} // end SysTick_Handler

void test(void)
{
	lcd_print ("Salut");
	sens = 4; // pour ne pas etre perturbé par la routine d'IT

	while (1)
	{
		if (tick == 1)
		{
			tick = 0;

			set_cursor(0,1);
			lcd_print("                ");
			set_cursor(0,1);

			//tmp = adc_Convert(1); // convertion de l'ADC 1( le potar)
			tmp = adc_Convert(14); // convertion de l'ADC 15( voie X ?)
			if ((ADC_min > tmp) && ((tmp>>4) >0)) ADC_min = tmp;
			if (ADC_max < tmp) ADC_max = tmp;
			sprintf (text, "min %i, max %i", ADC_min>>4, ADC_max>>4);
			lcd_print(text);

			led_SetValue(tmp>>4);
		}
	}
}

int main (void)
{
	stm32_Init();
	adc_Init();
    led_Init();
	lcd_init();

	tmp = 0;
	sens = 0; /* si sens = 0 -> ne rien faire
	                sens = 1 -> Sens direct
					sens = 2 -> Sens indirect */

	index_tab = 0;
	index_font = 0;
	tick = 0;

	/* Effacement de l'ecran */
	lcd_clear();
	set_cursor(0,0);

	ADC_min = 0xFFFF;
	ADC_max = 0;

	/* Fonction de test */
	//test();

	while (1)
	{
		/* Dessus MAX max */
		while (tmp <MAX_max)
		{
			 tmp = adc_Convert(14);
		}

		//GPIOB->ODR = 0x0;
		sens = 0;

	   	/* Dessous MAX min */
		while (tmp >MAX_min)
		{
			 tmp = adc_Convert(14);
		}

		//GPIOB->ODR = 0x00008000;
		sens = 1;
		index_font = 0;	/* debut du pattern a afficher */
		index_tab = 0; /* debut du tableau */

		/* Dessous MIN min */
		while (tmp >MIN_min)
		{
			 tmp = adc_Convert(14);
		}

		//GPIOB->ODR = 0x0;
		sens = 0;

		/* Dessus MIN max */
		while (tmp <MIN_max)
		{
			 tmp = adc_Convert(14);
		}

		//GPIOB->ODR = 0x00000100;
		sens = 2;
		index_font = 7;	/* fin du pattern a afficher */
		index_tab = 3; /* fin du tableau */
	}

}
