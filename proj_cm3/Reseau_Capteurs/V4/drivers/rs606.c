/*
 * Projet: Reseau de capteur
 *
 * Rôle: Gestion du module FM RS606
 * Version: 4.0
 */
#include <stm32f10x_lib.h>
#include "RS606.h"
#include "uart.h"
#include "time.h"

extern char buffer_RS606[200];
int etat_RS = RS606_OFF;

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
void RS606SetMode(RS606_MODE mode)
{
volatile int temp;

	if (mode != etat_RS) /* Inutile de modifier la config du module RS606 si c'est la meme que celle deja en cours */
	{
		/* Met les lignes PB9 (TX_CMD) PB8 (RX_CMD) en sortie : fait dans stm32_Init.c */
	
		/* Desactive la voie montante RX du RS606 */
		UARTDesactiverRXRS606();
	
		/* remet PB8 et PB9 à 0 */
		GPIOB->ODR = GPIOB->ODR & ~(GPIO_Pin_8 + GPIO_Pin_9);
	
		switch (mode)
		{
			case RS606_OFF:
				GPIOB->ODR = GPIOB->ODR | GPIO_Pin_8; // met la ligne RX_CMD à 1 (donc 0 en entrée du RS606) */
				GPIOB->ODR = GPIOB->ODR | GPIO_Pin_9; // met la ligne TX_CMD à 1 (donc 0 en entrée du RS606) */
				break;
			case RS606_TX:
				GPIOB->ODR = GPIOB->ODR | GPIO_Pin_8; // met la ligne RX_CMD à 1 (donc 0 en entrée du RS606) */ 
				break;
			case RS606_RX:
				GPIOB->ODR = GPIOB->ODR | GPIO_Pin_9; // met la ligne TX_CMD à 1 (donc 0 en entrée du RS606) */
	
				/* Attente de 1 ms */
				if ((GPIOC->IDR & GPIO_Pin_4)==0) /* Si une porteuse est detectée (CD à 0) */
				{
					UARTActiverRXRS606();	
				}
				break;
			case RS606_TX_LOOPBACK:
				/* Rien a faire (car un 0 en sortie du STM32 donne un 1 en entrée du module RS606) */
				break;
		}
	
		etat_RS = mode;
	}
}

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
void EXTI4_IRQHandler(void)
{
	//TIMEWait1ms();
	EXTI->PR = EXTI->PR |(1<<4);

	if (etat_RS != RS606_TX)
	{
		if (GPIOC->IDR & GPIO_Pin_4)
		{
			/* Pas de CD -> Desactiver l'UART RX*/
			UARTDesactiverRXRS606();

			GPIOB->ODR = GPIOB->ODR & ~(1<<15);

			USARTFlushBuffer();
			buffer_RS606[0] = 0;
		}
		else
		{
			/* CD detectée -> Activer l'UART RX */
			UARTActiverRXRS606();

			GPIOB->ODR = GPIOB->ODR | (1<<15);
		}
	}
}
