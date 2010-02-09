#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions

#include "lib_baguette.h"
#include "support.h"
//#include <stdio.h>

extern unsigned char font8data[];

// Main
volatile u32 tmp;
volatile u8 sens;
volatile u8 index_tab;
volatile u8 index_font;

char text[50];

int compteur_ihm;

volatile u8 tick;

char texte_baguette[4];
int index_tableau;

volatile int adc;
volatile char caractere;

#define MAX_max 2700
#define MAX_min	2400
#define MIN_max	300
#define MIN_min	150

void Init_Interface(void);
void Interface_MAJ(void);

/* 
 * Fonction: 	main
 * Role: 		Fonction principale du programme
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
int main (void)
{
	Init_Baguette();
	Init_Interface();

	Init_LED();
	Init_Touche();

	tmp = 0;
	sens = 0; /* si sens = 0 -> ne rien faire
	                sens = 1 -> Sens direct
					sens = 2 -> Sens indirect */

	index_tab = 0;
	index_font = 0;
	tick = 0;

	compteur_ihm = 0;

	/* Lance le rafraichissement de l'interface */
	Demarre_Timer1();

	/* Lance le timer pour le clignotement des led */
	Demarre_SYSTICK();

	while (1)
	{
		/* Dessus MAX max */
		while (tmp <MAX_max)
		{
			 tmp = Lire_ADC(ADC_X);
		}

		sens = 0;

	   	/* Dessous MAX min */
		while (tmp >MAX_min)
		{
			 tmp = Lire_ADC(ADC_X);
		}

		sens = 1;
		index_font = 0;	/* debut du pattern a afficher */
		index_tab = 0; /* debut du tableau */

		/* Dessous MIN min */
		while (tmp >MIN_min)
		{
			 tmp = Lire_ADC(ADC_X);
		}

		sens = 0;

		/* Dessus MIN max */
		while (tmp <MIN_max)
		{
			 tmp = Lire_ADC(ADC_X);
		}

		sens = 2;
		index_font = 7;	/* fin du pattern a afficher */
		index_tab = 3; /* fin du tableau */
	}
}

/* 
 * Fonction: 	SysTick_Handler
 * Role: 		Vecteur d'IT du timer systeme, utilisé pour cadencer la clignotement des LED
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void SysTick_Handler (void) 
{
u8 caractere;
u8 pattern;

  	switch (sens)
	{
	case 0:
		/* rien a faire */
		Ecrit_LED(0xFF);
		break;
	case 1:
		/* Sens direct */
		caractere = texte_baguette[index_tab];
		pattern = font8data[(caractere*8) + index_font];
		Ecrit_LED(~pattern);

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
		Ecrit_LED(~pattern);

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

/* 
 * Fonction: 	Init_Interface
 * Role: 		Initialisation de la partie affichage (interface) pour afficher le texte et saisir les caracteres
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Init_Interface(void)
{	
	/* Effacement de l'ecran */
	Efface_Ecran();
	Position_Curseur(0,0);

	Ecrit_Chaine_Ecran("Texte:");

	Position_Curseur(0,1);
	Ecrit_Chaine_Ecran("Caractere> "); 

	for (index_tableau=0; index_tableau<4; index_tableau++)
	{
		texte_baguette[index_tableau]=' ';	
	}

	index_tableau=0;
}

/* 
 * Fonction: 	TIM1_UP_IRQHandler
 * Role: 		Interruption du timer 1 sur overflow
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void TIM1_UP_IRQHandler(void)
{
int i;

	Acquite_Timer1();

	adc = Lire_ADC(ADC_POTENTIOMETRE);
	
	caractere = ((adc*25)/0xFFF) + 'A';
		
	if (Lire_Touche(BOUTON_VALID))
	{
		if (index_tableau<4)
		{
			texte_baguette[index_tableau++]= caractere;
		}	
	}	

	if (Lire_Touche(BOUTON_EFFACE))
	{
		for (index_tableau=0; index_tableau<4; index_tableau++)
		{
			texte_baguette[index_tableau]=' ';	
		}

		index_tableau=0;	
	}
	
	Efface_Ecran();
	Position_Curseur(0,0);
	Ecrit_Chaine_Ecran("Texte: ");

	for (i=0; i<4; i++)
	{
		Ecrit_Ecran(texte_baguette[i]);	
	}

	Position_Curseur(0,1);
	Ecrit_Chaine_Ecran("Caractere> ");
	Ecrit_Ecran(caractere);	
}

