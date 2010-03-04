#include "uart.h"
#include "adc.h"
#include "time.h"
#include "at.h"
#include "gpio.h"
#include "lcd.h"
#include "rs606.h"

#include "STM32_Init.h"

#define GSM		1
#define XBEE	2
#define RS606	3

u16 ADC_data;
AT_STATUS Status;
u8 preScaler;
u8 Caractere;

void Delay_1_RS606(void)
{
volatile int i;
	
	for (i=0; i<4000; i++);
}

void Delay_2_RS606(void)
{
volatile int i;
	
	for (i=0; i<40000; i++);
}

void envoyerCaractereXBEE(char C)
{
	set_cursor(1,1);
	lcd_print("=>Envoi de : ");
	lcd_putchar(C);
	
	putchar(XBEE,C);
}

void envoyerCaractereRS606(char C)
{
//u8 temp;

	set_cursor(1,1);
	lcd_print("=>Envoi de : ");
	lcd_putchar(C);

	//RS606SetMode(RS606_TX);	
	Delay_1_RS606();
	putchar(RS606,C);
	Delay_1_RS606();
	//RS606SetMode(RS606_RX);
	/*Delay_1_RS606();
	Delay_1_RS606();
	RS606SetMode(RS606_RX);
	getchar_NB(RS606,&temp);*/
}
	
void MonCallback (void)	;

int main (void)
{
	/* Init du micro et des peripheriques */
	stm32_Init ();
	lcd_init();	
	
	/* Demarrage du service Timer */
	//initialisation du prescaler
	preScaler = 0 ; 
	//avec appel de la routine MonCallback toutes les 10ms
	TIMEInit( MonCallback);	

	/* Affichage d'un message d'accueil */
	lcd_clear();

	RS606SetMode(RS606_RX);

#ifdef _IS_SLAVE_
	/* Esclave (recepteur) */
	RS606SetMode(RS606_RX);
		
	lcd_print("En attente de");  
	set_cursor(1,1);
	lcd_print("l'initiateur");  
#else /* _IS_MASTER_ */
	/* En maitre (emetteur) */
	RS606SetMode(RS606_TX);	

	lcd_print("Initiateur"); 
/*	Caractere = 'a';
	envoyerCaractereRS606(Caractere); */
#endif /* _IS_SLAVE_ */

	while (1)
	{}
}

void MonCallback (void)
{
static u8 caractere = 'a';
bool caractereLu;

	preScaler++;
	if (preScaler==100)
	{
/*		bool caractereLu;
		preScaler = 0;		
		caractereLu=getchar_NB(RS606,&Caractere);		
		if (caractereLu != FALSE)
		{
			if (Caractere == ('z'))
			{
				lcd_print("Reception de : ");
				lcd_putchar(Caractere);
				Caractere ='a';
				lcd_clear();
				lcd_print("Forcage a 'a'");				
				envoyerCaractereRS606(Caractere);
			}
			else
			{ 
				lcd_clear();
				lcd_print("Reception de : ");
				lcd_putchar(Caractere);		 
				envoyerCaractereRS606(Caractere+1);
			}
		}
		else
		{
			lcd_clear();
			lcd_print("No character");  
		}
	}*/

#ifdef _IS_MASTER_
   		envoyerCaractereRS606(caractere);

		caractere++;

		if (caractere>'z') caractere = 'a';
#else /* _IS_SLAVE_ */
		caractereLu=getchar_NB(RS606,&caractere);
		
		if (caractereLu != FALSE)
		{
			lcd_clear();
			lcd_print("Reception de : ");
			lcd_putchar(caractere);	
		}
		else
		{
			lcd_clear();
			lcd_print("No character");
		}
#endif /* _IS_MASTER_ */
	}	
}

