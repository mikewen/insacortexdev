/*
 * Projet: Reseau de capteur
 *
 * Rôle: Gestion des entrées/sorties discretes et des boutons
 * Version: 4.0
 */
#ifndef __GPIO_H__
#define __GPIO_H__

#include <stm32f10x_lib.h>

/* 
 * Definition des identifiants des GPIOs et boutons
 *
 * Si d'autres boutons ou GPIO sont necessaires, les rajouter dans la liste
 */
enum 
{
	BUTTON_TAMP=0,	  /* Fixé à zero pour pouvoir s'en servir d'index dans le tableau GPIODebounceBuffer */
	BUTTON_WKUP
};

/*
 * Definition des etats de sortie pour GPIOButton
 */
enum
{
	BUTTON_PRESSED,
	BUTTON_JUST_PRESSED,
	BUTTON_RELEASED,
	BUTTON_JUST_RELEASED
};

/*
 * Fonctions exportées (publiques)
 */
int GPIOGetState (int port);
int GPIOButton (int button);

#endif /* __GPIO_H__ */
