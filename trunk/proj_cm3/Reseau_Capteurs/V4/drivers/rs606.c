#include <stm32f10x_lib.h>
#include "RS606.h"

int etat_RS = RS606_OFF;

void RS606Init(void)
{
	/*Reglage de EXTI */

}

void RS606SetMode(RS606_MODE mode)
{
volatile int temp;

	/* Met les lignes PB9 (TX_CMD) PB8 (RX_CMD) en sortie : fait dans stm32_Init.c */

	/* remet PB8 et PB9 à 0 */
	GPIOB->ODR = GPIOB->ODR & ~(GPIO_Pin_8 + GPIO_Pin_9);

	if (GPIOC->IDR & GPIO_Pin_4) /* Si aucune porteuse n'est detectée (CD à 1) */
	{
		if (mode == RS606_RX) mode = RS606_OFF;
	}

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
			break;
		case RS606_TX_LOOPBACK:
			/* Rien a faire (car un 0 en sortie du STM32 donne un 1 en entrée du module RS606) */
			break;
	}

	etat_RS = mode;
}

void EXTI4_IRQHandler(void)
{
	if (etat_RS != RS606_TX)
	{
		if (GPIOC->IDR & GPIO_Pin_4)
		{
			/* Pas de CD */
			RS606SetMode(RS606_OFF);
		}
		else
		{
			/* CD detectée */
			RS606SetMode(RS606_RX);
		}
	}
}
