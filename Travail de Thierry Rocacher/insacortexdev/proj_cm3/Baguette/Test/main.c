#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions

#include "lib_baguette.h"
#include "support.h"

#include <stdio.h>

/* Inclusion de la table de caractere */
extern unsigned char font8data[];

/*
 * Prototype des focntions locales
 */
void Init_Interface(void);
void Interface_MAJ(void);

/*
 * Declaration des variables
 */

int tmp;		// Variable temporaire utilisé pour stocker le resultat de l'ADC

int sens;		// Indique le sens de defilement sur la baguette. Utilise les constantes 
				// SENS_ETEINT, SENS_ALLER, SENS_RETOUR

u8 index_tab; 	// Index pour la chaine a afficher
u8 index_font;	// index dans la table de police d'ecriture

char texte_baguette[4];		// texte a afficher
int index_tableau;			// Index dans la chaine a afficher, mais utilisé par l'interface

int potentiometre; 			// Variable temporaire contenant la valeur du potentiometre
char caractere;				// Caractere "selectionné" par le potentiometre

char buffer[50];			// tempon pour la fonction sprintf
int ADC_temp;				// Var temporaire pour l'ADC
/*
 * Constantes
 */

// Seuils
#define MAX_max 0xDCA
#define MAX_min	0xC41
#define MIN_max	0x5CA
#define MIN_min	0x441

// Constantes de sens
#define SENS_ETEINT	0
#define SENS_ALLER	1
#define SENS_RETOUR	2

/* 
 * Fonction: 	main
 * Role: 		Fonction principale du programme
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
int main (void)
{
	/* Initialisation des peripheriques */
	Init_Baguette();

	/* Initialisation de l'interface (IHM) de saisi du texte */
	Init_Interface();

	/* Initialisation utilisateur, pour regler les LED en sortie et les touches en entrée */
	Init_LED();
	Init_Touche();

	/* Initailisation des variables globales */
	tmp = 0;
	sens = SENS_ETEINT;
	index_tab = 0;
	index_font = 0;

	/* Lance le rafraichissement de l'interface */
	Demarre_Timer1();

	while (1)
	{

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

}

/* 
 * Fonction: 	Init_Interface
 * Role: 		Initialisation de la partie affichage (interface) pour afficher le texte et saisir les caracteres
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Init_Interface(void)
{	
	/* Reset de l'ecran */
	Efface_Ecran();
	
	/* Affichage du menu ecran */
	Position_Curseur(0,0);
	Ecrit_Chaine_Ecran("ADC_X = ");
	Position_Curseur(0,1);
	Ecrit_Chaine_Ecran("ADC_Y = "); 
}

/* 
 * Fonction: 	TIM1_UP_IRQHandler
 * Role: 		Interruption du timer 1 sur overflow
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void TIM1_UP_IRQHandler(void)
{
static int led_courante = 8;

	/* Acquitement de l'IT timer */
	Acquite_Timer1();

	/* Allume la led correspondante */
	GPIOB->ODR |= 0xFF00;	/* Remet a 1 l'etat des led (eteint sur la baguette)*/
	GPIOB->ODR &= ~(1<<led_courante);
	
	led_courante++;
	if (led_courante>=16) led_courante = 8;	

	/* Rafraichissement de l'ecran avec les nouvelle infos */
	Efface_Ecran();

	/* Affichage du texte "baguette" */
	Position_Curseur(0,0);
	ADC_temp = Lire_ADC(ADC_X);
	ADC_temp = ADC_temp >> 4;
	sprintf (buffer, "ADC_X = 0x%X", ADC_temp); 
	Ecrit_Chaine_Ecran(buffer);

	Position_Curseur(0,1);
	ADC_temp = Lire_ADC(ADC_Y);
	ADC_temp = ADC_temp >> 4;
	sprintf (buffer, "ADC_Y = 0x%X", ADC_temp);
	Ecrit_Chaine_Ecran(buffer); 
}

