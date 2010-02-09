/*
 * Ecrire un truc ici
 */

#include <stm32f10x_lib.h>
#include "stm_clock.h"
#include "stm_usartx.h"
#include "stm_system.h"

#include "lcd.h"
#include "led.h"

FILE UART;

void Init_Baguette(void)
{
	/* Init des horloges */
	Init_Clock_System();

	/* Init de l'ecran LCD */
	lcd_init();

	/* Init du port serie (utilisé pour la simu) */
	setup_usart();

	/* Init des ADC */

	/* Init des boutons */

	/* Init des niveaux d'IT */
}

/* Fonctions pour la gestion de l'ecran LCD */
void Efface_Ecran(void)
{
	/* Pour l'ecran */
	lcd_clear();

	/* pour la LS */
	fputc('\n',&UART);
	fputc('\n',&UART);
}

void Ecrit_Ecran(char c)
{
	/* Pour l'ecran */
	lcd_putchar(c);

	/* pour la LS */
	fputc(c,&UART);
}

void Position_Curseur(int x, int y)
{
	/* Pour l'ecran */
	set_cursor(x,y);

	/* pour la LS */
	fputc('\n',&UART);	
}






