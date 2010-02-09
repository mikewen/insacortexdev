/*
 * Ecrire un truc ici
 */

#ifndef __SUPPORT_H__
#define __SUPPORT_H__

/* 
 * Fonctions pour la lecture des touches et l'ecriture des LED
 */

/* 
 * Fonction: 	Init_LED
 * Role: 		Initialisation des leds (PB8 - PB15)
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Init_LED(void);

/* 
 * Fonction: 	Ecrit_LED
 * Role: 		Ecriture du poids fort des LED (PB8 - PB15)
 * Entrée: 		
 *		R0: Valeur a ecrire (octet de poids faible)
 * Sortie: 		Rien
 */
void Ecrit_LED(int val);

/* 
 * Fonctions pour la lecture des touches et l'ecriture des LED
 */
#define BOUTON_EFFACE 	0x01
#define BOUTON_VALID 	0x02

/* 
 * Fonction: 	Init_Touche
 * Role: 		Initialisation des touches (PA0 -> Valide, PC13 -> RAZ)
 * Entrée: 		Rien
 * Sortie: 		Rien
 */
void Init_Touche(void);

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
int Lire_Touche(int button);

#endif /* __SUPPORT_H__ */
