/*
 * Ecrire un truc ici
 */

#include <stm32f10x_lib.h>
#include "support.h"

/* 
 * Fonctions pour la lecture des touches et l'ecriture des LED
 */
#define MASK	0xFF00

/* 
 * Fonction: 	Init_LED
 * Role: 		Initialisation des leds (PB8 - PB15)
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Init_LED(void)
{
	RCC->APB2ENR |= (1<<3);

	GPIOB->CRH = 0x22222222;   /* A reprendre  */

	GPIOB->ODR = GPIOB->ODR & (~MASK); 
}

/* 
 * Fonction: 	Ecrit_LED
 * Role: 		Ecriture du poids fort des LED (PB8 - PB15)
 * Entrée: 		
 *		R0: Valeur a ecrire (octet de poids faible)
 * Sortie: 		Rien
 */
void Ecrit_LED(int val)
{
	GPIOB->ODR = GPIOB->ODR & (~MASK); 		// Remet les led à zero
	GPIOB->ODR = GPIOB->ODR | (val<<8); 	// Ecrit val sur PB8-> PB15
}

/* 
 * Fonction: 	Init_Touche
 * Role: 		Initialisation des touches (PA0 -> Valide, PC13 -> RAZ)
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Init_Touche(void)
{
	RCC->APB2ENR |= (1<<2) + (1<<4);

	/* Les touches sont normallement reglées en entrée par defaut */
}

/* 
 * Fonction: 	Init_Touche
 * Role: 		Lecture d'une touche (PC13 -> Validation, PA0 -> RAZ)
 * Entrée: 		
 *		R0: Bouton a lire
 *			1 = Bouton Effacement (BOUTON_EFFACE)
 *			2 = Bouton Validation (BOUTON_VALID)
 * Sortie: 
 *		R0: Etat du bouton
 *			0 = Bouton appuyé
 *			Different de 0 = Bouton relaché
 */
int Lire_Touche(int button)
{
int state;

	state = 0;

	if (button == BOUTON_EFFACE)
	{
		/* Bouton clear	== PA0 */
		state = GPIOA->IDR & 0x01;
	}
	else
	{
		/* Bouton set == PC13 */
		state = GPIOC->IDR & (0x01<<13);	
	}

	return state ? 0 : 1;
}
