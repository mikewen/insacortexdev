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
	RCC->APB2ENR |= RCC_APB2Periph_GPIOB;

	GPIOB->CRH = 0x22222222;   /* Met les ligne 8 à 15 du port B en sortie push-pull, 2 Mhz.
	                              A reprendre, moche  */

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
 * Role: 		Initialisation des touches (PC13 (TAMP) -> Validation, PA0 (WKUP) -> RAZ)
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Init_Touche(void)
{
	RCC->APB2ENR |= RCC_APB2Periph_GPIOA + RCC_APB2Periph_GPIOC;

	/* Les touches sont normallement reglées en entrée par defaut */
}

/* 
 * Fonction: 	Init_Touche
 * Role: 		Lecture d'une touche (PC13 (TAMP) -> Validation, PA0 (WKUP) -> RAZ)
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
		state = GPIOA->IDR & GPIO_Pin_0;
	}
	else
	{
		/* Bouton set == PC13 */
		state = GPIOC->IDR & GPIO_Pin_13;	
	}

	return state ? 0 : 1;
}
