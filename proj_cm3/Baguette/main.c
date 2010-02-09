#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions

#include "lib_baguette.h"
#include "lcd.h"
#include "led.h"
#include "ihm.h"
#include <stdio.h>

extern unsigned char font8data[];

// Main
volatile u32 tmp;
volatile u8 sens;
volatile u8 index_tab;
volatile u8 index_font;

char text[50];

u32 ADC_Array[3];

int compteur_ihm;
int Current_ADC;

int ADC_min, ADC_max;

volatile u8 tick;

#define MAX_max 2700
#define MAX_min	2400
#define MIN_max	300
#define MIN_min	150

#define COMPTEUR_IHM_MAX 100

#define ADC_X 1
#define ADC_Y 2

void adc_Init (void) 
{
  Current_ADC = 0;

  RCC->APB2ENR |= (1<<9);                         // enable periperal clock for ADC1

  ADC1->SQR1  = 0x00000000;                       // only one conversion
  ADC1->SMPR2 = 0x00000028;                       // set sample time channel1 (55,5 cycles)
  ADC1->SQR3  = 0x00000001;                       // set channel1 as 1st conversion

  ADC1->CR1   = 0x00000020;                       // use independant mode, EOCIE mode
  ADC1->CR2   = 0x000E0001;                       // use data align right,continuous conversion
                                                  // EXTSEL = SWSTART 
                                                  // enable ADC, no external Trigger
  ADC1->CR2  |=  0x00500000;					  // start SW conversion

  /* Activation de l'IT EOC */
  NVIC->ISER[0] = (0x01<<18);
}

void ADC_IRQHandler	(void)
{
	ADC_Array[Current_ADC] = ADC1->DR;

	Current_ADC++;
	if (Current_ADC>=3) Current_ADC =0;
	
	if (Current_ADC == 0) ADC1->SQR3  = 1;                       // set Channel as 1st conversion
	else if (Current_ADC == 1) ADC1->SQR3  = 14;                       // set Channel as 1st conversion
	else ADC1->SQR3  = 15; 

	ADC1->CR2  |=  0x00500000;					  // start SW conversion
}

u32 adc_Convert(u8 channel)
{
volatile register u32 tmp;

	do
	{
		tmp = ADC_Array[channel];
	} while ((channel !=0) && ((tmp<=5) || (tmp >=0xFF0)));

	if (channel !=0)
	{
		if ((ADC_min > tmp) && ((tmp>>4) >0)) ADC_min = tmp;
		if (ADC_max < tmp) ADC_max = tmp;
	}

	return tmp;
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

	compteur_ihm++;

	if (compteur_ihm >= COMPTEUR_IHM_MAX)
	{
		compteur_ihm = 0;

		ihm_Animate();	
	}

  	switch (sens)
	{
	case 0:
		/* rien a faire */
		//GPIOB->ODR = 0x0;
		led_SetValue(0xFF);
		break;
	case 1:
		/* Sens direct */
		caractere = texte_baguette[index_tab];
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
		caractere = texte_baguette[index_tab];
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
	//stm32_Init();
	//adc_Init();
    //led_Init();
	Init_Baguette();

	Efface_Ecran();
	Ecrit_Ecran('A');

	ihm_Init();

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

	compteur_ihm = 0;
	/* Fonction de test */
	//test();

	while (1)
	{
		/* Dessus MAX max */
		while (tmp <MAX_max)
		{
			 tmp = adc_Convert(ADC_X);
		}

		//GPIOB->ODR = 0x0;
		sens = 0;

	   	/* Dessous MAX min */
		while (tmp >MAX_min)
		{
			 tmp = adc_Convert(ADC_X);
		}

		//GPIOB->ODR = 0x00008000;
		sens = 1;
		index_font = 0;	/* debut du pattern a afficher */
		index_tab = 0; /* debut du tableau */

		/* Dessous MIN min */
		while (tmp >MIN_min)
		{
			 tmp = adc_Convert(ADC_X);
		}

		//GPIOB->ODR = 0x0;
		sens = 0;

		/* Dessus MIN max */
		while (tmp <MIN_max)
		{
			 tmp = adc_Convert(ADC_X);
		}

		//GPIOB->ODR = 0x00000100;
		sens = 2;
		index_font = 7;	/* fin du pattern a afficher */
		index_tab = 3; /* fin du tableau */
	}

}
