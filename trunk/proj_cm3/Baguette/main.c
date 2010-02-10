#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions

#include "lib_baguette.h"
#include "support.h"

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

	/* Lance le timer pour le clignotement des led */
	Demarre_SYSTICK();

	while (1)
	{
		/* Dessus MAX max */
		while (tmp <MAX_max)
		{
			 tmp = Lire_ADC(ADC_X);
		}

		sens = SENS_ETEINT;

	   	/* Dessous MAX min */
		while (tmp >MAX_min)
		{
			 tmp = Lire_ADC(ADC_X);
		}

		sens = SENS_ALLER;
		index_font = 0;	/* debut du pattern a afficher */
		index_tab = 0; /* debut du tableau */

		/* Dessous MIN min */
		while (tmp >MIN_min)
		{
			 tmp = Lire_ADC(ADC_X);
		}

		sens = SENS_ETEINT;

		/* Dessus MIN max */
		while (tmp <MIN_max)
		{
			 tmp = Lire_ADC(ADC_X);
		}

		sens = SENS_RETOUR;
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
	case SENS_ETEINT:
		/* rien a faire */
		Ecrit_LED(0xFF);
		break;
	case SENS_ALLER:
		/* Sens "aller" */
		/* Recupere dans la police d'ecriture le motif de led a afficher */
		caractere = texte_baguette[index_tab];
		pattern = font8data[(caractere*8) + index_font];
		
		/* Inverse la polarité et affiche le motif */
		Ecrit_LED(~pattern);

		/* Fait avancer les index pour preparer le prochain motif */
		index_font++;
		if (index_font>=8)
		{
			index_font = 0;

			index_tab ++;
			if (index_tab>=4)
			{
				sens = SENS_ETEINT;
			}
		}
		break;
	case SENS_RETOUR:
		/* Sens "retour" */
		/* Recupere dans la police d'ecriture le motif de led a afficher */
		caractere = texte_baguette[index_tab];
		pattern = font8data[(caractere*8) + index_font];
		
		/* Inverse la polarité et affiche le motif */
		Ecrit_LED(~pattern);

		/* Fait avancer les index pour preparer le prochain motif */
		if (index_font==0)
		{
			index_font = 7;

			if (index_tab==0)
			{
				sens = SENS_ETEINT;
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
	Ecrit_Chaine_Ecran("Texte:");
	Position_Curseur(0,1);
	Ecrit_Chaine_Ecran("Caractere> "); 

	/* RAZ de la chaine a afficher */
	for (index_tableau=0; index_tableau<4; index_tableau++)
	{
		texte_baguette[index_tableau]=' ';	
	}

	/* Raz de l'index dans la chaine a afficher */
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

	/* Acquitement de l'IT timer */
	Acquite_Timer1();

	/* Recuperation du caractere selectionné apr le potentiometre */
	potentiometre = Lire_ADC(ADC_POTENTIOMETRE);
	caractere = ((potentiometre*25)/0xFFF) + 'A';
	
	/* Si le bouton "VALIDATION" est enfoncé, le caractere est ajouté 
	   à la chaine a afficher (sauf si la chaine est pleine) */	
	if (Lire_Touche(BOUTON_VALID))
	{
		if (index_tableau<4)
		{
			texte_baguette[index_tableau++]= caractere;
		}	
	}	

	/* Si le bouton "EFFACER" est appuyé, RAZ de la chaine et de l'index */
	if (Lire_Touche(BOUTON_EFFACE))
	{
		for (index_tableau=0; index_tableau<4; index_tableau++)
		{
			texte_baguette[index_tableau]=' ';	
		}

		index_tableau=0;	
	}
	
	/* Rafraichissement de l'ecran avec les nouvelle infos */
	Efface_Ecran();

	/* Affichage du texte "baguette" */
	Position_Curseur(0,0);
	Ecrit_Chaine_Ecran("Texte: ");

	for (i=0; i<4; i++)
	{
		Ecrit_Ecran(texte_baguette[i]);	
	}

	/* Affichage du caractere selectionné par le potar */
	Position_Curseur(0,1);
	Ecrit_Chaine_Ecran("Caractere> ");
	Ecrit_Ecran(caractere);	
}

