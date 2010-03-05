#include "uart.h"
#include "adc.h"
#include "time.h"
#include "at.h"
#include "gpio.h"
#include "lcd.h"
#include "rs606.h"

#include "STM32_Init.h"
#include <stdio.h>

#define GSM		&UART_1
#define XBEE	&UART_2
#define RS606	&UART_3

#define GSM_NB		&UART_1_NB
#define XBEE_NB		&UART_2_NB
#define RS606_NB	&UART_3_NB

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
	
	fputc(C, XBEE);
}

void envoyerCaractereRS606(char C)
{
//u8 temp;

	set_cursor(1,1);
	lcd_print("=>Envoi de : ");
	lcd_putchar(C);
	
//	Delay_1_RS606();
	fputc(C, RS606);
//	Delay_1_RS606();
}
	
void MonCallback (void)	;

int main (void)
{
	/* Init du micro et des peripheriques */
	stm32_Init ();
	uart_init();
	lcd_init();	

	/* Flux par defaut */
	stdin->_file = UART_1._file;
	stdout->_file = UART_2._file;

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
static signed char caractere = 'a';

	preScaler++;

	if (preScaler==100)
	{
		preScaler = 0;

#ifdef _IS_MASTER_
   		envoyerCaractereRS606(caractere);

		caractere++;

		if (caractere>'z') caractere = 'a';
#else /* _IS_SLAVE_ */
		//caractere=fgetc(RS606_NB);
		
		if (UART_Buffer_State(RS606) != EMPTY)
		{
			caractere=fgetc(RS606);

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

