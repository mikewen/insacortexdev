/*
 * Projet: Reseau de capteur
 *
 * Rôle: Gestion des entrées/sorties discretes et des boutons
 * Version: 4.0
 */
 
#include <stm32f10x_lib.h>
#include "gpio.h"

/*
 * Variables pour gerer l'antirebond (fonction GPIOButton)
 */

int GPIODebounceBuffer[2][2]; /* Tableau pour l'antirebond, à 2 lignes (pour les deux boutons)
                                 et 2 colonnes (pour les deux etats comparés) */
int GPIOButonLastState[2]= {BUTTON_RELEASED,BUTTON_RELEASED};    /* Tableau pour memoriser l'etat precedent des boutons et detecter les
                                 transitions appuyé->relaché et relaché->appuyé */
/*
 * int GPIOGetState (int port)
 *
 * Retourne l'etat courant d'un GPIO (pas d'antirebond)
 *
 * Parametres:
 *     Entrée: 
 *            port (int): identifiant du port
 *     Sortie:
 *            int: etat du port (0 ou different de 0) ou -1 si l'identifiant du bouton est invalide 
 *
 * Limitations:
 *     Aucune
 */
int GPIOGetState (int port)
{
int state =0;

	switch (port)
	{
	case BUTTON_TAMP:
		state = (GPIOC->IDR & GPIO_Pin_13);
		break;
	case BUTTON_WKUP:
		state = (GPIOA->IDR & GPIO_Pin_0);
		break;
	default:
		state = -1;	
	}	

	return state;
}

/*
 * int GPIOButton (int button)
 *
 * Retourne l'etat d'un bouton (avec antirebond)
 * 
 * Parametres:
 *     Entrée: 
 *            button (int): identifiant du bouton
 *     Sortie:
 *            int: etat du bouton (PRESSED, JUST_PRESSED, RELEASED, JUST_RELEASED) ou -1 si l'identifiant du bouton est invalide
 *
 * Limitations:
 *     Pour que l'antirebond fonctionne correctement, cette fonction ne doit pas etre appelée à une
 *     periode inferieur à 50ms.
 */
int GPIOButton (int button)
{
	/* Le dernier echantillon devient le plus ancien, et l'etat actuel du bouton est echantillonné */
	GPIODebounceBuffer[button][0] = GPIODebounceBuffer[button][1];
	GPIODebounceBuffer[button][1] = GPIOGetState(button);

	/* Si l'identifiant du bouton est invalide, renvoi -1 */
	if (GPIODebounceBuffer[button][1]==-1) return -1;
	 
	if (GPIODebounceBuffer[button][0] == GPIODebounceBuffer[button][1])	/* Le bouton est stable */
	{
		if (GPIOButonLastState[button] == GPIODebounceBuffer[button][1]) /* le bouton n'a pas changé d'etat depuis la derniere fois */
		{
			if (GPIODebounceBuffer[button][1] == 0) return BUTTON_PRESSED;
			else return BUTTON_RELEASED;
		}
		else /* Le bouton vient de changer d'etat */
		{
			GPIOButonLastState[button] = GPIODebounceBuffer[button][1]; /* Au memorise l'etat courant pour detecter un changement d'etat
			                                                               la prochaine fois */
			
			/* Transition bouton relaché-> appuyé */ 
			if (GPIODebounceBuffer[button][1]==0) return BUTTON_JUST_PRESSED;
			else return BUTTON_JUST_RELEASED;
		} 				
	}
	else 
	{
		if (GPIOButonLastState[button] == 0) return BUTTON_PRESSED; /* Le bouton n'est pas stable, on renvoi l'etat stable precedent */
		else return BUTTON_RELEASED;
	}

/* Suppression du warning qui indique que la fonction se termine sans return. ARMCC n'est pas trés futé sur ce coup là ... */
#pragma diag_remark	940
}
