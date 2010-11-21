/*
 * Projet: Reseau de capteur
 *
 * Rôle: Gestion des entrées/sorties discretes et des boutons
 * Version: 4.0
 */
#include <stm32f10x_lib.h>
#include "time.h"

TIME_CALLBACK TIMECallback = 0;
int TIMEEnabled;
int TIME10msevent;

/*
 * void TIMEInit (TIME_CALLBACK callback)
 *
 * Initilise le timer periodique et permet de definir une fonction callback appelée toute les 10ms
 * A la fin de cette fonction, le timer est initialisé mais inactif. Pour le demarrer, il faut mettre
 * la variable TIMEEnabled à 1
 *
 * Parametres:
 *     Entrée: 
 *            callback (TIME_CALLBACK): fonction a appeler toutes les 10ms ou NULL 
 *     Sortie:
 *            Aucun
 *
 * Limitations:
 *     Attention: le callback est appelé sous interruption ! Ne faire que des traitements courts avec.
 */
void TIMEInit (TIME_CALLBACK callback)
{
	TIMECallback = callback;
	TIMEEnabled =0;
	TIME10msevent=0;
}

/*
 * int TIME10ms(void)
 *
 * Indique si une periode de 10ms s'est ecoulée sur le timer periodique
 *
 * Parametres:
 *     Entrée: 
 *            Aucun
 *     Sortie:
 *            int : vaut 1 si 10ms s'est ecoulée, 0 sinon 
 *
 * Limitations:
 *     Aucune
 */
int TIME10ms(void)
{ 
int status = FALSE;

	if (TIME10msevent == 1)
	{
		TIME10msevent =0;
		status = TRUE;
	}

	return status;
}

/*
 * void TIMEWait1ms(void)
 *
 * Cree un delai d'1 ms.
 *
 * Parametres:
 *     Entrée: 
 *            Aucun
 *     Sortie:
 *            Aucun 
 *
 * Limitations:
 *     Attention: fonction bloquante ! Ne pas utiliser sous interruption.
 */
void TIMEWait1ms(void)
{
#define _MAX_1MS_ 9000L
volatile unsigned int i;

	for (i=_MAX_1MS_; i!=0; i--); 
}

/*
 * void TIMEWait1ms(void)
 *
 * Cree un delai de x ms.
 *
 * Parametres:
 *     Entrée: 
 *            ms (int): durée en ms.
 *     Sortie:
 *            Aucun 
 *
 * Limitations:
 *     Attention: fonction bloquante ! Ne pas utiliser sous interruption.
 */
void TIMEWaitxms(int ms)
{
volatile int cpt;

	for (cpt=0; cpt<ms; cpt++)
	{
		TIMEWait1ms();	
	}
}

/*
 * void SysTick_Handler(void)
 *
 * routine d'interruption du timer
 *
 * Parametres:
 *     Entrée: 
 *            Aucun
 *     Sortie:
 *            Aucun 
 *
 * Limitations:
 *     Aucune a par que c'est une routine d'interruption
 */
void SysTick_Handler(void)
{
	if (TIMEEnabled)
	{
		TIME10msevent=1;

		if (TIMECallback != 0x0)
		{
			TIMECallback();
		}
	}
}
