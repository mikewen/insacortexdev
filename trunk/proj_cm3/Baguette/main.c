#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions

#include "lib_baguette.h"

/*
 * Constantes
 */
#define AU_MIN 0
#define MONTE  1
#define AU_MAX   2
#define DESCEND  3

// Seuil pour l'accelerometre (axe X)
#define S_TB  	0x441 	// Seuil_TresBas ((short int)(1.0/3.3*4096.0))
#define S_B  	0x5CA 	// Seuil_Bas ((short int)(1.1/3.3*4096.0))
#define S_H  	0xC41 	// Seuil_Haut ((short int)(2.6/3.3*4096.0))
#define S_TH	0xDCA 	// Seuil_TresHaut ((short int)(2.9/3.3*4096.0))

/* Inclusion de la table de caractere */
#define NB_CARS (4)			// Longueur du texte (en caractere) a afficher
extern unsigned char font8data[];
char texte_baguette[NB_CARS] = {'A','B','C','D'};		// texte a afficher
int index_tableau;			// Index dans la chaine a afficher, mais utilisé par l'interface
#define FONT_SIZE (8)
#define NB_TRAMES (NB_CARS*FONT_SIZE+2) 
char trame[NB_TRAMES];		// Enchainement des differents motifs a enchainer sur les LED pour afficher le texte

/*
 * Declaration des variables
 */

int tmp;		// Variable temporaire utilisé pour stocker le resultat de l'ADC

volatile char phase ; // Indique la phase de mouvement de la baguette. Utilise les constantes 
					  // AU_MIN, MONTE, AU_MAX, DESCEND

u8 index_tab; 	// Index pour la chaine a afficher
u8 index_font;	// index dans la table de police d'ecriture

int potentiometre; 			// Variable temporaire contenant la valeur du potentiometre
char caractere;				// Caractere "selectionné" par le potentiometre


/* 
 * Fonction: 	MAJ_trame
 * Role: 		Construction de la trame (enchainement des differents motifs LED) a sequencer sur le baguette
 * Entrée: 		
 *			R0 = @ du tableau contenant le texte a afficher, avec zero terminal
 *			R1 = @ du tableau prevu pour stocker la trame de motifs LED
 * Sortie: 		Rien
 */
MAJ_trame(char texte_baguette[4], char trame[8*4])
{
	char i,j,k;
	char * avoile;
	
	k=0;

	trame[k++] = 0;

	for (i=0;i<NB_CARS;i++)
	{
		avoile = (char *) (font8data + (texte_baguette[i]*FONT_SIZE)); 
		for(j=0;j<FONT_SIZE;j++)
		{
			 trame[k++]=  *(avoile++);
		}
	}

	trame[k++] = 0;
}

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

	phase = MONTE;
	Watch_For_Higher_Than(S_TH);

	/* Raz de l'index dans la chaine a afficher */
	index_tableau=0;
	MAJ_trame(texte_baguette,trame);

	MAJ_Ecran (texte_baguette,caractere);

	/* Initialisation utilisateur, pour regler les LED en sortie et les touches en entrée */
	Init_LED();
	Init_Touche();

	/* Initailisation des variables globales */
	tmp = 0;
	//sens = SENS_ETEINT;
	index_tab = 0;
	index_font = 0;

	/* Lance le rafraichissement de l'interface */
	Demarre_Timer1();

	/* Lance le timer pour le clignotement des led */
	Demarre_SYSTICK();

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
u8 pattern;

  	switch (phase)
	{
	case AU_MIN:
		/* rien a faire */
		Ecrit_LED(0xFF);
		break;
	case AU_MAX:
		/* rien a faire */
		Ecrit_LED(0xFF);
		break;
	case MONTE:
		/* Sens "aller" */
		pattern = trame[index_font++];
		
		/* Inverse la polarité et affiche le motif */
		Ecrit_LED(~pattern);	
	
		/* Fait avancer les index pour preparer le prochain motif */
		if (index_font>= NB_TRAMES)
		{
			index_font = NB_TRAMES-1;
		}
		break;
	case DESCEND:
		/* Sens "retour" */
		pattern = trame[index_font--];

		/* Inverse la polarité et affiche le motif */
		Ecrit_LED(~pattern);
		
		/* Fait avancer les index pour preparer le prochain motif */
		if (((signed char) index_font) == -1 )
		{
			index_font = 0;
		}
		break;
	}
}

/* 
 * Fonction: 	TIM1_UP_IRQHandler
 * Role: 		Interruption du timer 1 sur overflow
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void TIM1_UP_IRQHandler(void)
{
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
			MAJ_trame(texte_baguette,trame);
		}	
	}	

	/* Si le bouton "EFFACER" est appuyé, RAZ de la chaine et de l'index */
	if (Lire_Touche(BOUTON_EFFACE))
	{
		for (index_tableau=0; index_tableau<NB_CARS; index_tableau++)
		{
			texte_baguette[index_tableau]=' ';	
		}
		MAJ_trame(texte_baguette,trame);

		index_tableau=0;	
	}
	
	MAJ_Ecran (texte_baguette,caractere);
}

 /* 
 * Fonction: 	ADC_IRQHandler
 * Role: 		Vecteur d'interruption de l'ADC: appelée par l'Analog Watchdog en cas de franchissement d'un seuil
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void ADC_IRQHandler	(void)
{
	switch (phase)
	{
		case MONTE : 
			phase = AU_MAX;
			Watch_For_Lower_Than(S_H);
			break;
		case AU_MAX : 
			phase = DESCEND;
			Watch_For_Lower_Than(S_TB);
			index_font = NB_TRAMES-1;	/* fin du pattern a afficher */
			break;
		case DESCEND : 
			phase = AU_MIN;
			Watch_For_Higher_Than(S_B);
			break;
		case AU_MIN : 
			phase = MONTE;
			Watch_For_Higher_Than(S_TH);
			index_font = 0;	/* debut du pattern a afficher */
			break;
		default :
			break; 
	}
	
	/* Acquitement du drapeau d'interruption AWD (Analog Watchdog) pour terminer l'interruption */	
	Acquite_ADC();
}

