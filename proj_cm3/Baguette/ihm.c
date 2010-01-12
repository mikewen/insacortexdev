#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "ihm.h"
#include "lcd.h"

#define BUTTON_CLEAR 0x01
#define BUTTON_SET 0x02
#define ADC_IHM 0

char texte_baguette[4];
int index_tableau;

u32 adc_Convert(u8 Channel);

volatile int adc;
volatile char caractere;

void ihm_ButtonInit(void)
{
}

int ihm_GetButton(int button)
{
int state;

	state = 0;

	if (button == BUTTON_CLEAR)
	{
		/* Bouton clear	== PA0 */
		state = GPIOA->IDR & 0x01;
	}
	else
	{
		/* Bouton set == PC13 */
		state = GPIOC->IDR & (0x01<<13);	
	}

	return state ? 0 : 1;;
}

void ihm_Init(void)
{
	lcd_init(); // Init lcd screen
	
	ihm_ButtonInit();

	set_cursor(0,0);
	lcd_print("Texte:");

	set_cursor(0,1);
	lcd_print("Caractere> "); 

	for (index_tableau=0; index_tableau<4; index_tableau++)
	{
		texte_baguette[index_tableau]=' ';	
	}

	index_tableau=0;
}

void ihm_Animate(void)
{
int i;
	
	adc = adc_Convert(ADC_IHM);
	
	caractere = ((adc*25)/0xFFF) + 'A';
		
	if (ihm_GetButton(BUTTON_SET))
	{
		if (index_tableau<4)
		{
			texte_baguette[index_tableau++]= caractere;
		}	
	}	

	if (ihm_GetButton(BUTTON_CLEAR))
	{
		for (index_tableau=0; index_tableau<4; index_tableau++)
		{
			texte_baguette[index_tableau]=' ';	
		}

		index_tableau=0;	
	}
	
	lcd_clear();
	set_cursor(0,0);
	lcd_print("Texte: ");

	for (i=0; i<4; i++)
	{
		lcd_putchar(texte_baguette[i]);	
	}

	set_cursor(0,1);
	lcd_print("Caractere> ");
	lcd_putchar(caractere);	
}

